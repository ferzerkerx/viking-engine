#include <ctime>
#include "Timer.h"

static unsigned int getTickCount() {
    struct timespec now{};
    if (clock_gettime(CLOCK_MONOTONIC, &now)) {
        return 0;
    }
    return static_cast<unsigned int>(now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0);
}

Timer::Timer() {
    reset_time_ = getTickCount();
    current_time_ = getTickCount();
    last_call_ = getTickCount();
}

Timer::~Timer() = default;

int Timer::Milliseconds() {
    current_time_ = getTickCount();
    return current_time_ - reset_time_;
}

int Timer::MillisecondsSinceLastCall() {
    int res = last_call_ - getTickCount();
    last_call_ = getTickCount();
    return res;

}

int Timer::Seconds() {
    current_time_ = getTickCount();
    return (current_time_ - reset_time_) / 1000;

}

int Timer::SecondsSinceLastCall() {
    int res = last_call_ - getTickCount();
    last_call_ = getTickCount();
    return res / 1000;
}

void Timer::Reset() {
    reset_time_ = current_time_;
}

