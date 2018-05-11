# Daylight Clock MK7
## Todo
* Set alarms to 1 hour intervals to make the trigger math more simple
* Remove alarms trigger on boot
* Add var for alarm set and return with JSON
* Add var for offset of timezone and note about daylight saving.
* Get Hardware working on pins
* Map pin assignments
* add brightness control to phases


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



## Known Issues
1. Alarms need to be set each time and are not reccuring. This is possible but the alrms need to be reset after each change.
