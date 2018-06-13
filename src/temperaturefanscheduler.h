#ifndef TEMPERATUREFANSCHEDULER_H
#define TEMPERATUREFANSCHEDULER_H


#include "fanscheduler.h"


class TemperatureFanScheduler : public FanScheduler
{
public:
    TemperatureFanScheduler();
    virtual ~TemperatureFanScheduler();

    virtual void init();
    virtual void fini();
    virtual bool isTimeToRun(int fanState);

private:
    int _lastClosetTemp = 0;
    int _closetTempTimestamp = 0;
    int _lastRoomTemp   = 0;
    int _roomTempTimestamp = 0;

    void temperatureCallback(const char* event, const char* data);
    void validateValues();
};


#endif
