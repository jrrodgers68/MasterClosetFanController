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


 // globals
//Switch lightSwitch(SWITCHPIN);
Fan  fan(RELAYPIN);
FanScheduler* fanScheduler = NULL;

bool gMinRunMode = false;

bool isDST(int day, int month, int dow)
{
    // inputs are assumed to be 1 based for month & day.  DOW is ZERO based!

    // jan, feb, and dec are out
    if(month < 3 || month > 11)
    {
        return false;
    }

    // april - oct is DST
    if(month > 3 && month < 11)
    {
        return true;
    }

    // in March, we are DST if our previous Sunday was on or after the 8th
    int previousSunday = day - dow;
    if(month == 3)
    {
        return previousSunday >= 8;
    }

    // for November, must be before Sunday to be DST ::  so previous Sunday must be before the 1st
    return previousSunday <= 0;
}

 void setup()
 {
        
    Particle.connect();
    waitFor(Particle.connected, 30000);

    int offset = -5;
    if(isDST(Time.day(), Time.month(), Time.weekday()-1))
    {
        offset = -4;
    }

    Time.zone(offset);
    Particle.syncTime();

    fanScheduler = FanSchedulerFactory::instance()->getScheduler(FanSchedulerFactory::DURATION);
    fanScheduler->init();
 }


 void loop()
 {
    if (Particle.connected() == false)
    {
        Particle.connect();
        waitFor(Particle.connected, 10000);
    }

    // basic logic - if switch is ON, just run the fan.  If off, run if its the scheduled time - otherwise fan is off
    int state = fanScheduler->isTimeToRun(fan.getFanState()) ? ON : OFF;
    if(state == OFF)
    {
        if(gMinRunMode == false)
        {
            // make sure we run every 6 hours
            if(Time.now() - fan.getLastRunStart() >= (6 * 3600))
            {
                state = ON;
                gMinRunMode = true;
            }
        }
        else
        {
            // check if time to turn off - at 20 minutes
            if(Time.now() - fan.getLastRunStart() < (20 * 60))
            {
                state = ON;
            }
            else
            {
                gMinRunMode = false;
            }
        }
    }

    fan.setFanState(state);
 }
