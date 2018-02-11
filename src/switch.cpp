#include "Particle.h"
#include "constants.h"
#include "switch.h"
#include "EventPublisher.h"



Switch::Switch(int pin) : _switchPin(pin)
{
    pinMode(_switchPin, INPUT_PULLUP);
}


bool Switch::getSwitchState()
{
    // bail out limit so we don't get stuck waiting on the debounce
    const long BAIL_OUT = 25000;
    int previousRead = -1;

    unsigned long time = 0;
    unsigned long debounce = 30;

    for(long i = 0; i < BAIL_OUT; ++i)
    {
        int value = digitalRead(_switchPin);
        // if switch changed, either bounce or pressed
        if(value != previousRead)
        {
            time = millis(); // reset debounce timer
        }

        if((millis() - time) > debounce) // we've been stable long enough!
        {
            // we used a pullup resistor, so values are reversed - HIGH is off, LOW is on
            return (value == HIGH ? false : true);
        }

        previousRead = value;
    }

    // SHOULD NEVER GET HERE!!
    EventPublisher::instance()->publishEvent("RUNTIME_ERROR","Invalid switch state - loop exited without value");
    return false;
}
