#ifndef PARTICLEEVENTPUBLISHER_H
#define PARTICLEEVENTPUBLISHER_H

#include "Particle.h"

class ParticleEventPublisher
{
public:

    static ParticleEventPublisher* instance()
    {
          if(_instance == NULL)
          {
              _instance = new ParticleEventPublisher();
          }
          return _instance;
    }

    void publishEvent(const char* name, const char* value);

private:
    ParticleEventPublisher();
    ~ParticleEventPublisher();

    static ParticleEventPublisher* _instance;
};


#endif // PARTICLEEVENTPUBLISHER_H
