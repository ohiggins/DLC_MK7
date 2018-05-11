# Daylight Clock MK7
## Todo
* ~~Set alarms to 1 hour intervals to make the trigger math more simple~~
* ~~Remove alarms trigger on boot~~
* ~~Add var for alarm set and return with JSON~~
* A~~dd var for offset of timezone and note about daylight saving.~~
* Get Hardware working on pins
* Map pin assignments
* ~~add brightness control to phases~~
* Send ip to the 7 seg display
* ~~hh and mm need to updated fo the clock to function (7seg)~~


## Version
### 016
* No major revisions
* changed the SSID and password

### 017
* Change alarmset var to 0
* add to alarm set function to change to 1
* Add id's to alarms	
* Uncomment TM1637 Code
* Remove test loop for light on
* Remove set Alarm from the setup.
* Added brightness to alarms 
* Change delay from 10 to 5 seconds after the NTP server has been working

## 018
* Clean commented code up so its more readable.

## 019
* Hardware build

## Known Issues
1. Alarms need to be set each time and are not reccuring. This is possible but the alrms need to be reset after each change.
