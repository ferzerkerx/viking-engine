#include "ConsoleEventLogger.h"

#ifdef ENABLE_LOGGING


void ConsoleEventLogger::Output(const char *buffer, unsigned int flags) {
    printf(" %s\n", buffer);
}

void ConsoleEventLogger::StartCallStackLevel(const char *str) {
    printf("%s\n", str);
}

void ConsoleEventLogger::EndStackCallLevel() {
    printf("\n");
}

void ConsoleEventLogger::Update(int hours, int minutes, int seconds, int hundrethSeconds) {
    printf("Update = %d, Time = %.2d:%.2d:%.2d.%.2d\n", update_count_, hours, minutes, seconds,
           hundrethSeconds);
}


void ConsoleEventLogger::Flush() {
    //Nothing to do here
}

#endif //logging