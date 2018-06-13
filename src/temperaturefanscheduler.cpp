#include "Particle.h"
#include "temperaturefanscheduler.h"
#include "constants.h"


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

void TemperatureFanScheduler::validateValues()
{
    // if timestamp is older than 30 minutes, zero out the temps
    int now = Time.now();
    int validTimeWindow = 30 * 60;
    if((now - _roomTempTimestamp) > validTimeWindow)
    {
        _lastRoomTemp = 0;
    }

    if((now - _closetTempTimestamp) > validTimeWindow)
    {
        _lastClosetTemp = 0;
    }
}

bool TemperatureFanScheduler::isTimeToRun(int fanState)
{
    validateValues();

    if((_lastRoomTemp == 0) || (_lastClosetTemp == 0))
    {
        return false;
    }

    int comparitor = (fanState == ON ? 2 : 4);

    // basic idea - compare our temps, if closet is off by more than 2 degrees, run fan
    return (abs(_lastClosetTemp - _lastRoomTemp) > comparitor);
}

void TemperatureFanScheduler::temperatureCallback(const char* event, const char* data)
{
    if(data)
    {
        String dataStr(data);
        int value  = (int)(dataStr.toFloat() + 0.5f);
        String closetTemp(CLOSET_TEMP);
        String roomTemp(ROOM_TEMP);
        if(closetTemp.compareTo(event) == 0)
        {
            _lastClosetTemp = value;
            _closetTempTimestamp = Time.now();
        }
        else if(roomTemp.compareTo(event) == 0)
        {
            _lastRoomTemp = value;
            _roomTempTimestamp = Time.now();
        }
    }
}
