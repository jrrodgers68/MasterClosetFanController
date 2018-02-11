/*
 * Project MasterClosetFanController
 * Description: Controls the fan in the master bedroom closet
 * Author: John Rodgers
 * Date: 2/11/2018
 */

#include <string>
#include "constants.h"
#include "fan.h"
#include "switch.h"
#include "fanscheduler.h"
#include "fanschedulerfactory.h"


 // globals
Switch lightSwitch(SWITCHPIN);
Fan  fan(RELAYPIN);
FanScheduler* fanScheduler = NULL;


 void setup()
 {
     Particle.connect();
     waitFor(Particle.connected, 30000);

     Time.zone(-5);
     Particle.syncTime();

     fanScheduler = FanSchedulerFactory::instance()->getScheduler(FanSchedulerFactory::DURATION);

     fanScheduler->init();
 }


 void loop()
 {
     if (Particle.connected() == false)
     {
         Particle.connect();
         waitFor(Particle.connected, 30000);
     }

     // basic logic - if switch is ON, just run the fan.  If off, run if its the scheduled time - otherwise fan is off
     if(lightSwitch.isOn())
     {
         fan.setFanState(ON);
     }
     else
     {
         int state = fanScheduler->isTimeToRun() ? ON : OFF;
         fan.setFanState(state);
     }
 }
