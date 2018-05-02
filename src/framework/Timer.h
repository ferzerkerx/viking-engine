#ifndef __TIMER__
#define __TIMER__

class Timer {
private:
    unsigned int reset_time_;
    unsigned int current_time_;
    unsigned int last_call_;

public:
    Timer();

    ~Timer();

    int Milliseconds();

    int MillisecondsSinceLastCall();

    int Seconds();

    int SecondsSinceLastCall();

    void Reset();

};

#endif