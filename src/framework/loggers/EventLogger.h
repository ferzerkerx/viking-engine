#ifndef __EVENTLOGGER_H__
#define __EVENTLOGGER_H__


#define ENABLE_LOGGING

#ifdef ENABLE_LOGGING

#include <vector>
#include <map>
#include <cstdarg>
#include <cstdio>
#include <cassert>

#define FN(var_1)                   EventLogFN obj____unique_name(var_1)
#define LOG                         logger->LogEvent
#define LOG_INIT                    logger->Init
#define LOG_UPDATE                  logger->Update
#define LOG_PAUSE                   logger->Pause
#define LOG_FLUSH                   logger->Flush


const int MAX_DEBUG_LINE_LEN = 1024;
/**
* @class EventLogFN
* @author Fernando Montes de Oca Cespedes
* @date Tuesday, October 23, 2007 2:28:41 PM
*/
class EventLogFN {
public:
    explicit EventLogFN(const char *function_name);

    ~EventLogFN();
};



typedef std::vector<const char *> CallStackVector;

/**
* @class EventLogger
* @author Fernando Montes de Oca Cespedes
* @date Tuesday, October 23, 2007 2:22:31 PM
*/
class EventLogger {
public:
    EventLogger();

    virtual bool Init(const char *log_name);

    virtual void Pause();

    bool is_initialized();

    void LogEvent(unsigned int flags, const char *format, ...);

    void LogEvent(const char *format, ...);

    void Update();

    virtual void Flush()=0;

    void PushFunction(const char *name);

    void PopFunction();

    virtual ~EventLogger();

protected:

    virtual void Output(const char *buffer, unsigned int flags) = 0;

    virtual void StartCallStackLevel(const char *str) = 0;

    virtual void EndStackCallLevel() = 0;

    virtual void Update(int hours, int minutes, int seconds, int hundreth_seconds) = 0;

    void LogOutput(char *buffer, unsigned int flags);

    void LogCallStack();

    unsigned int update_count_;
private:
    CallStackVector call_stack_vector_;
    bool has_logged_event_;
    unsigned int previous_stack_level_;
    bool is_initialized_;
};


extern EventLogger *logger;

#else

class EventLogger{};

#define LOG		0 &&
#define FN(x_)
#define LOG_INIT(x_)
#define LOG_UPDATE()
#define LOG_PAUSE()
#define LOG_FLUSH()


#endif //#ifdef ENABLE_LOGGING

#endif //class