#ifndef SWITCH_H
#define SWITCH_H

class Switch
{
public:
    Switch(int pin);

    bool isOn()
    {
        return getSwitchState();
    }

private:
    int _switchPin;

    // return true is switch is ON,  false otherwise
    bool getSwitchState();
};


#endif
