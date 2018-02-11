#include "Particle.h"
#include "fanschedulerfactory.h"
#include "durationfanscheduler.h"
//#include "temperaturefanscheduler.h"

FanSchedulerFactory* FanSchedulerFactory::_instance = NULL;


FanSchedulerFactory::FanSchedulerFactory()
{
    // create 1 of each type of scheduler and store in array
    _cachedSchedulers[0] = new DurationFanScheduler();
    //_cachedSchedulers[1] = new TemperatureFanScheduler();
}

FanScheduler* FanSchedulerFactory::getScheduler(ScheduleType st)
{
    return _cachedSchedulers[st];
}
