#ifndef FAN_H
#define FAN_H

class Fan
{
public:
    Fan(int relayPin);

    void setFanState(int state);

private:
    int _relayPin;
    int _lastFanState;
};



#endif
