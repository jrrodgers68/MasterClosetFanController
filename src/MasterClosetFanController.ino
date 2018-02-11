/*
 * Project MasterClosetFanController
 * Description: Controls the fan in the master bedroom closet
 * Author: John Rodgers
 * Date: 2/11/2018
 */

 #include <string>

 // pins for switch and relay
 #define SWITCHPIN D5
 #define RELAYPIN  D3

 #define DEFAULT_RUN_DURATION 5
 #define DEFAULT_RUN_CYCLE    30

 #define OFF  0
 #define ON   1


 class Fan
 {
 public:
     Fan(int relayPin) : _relayPin(relayPin), _lastFanState(OFF)
     {
         pinMode(_relayPin, OUTPUT);
         digitalWrite(_relayPin, LOW);
     }

     void setFanState(int state)
     {
         if(state != _lastFanState)
         {
             digitalWrite(_relayPin, state);
             _lastFanState = state;
             publishEvent("FAN", state == ON ? "ON" : "OFF");
         }
     }

 private:
     int _relayPin;
     int _lastFanState;
 };

 class FanScheduler
 {
     int _lastRun = 0;
     int _runDuration; // in minutes, amount of time to run the fan
     int _runCycle; // in minutes, time block - fan runs for _runDuration minutes within a _runCycle block (e.g. runDuartion 5 mins, runCycle 30 mins)

     public:
       FanScheduler(int duration, int cycle): _runDuration(duration), _runCycle(cycle)
       {

       }

       void init()
       {
           Particle.variable("duration", _runDuration);
           Particle.variable("cycle", _runCycle);
       }

       void setDuration(int value)
       {
           _runDuration = value;
           String msg = "Duration changed to ";
           msg = msg + String(value);
           publishEvent("FAN_SCHEDULE", msg);
       }

       void setCycle(int value)
       {
           _runCycle = value;
           String msg = "Cycle changed to ";
           msg = msg + String(value);
           publishEvent("FAN_SCHEDULE", msg);
       }

       bool isTimeToRun()
       {
           // returns true if it is time to run based on the duration, cycle, and last run time
           // first, figure out which cycle we are in
           int now = Time.now();
           int durationSeconds = 60 * _runDuration;
           int cycleSeconds = 60 * _runCycle;
           if(now > (_lastRun + cycleSeconds))  // new cycle
           {
               _lastRun = now;
               return true;
           }
           else
           {
               // fan ALWAYS runs from the start of the cycle
               return now <= (_lastRun + durationSeconds);
           }
       }

       int getNextScheduledRun()
       {
           // adjust to local time!
           //int adjustment = Time.now() - Time.local();
           return _lastRun + (60 * _runCycle); // - adjustment;
       }
 };

 class Switch
 {
 public:
     Switch(int pin) : _switchPin(pin)
     {
         pinMode(_switchPin, INPUT_PULLUP);
     }

     bool isOn()
     {
         return getSwitchState();
     }

 private:
     int _switchPin;

     // return true is switch is ON,  false otherwise
     bool getSwitchState()
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
                 if(value == HIGH)
                 {
                     return false;
                 }
                 else
                 {
                     return true;
                 }
             }

             previousRead = value;
         }

         // SHOULD NEVER GET HERE!!
         publishEvent("RUNTIME_ERROR","Invalid switch state - loop exited without value");
         return false;
     }
 };


 // globals
 FanScheduler fs(DEFAULT_RUN_DURATION, DEFAULT_RUN_CYCLE);
 Switch lightSwitch(SWITCHPIN);
 Fan  fan(RELAYPIN);

 int  nextScheduledRun = 0;
 String nextRunString;


 // subscribed events callback function
 void subscribedEventsHandler(const char *event, const char *data)
 {
     // we currently handle 2 events - DURATION and CYCLE
     String duration("duration");
     String cycle("cycle");
     if(duration.compareTo(event) == 0)
     {
         int value = atoi(data);
         fs.setDuration(value);
     }
     else if(cycle.compareTo(event) == 0)
     {
         int value = atoi(data);
         fs.setCycle(value);
     }
     else
     {
         publishEvent("RUNTIME_ERROR", "Received unknown event - unable to process/ignoring");
     }
 }


 void setup()
 {
     pinMode(D7, OUTPUT);
     digitalWrite(D7, LOW); // turn off LED

     Particle.connect();
     waitFor(Particle.connected, 30000);

     Time.zone(-5);
     Particle.syncTime();

     Particle.subscribe("duration", subscribedEventsHandler);
     Particle.subscribe("cycle", subscribedEventsHandler);

     fs.init();

     Particle.variable("next_run", nextRunString);
 }

 String formatNextRunTime(int nextRunTime)
 {
     //char       buf[80];

     //time_t nextRun = nextRunTime;
     //struct tm *ts = localtime(&nextRun);
     //strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", ts);
     return Time.format(nextRunTime, TIME_FORMAT_DEFAULT);
     //return String(buf);
 }

 void publishNextRun(String nextRunTime)
 {
     publishEvent("Next_Run", nextRunTime);
 }

 void publishEvent(const char* name, const char* value)
 {
     if(Particle.connected() == false)
     {
         Particle.connect();
         waitFor(Particle.connected, 30000);
     }

     bool status = Particle.publish(name, value, 60, PRIVATE);
     if(status == false)
     {
         digitalWrite(D7, HIGH);
     }
     else
     {
         digitalWrite(D7, LOW);
     }
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
         int state = fs.isTimeToRun() ? ON : OFF;
         fan.setFanState(state);
     }

     // publish next scheduled runs whenever it changes
     int nr = fs.getNextScheduledRun();
     if(nextScheduledRun != nr)
     {
         nextRunString = formatNextRunTime(nr);
         publishNextRun(nextRunString);
         nextScheduledRun = nr;
     }
 }
