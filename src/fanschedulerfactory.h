#ifndef FANSCHEDULERFACTORY_H
#define FANSCHEDULERFACTORY_H

#include "fanscheduler.h"


class FanSchedulerFactory
{
public:
    static FanSchedulerFactory* instance()
    {
        if(_instance == NULL)
        {
            _instance = new FanSchedulerFactory();
        }
        return _instance;
    }

    enum ScheduleType {
        DURATION=0,
        TEMPERATURE
    };

    FanScheduler* getScheduler(ScheduleType st);

private:
    FanSchedulerFactory();

    FanScheduler* _cachedSchedulers[2];

    static FanSchedulerFactory* _instance;
};


#endif
