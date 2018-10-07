#ifndef EVENTPUBLISHER_H
#define EVENTPUBLISHER_H

#include "Particle.h"

class EventPublisher
{
public:
    static EventPublisher* instance()
    {
          if(_instance == NULL)
          {
              _instance = new EventPublisher();
          }
          return _instance;
    }

    void init();
    void publishEvent(const char* name, const char* value);

private:
    static EventPublisher* _instance;

    int _discardedEventsCounter = 0;
    int _failedEventsCounter = 0;
    int _lastPublishTime = 0;
    String _lastPublishTimeS;

    EventPublisher();
};

#endif
