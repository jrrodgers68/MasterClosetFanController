#include "Particle.h"
#include "durationfanscheduler.h"


DurationFanScheduler::DurationFanScheduler()
{
    _runDuration = DEFAULT_RUN_DURATION;
    _runCycle = DEFAULT_RUN_CYCLE;
}

DurationFanScheduler::~DurationFanScheduler()
{
    
}

void DurationFanScheduler::init()
{
    Particle.subscribe("duration", &DurationFanScheduler::setDuration, this, MY_DEVICES);
    //Particle.subscribe("cycle", &DurationFanScheduler::setCycle, this, MY_DEVICES);

    Particle.variable("duration", _runDuration);
    Particle.variable("cycle", _runCycle);
    Particle.variable("next_run", _nextRun);
}

void DurationFanScheduler::fini()
{
    // we can't unregister our variables...
}

bool DurationFanScheduler::isTimeToRun()
{
    // returns true if it is time to run based on the duration, cycle, and last run time
    // first, figure out which cycle we are in
    int now = Time.now();
    int durationSeconds = 60 * _runDuration;
    int cycleSeconds = 60 * _runCycle;
    _nextRun = Time.format((_lastRun + cycleSeconds), TIME_FORMAT_DEFAULT);
    if(now > (_lastRun + cycleSeconds))  // new cycle
    {
        _lastRun = now;
        return true;
    }
    else
    {
        // fan ALWAYS runs from the start of the cycle
        return now <= (_lastRun + durationSeconds);
    }
}


// call backs for registered events
void DurationFanScheduler::setDuration(const char *event, const char *data)
{
    if(data)
    {
        int value = atoi(data);
        _runDuration = value;
    }
}

void DurationFanScheduler::setCycle(const char *event, const char *data)
{
    if(data)
    {
        int value = atoi(data);
        _runCycle = value;
    }
}
