- ---- SPENTO ----
- button to set timer
- button to set target T
- button to switch on display and stop heating (holding the button)

- HW: relay, PSU, rotary encoders

- Rotary encoders: to replace buttons (set timer and target T)

- Bluetooth: communication between 2 Arduinos

- Use PID algo in heating: https://playground.arduino.cc/Code/PIDLibrary/
  Or it is useless as the time it takes to reach the T is slow

- list-utils::getByIndex to set an IndexError
  using exceptions defined here: https://github.com/puntonim/arduino-monorepo/blob/790c425cb40c56209f74f5c541a8fc9b81dbcfd8/THERMOSTAT/thermostat-led-display/common-exceptions.h
  See notes in: /Volumes/home/Drive/DOCUMENTI-SYNC/IT/\_SW,\ SYS\ ENGINEERING/C++/Exceptions.md

- What to do in case of (local) sensor error?
  Maybe, in that error case, we should consider only the timer!
  But do we switch ON until the timer is over then switch OFF?
  or do we keep the current status (ON or OFF) until the timer is over and then
  switch OFF?

- What to do in case of remote sensor error?
  If we use a remote (Bluetooth) probe for the T, then the Bluetooth might fail
  or the sensor might fail. WHat to do?
  Do we also add an internal temp sensor?
  Or do we use the same logic as a local sensor error?

- Cloud and web:
  - send data to IoT cloud or custom BE to track:
    - T and H over time
    - heating ON/OFF
      These data could collected (in memory) and sent to the remote every 5 mins
      Or they could be collected every 1 min, stored in a SD card and sent to
      the remote every 5 mins
  - Hmmm: receive data from remote (IoT cloud? regular HTTP reqs?)
    But I don't rellay need the feature to switch ON/OFF the heating from
    remote
