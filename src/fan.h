#ifndef FAN_H
#define FAN_H

#include <time.h>

class Fan
{
public:
    Fan(int relayPin);

    void setFanState(int state);
    int  getFanState();

    //time_t getLastRunStart();
    unsigned long timeSinceLastRun();

private:
    int _relayPin;
    int _lastFanState;
    time_t _lastRunStart;
};



#endif
