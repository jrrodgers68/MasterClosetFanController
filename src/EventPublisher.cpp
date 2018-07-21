#include "EventPublisher.h"

EventPublisher* EventPublisher::_instance = NULL;


EventPublisher::EventPublisher()
{
    // register our variables
    Particle.variable("EP_discards", this->_discardedEventsCounter);
    Particle.variable("EP_failed", this->_failedEventsCounter);
    Particle.variable("EP_lastPub", this->_lastPublishTimeS);
}

void EventPublisher::publishEvent(const char* name, const char* value)
{
    if(Particle.connected() == false)
    {
        Particle.connect();
        waitFor(Particle.connected, 2000);
        if(Particle.connected() == false)
        {
            _failedEventsCounter++;
            return;
        }
    }

    // make sure we aren't spamming the events system - no more than
    //   1 publish per second
    if(Time.now() - _lastPublishTime < 1)
    {
        _discardedEventsCounter++;
        return;
    }

    // all events are published as PRIVATE!!
    if(Particle.publish(name, value, 60, PRIVATE) == true)
    {
        _lastPublishTime = Time.now();
        _lastPublishTimeS = Time.format(_lastPublishTime, TIME_FORMAT_DEFAULT);
    }
    else
    {
        _failedEventsCounter++;
    }
}
