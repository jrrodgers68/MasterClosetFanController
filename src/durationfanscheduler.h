#ifndef DURATIONFANSCHEDULER_H
#define DURATIONFANSCHEDULER_H

#include "constants.h"
#include "fanscheduler.h"


class DurationFanScheduler : public FanScheduler
{
public:
    DurationFanScheduler();

    virtual ~DurationFanScheduler();
    virtual void init();
    virtual void fini();
    virtual bool isTimeToRun();

private:
    int _lastRun = 0;
    int _runDuration; // in minutes, amount of time to run the fan
    int _runCycle; // in minutes, time block - fan runs for _runDuration minutes within a _runCycle block (e.g. runDuartion 5 mins, runCycle 30 mins)
    String _nextRun;

    // call backs for registered events
    void setDuration(const char *event, const char *data);
    void setCycle(const char *event, const char *data);
};

#endif
