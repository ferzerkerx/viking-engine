#include "EventLogger.h"
#include <cstring>

#ifdef ENABLE_LOGGING


EventLogFN::EventLogFN(const char *function_name) {
    logger->PushFunction(function_name);
}


EventLogFN::~EventLogFN() {
    logger->PopFunction();
}

EventLogger::EventLogger() {
    update_count_ = 0;
    has_logged_event_ = false;
    is_initialized_ = false;
    previous_stack_level_ = 0;
    call_stack_vector_.reserve(32);
}

EventLogger::~EventLogger() = default;


bool EventLogger::Init(const char *log_name) {
    if (is_initialized()) {
        return false;
    }

    is_initialized_ = true;
    has_logged_event_ = true;
    previous_stack_level_ = 0;

    return true;
}

bool EventLogger::is_initialized() {
    return is_initialized_;
}


void EventLogger::Pause() {
    is_initialized_ = false;
}

void EventLogger::LogEvent(unsigned int flags, const char *format, ...) {
    if (!is_initialized()) {
        return;
    }
    char buffer[MAX_DEBUG_LINE_LEN];
    va_list args;
    va_start(args, format);
    int buf = vsnprintf(buffer, MAX_DEBUG_LINE_LEN, format, args);
    assert(buf >= 0);
    va_end(args);

    LogOutput(buffer, flags);
}

void EventLogger::LogEvent(const char *format, ...) {
    if (!is_initialized()) {
    return;
    }
    char buffer[MAX_DEBUG_LINE_LEN];
    va_list args;
    va_start(args, format);

    int buf = vsnprintf(buffer, MAX_DEBUG_LINE_LEN, format, args);
    assert((buf >= 0) && (buf < MAX_DEBUG_LINE_LEN));
    va_end(args);

    LogOutput(buffer, 0);
}


void EventLogger::Update() {
    if (!is_initialized())
        return;

    update_count_++;

    LogCallStack();

    if (has_logged_event_) {

        int hundrethSeconds = update_count_ * 100 / 60 % 100;
        int seconds = update_count_ / 60 % 60;
        int minutes = update_count_ / 3600 % 60;
        int hours = update_count_ / 216000;

        Update(hours, minutes, seconds, hundrethSeconds);
    }

    has_logged_event_ = false;

    Flush();

}


void EventLogger::PushFunction(const char *name) {
    if (!is_initialized()) {
        return;
    }
    call_stack_vector_.push_back(name);
}


void EventLogger::PopFunction() {
    if (!is_initialized() || call_stack_vector_.empty()) {
        return;
    }
    call_stack_vector_.pop_back();
    if (previous_stack_level_ >= call_stack_vector_.size()){
        LogCallStack();
    }
}


void EventLogger::LogOutput(char *buffer, unsigned int flags) {
    auto i = static_cast<int>(strlen(buffer));
    if (i == 0)
        return;
    if (buffer[i - 1] == '\n')
        buffer[i - 1] = 0;

    has_logged_event_ = true;

    LogCallStack();

    Output(buffer, flags);
}

void EventLogger::LogCallStack() {
    auto current_stack_level = static_cast<unsigned int>(call_stack_vector_.size());

    while (previous_stack_level_ < current_stack_level) {
        StartCallStackLevel(call_stack_vector_[previous_stack_level_]);
        previous_stack_level_++;
    }

    while (previous_stack_level_ > current_stack_level) {
        EndStackCallLevel();
        previous_stack_level_--;
    }
}

#endif // ENABLE_LOGGING
