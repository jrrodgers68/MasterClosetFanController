/*
 * Project MasterClosetFanController
 * Description: Controls the fan in the master bedroom closet
 * Author: John Rodgers
 * Date: 2/11/2018
 */

#include "application.h"
#include <string>
#include "constants.h"
#include "fan.h"
#include "switch.h"
#include "fanscheduler.h"
#include "fanschedulerfactory.h"

#include "ParticleMQTTWrapper.h"

 // globals
Switch lightSwitch(SWITCHPIN);
Fan  fan(RELAYPIN);
FanScheduler* fanScheduler = NULL;

bool gMinRunMode = false;

void callback(const char* topic, uint8_t* payload, unsigned int length)
{
    // do nothing for now
}

 void setup()
 {
     Particle.connect();
     waitFor(Particle.connected, 30000);

     Time.zone(-5);
     Particle.syncTime();

     init("MasterClosetFanController", callback);

     fanScheduler = FanSchedulerFactory::instance()->getScheduler(FanSchedulerFactory::TEMPERATURE);
     fanScheduler->init();

     Publisher::writeLogMessage("main/setup", "completed setup function");
 }


 void loop()
 {
     if (Particle.connected() == false)
     {
         Publisher::writeLogMessage("main/loop", "trying to reconnect to Particle");
         Particle.connect();
         waitFor(Particle.connected, 10000);
     }

     MQTTWrapper::instance()->loop();

     // basic logic - if switch is ON, just run the fan.  If off, run if its the scheduled time - otherwise fan is off
     if(lightSwitch.isOn())
     {
         fan.setFanState(ON);
     }
     else
     {
         int state = fanScheduler->isTimeToRun(fan.getFanState()) ? ON : OFF;

         if(state == OFF)
         {
             if(gMinRunMode == false)
             {
                 // make sure we run every 6 hours
                 if(fan.timeSinceLastRun() >= (6 * 3600))
                 {
                     state = ON;
                     gMinRunMode = true;
                     Publisher::writeLogMessage("main/loop", "starting to min run mode");
                 }
             }
             else
             {
                 // check if time to turn off - at 20 minutes
                 if(fan.timeSinceLastRun() < (20 * 60))
                 {
                     state = ON;
                 }
                 else
                 {
                     gMinRunMode = false;
                     Publisher::writeLogMessage("main/loop", "exiting min run mode");
                 }
             }
         }

         fan.setFanState(state);
     }
 }
