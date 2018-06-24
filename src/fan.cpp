#include "fan.h"
#include "constants.h"
#include "EventPublisher.h"
#include "Publisher.h"
using namespace ParticleMQTTWrapper;


Fan::Fan(int relayPin) : _relayPin(relayPin), _lastFanState(OFF)
{
    pinMode(_relayPin, OUTPUT);
    digitalWrite(_relayPin, LOW);

    _lastRunStart = Time.now();

    Particle.variable("FanLastStart", _lastRunStart);
}


void Fan::setFanState(int state)
{
    if(state != _lastFanState)
    {
        digitalWrite(_relayPin, state);
        _lastFanState = state;
        EventPublisher::instance()->publishEvent("FAN", state == ON ? "ON" : "OFF");
        Publisher::writeLogMessage("fan/setFanState", state == ON ? "ON" : "OFF");
        if(state == ON)
        {
            _lastRunStart = Time.now();
        }
    }
}


int Fan::getFanState()
{
    return _lastFanState;
}


unsigned long Fan::timeSinceLastRun()
{
    return Time.now() - _lastRunStart;
}
