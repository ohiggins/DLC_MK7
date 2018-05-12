# Daylight Clock MK7
## Todo
* ~~Set alarms to 1 hour intervals to make the trigger math more simple~~
* ~~Remove alarms trigger on boot~~
* ~~Add var for alarm set and return with JSON~~
* ~~Add var for offset of timezone and note about daylight saving.~~
* ~~Get Hardware working on pins~~
* ~~Map pin assignments~~
* ~~add brightness control to phases~~
* Send ip to the 7 seg display
* ~~hh and mm need to updated for the clock to function (7seg)~~


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

## 020
* Final Code

## Known Issues
1. Alarms need to be set each time and are not reoccuring. They should be considered a "single shot" This is possible but the alarms need to be reset after each change.
2. On "No Packet yet error" need to output to 7 seg "err"

##Pin assignments
C++ Arduino Code=Markings on Wemos
D8  = D10 piezoPin
D7  = D7  Clk
D6  = D6  DIO
D5  = D5  Input button
D4  = D9  Not Used
D3  = D8  Neo Pixel
