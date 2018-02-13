#include "Particle.h"
#include "temperaturefanscheduler.h"

#define CLOSET_TEMP   "MC1TEMP"
#define ROOM_TEMP     "SITRMTEMP"

TemperatureFanScheduler::TemperatureFanScheduler()
{

}

TemperatureFanScheduler::~TemperatureFanScheduler()
{

}

void TemperatureFanScheduler::init()
{
    Particle.subscribe(CLOSET_TEMP, &TemperatureFanScheduler::temperatureCallback, this, MY_DEVICES);
    Particle.subscribe(ROOM_TEMP, &TemperatureFanScheduler::temperatureCallback, this, MY_DEVICES);

    Particle.variable(CLOSET_TEMP, _lastClosetTemp);
    Particle.variable(ROOM_TEMP, _lastRoomTemp);
}

void TemperatureFanScheduler::fini()
{
    // nothing to do here
}

bool TemperatureFanScheduler::isTimeToRun()
{
    // basic idea - compare our temps, if closet is off by more than 2 degrees, run fan
    return (abs(_lastClosetTemp - _lastRoomTemp) > 2);
}

void TemperatureFanScheduler::temperatureCallback(const char* event, const char* data)
{
    if(data)
    {
        int value = atoi(data);
        String closetTemp(CLOSET_TEMP);
        String roomTemp(ROOM_TEMP);
        if(closetTemp.compareTo(event) == 0)
        {
            _lastClosetTemp = value;
        }
        else if(roomTemp.compareTo(event) == 0)
        {
            _lastRoomTemp = value;
        }
    }
}