#ifndef FANSCHEDULER_H
#define FANSCHEDULER_H

class FanScheduler
{
public:
    virtual ~FanScheduler() { }

    // to initialize the scheduler before/after use
    virtual void init() = 0;
    virtual void fini() = 0;

    // called in loop every time
    virtual bool isTimeToRun(int fanState) = 0;

};


#endif
