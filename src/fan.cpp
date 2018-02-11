#include "fan.h"
#include "constants.h"
#include "EventPublisher.h"



Fan::Fan(int relayPin) : _relayPin(relayPin), _lastFanState(OFF)
{
    pinMode(_relayPin, OUTPUT);
    digitalWrite(_relayPin, LOW);
}


void Fan::setFanState(int state)
{
    if(state != _lastFanState)
    {
        digitalWrite(_relayPin, state);
        _lastFanState = state;
        EventPublisher::instance()->publishEvent("FAN", state == ON ? "ON" : "OFF");
    }
}
