#include "EventLogger.h"

#ifndef __CONSOLEEVENTLOGGER_H__
#define __CONSOLEEVENTLOGGER_H__

#ifdef ENABLE_LOGGING

/**
* @class ConsoleEventLogger
* @author Fernando Montes de Oca Cespedes
* @date Tuesday, October 23, 2007 12:36:25 PM
*
*/
class ConsoleEventLogger : public EventLogger {

private:
    void Output(const char *buffer, unsigned int flags) override;

    void StartCallStackLevel(const char *str) override;

    void EndStackCallLevel() override;

    void Update(int hours, int minutes, int seconds, int hundrethSeconds) override;

public:
    void Flush() override;
};

#else

class ConsoleEventLogger : public EventLogger{};

#endif //logging

#endif //class