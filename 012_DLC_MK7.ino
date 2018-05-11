#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
//IPAddress ip(10, 0, 0, 239); //comment out if static //fix this
//IPAddress gateway(10, 0, 0, 138);
//IPAddress subnet(255, 255, 255, 0);
//char ssid[] = "Oliver's iPhone";  //  your network SSID (name)
//char pass[] = "kass474kass474";       // your network password
const char* ssid = "Telstra00F027";
const char* pass =  "yrtqbgr9fann";
int lampState = 0;
int alarmSet = 0;
int alarmHour = 6;
int alarmMin = 0;
int timezone=10; //make 11 for Daylight saving. +10 for sydney etc

unsigned int localPort = 2390;      // local port to listen for UDP packets
/* Don't hardwire the IP address or we won't get the benefits of the pool.
    Lookup the IP address for the host name instead */
//IPAddress timeServer(129, 6, 15, 28); // time.nist.gov NTP server
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;


//Neopixel Setup
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            D6
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      16 //8 if only useing one strip, 16 for 2
#define BRIGHTNESS 255
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//
//Clock Setup
#include "TM1637.h"
//Pins for TM1637
#define CLK 3//8
#define DIO 4//was9
TM1637 tm1637(CLK, DIO);

//RTC Setup
#include <Wire.h>
//#include "RTClib.h"
//RTC_DS3231 rtc;
int hh, mm;
int decPoint = 0;

//Alarms Setup
#include <TimeLib.h>
#include <TimeAlarms.h>
//Pins setup
int resetButton = 12;
int piezoPin = 10;
int buzzerOn = 0;

//Alarm Time, whole our only, 24 hour time.
//int alarmHr = 06;//06= 6am, 21=9pm

//Functions for use in the main sketch

//Chase function, used in partyMode
static void chase(uint32_t c) {
  for (uint16_t i = 0; i < pixels.numPixels() + 4; i++) {
    pixels.setPixelColor(i  , c); // Draw new pixel
    pixels.setPixelColor(i - 4, 0); // Erase pixel a few steps back
    pixels.show();
    delay(25);
  }
}
//Demo to test all the pixels are working. Will loop until the reset button is held down
void partyMode() {
  while (digitalRead(resetButton) == LOW) {
    //set clock
    //we're gunna party like it's;
    //    tm1637.point(POINT_OFF);
    //    tm1637.display(0, 1);
    //    tm1637.display(1, 9);
    //    tm1637.display(2, 9);
    //    tm1637.display(3, 9);
    chase(pixels.Color(255, 0, 0)); // Red
    chase(pixels.Color(0, 255, 0)); // Green
    chase(pixels.Color(0, 0, 255)); // Blue
  }
}
void lightOn() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    //Serial.println(i);
    pixels.show();
  }
  lampState = 1;
  Serial.print("Lamp State: ");
  Serial.println(lampState);
}
void lightOff() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
  lampState = 0;
  Serial.print("Lamp State: ");
  Serial.println(lampState);
}
void setPink() {
  Serial.println("System OK");
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 20, 20));
    pixels.show();
  }
}
void setRed() {
  Serial.println("Set to RED, 60 minutes");
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    pixels.show();
  }
}
void setBlue() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 255)); // Moderately bright green color. //20
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
}
void setGreen() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // Moderately bright green color. //20
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
}
void set1() {//10
  
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 5, 0));
    pixels.show();
  }
}
void set2() {//20
  pixels.setBrightness(100);
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 10, 0));
    pixels.show();
  }
}
void set3() {//30
  pixels.setBrightness(200);
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 128, 0));
    pixels.show();
  }
}
void set4() {//40
  pixels.setBrightness(BRIGHTNESS);
  Serial.println("Wake up");
  buzzerOn = 1;
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    pixels.show();
  }
}
void setClockDisplay() {
  if (decPoint == 0) {
    tm1637.point(POINT_OFF);
    decPoint = 1;
  } else {
    tm1637.point(POINT_ON);
    decPoint = 0;
  }
  tm1637.display(0, hh / 10);  // hour
  tm1637.display(1, hh % 10);
  tm1637.display(2, mm / 10); // minutes
  tm1637.display(3, mm % 10);
  delay(500);
}
unsigned long sendNTPpacket(IPAddress& address) {
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
void getTime() {
  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP);

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);

  int cb = udp.parsePacket();
  if (!cb) {
    Serial.println("no packet yet");
  }
  else {
    Serial.print("packet received, length=");
    Serial.println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);
    //utc offset...
    //set this bit and ntp
    //setTime(8,29,0,1,1,11); // set time to Saturday 8:29:00am Jan 1 2011
    epoch = epoch + (3600 * timezone); //sets the UTC timezone offset

    int cHour = ((epoch  % 86400L) / 3600);
    int cMin = ((epoch  % 3600) / 60);
    int cSec = (epoch % 60);
    setTime(cHour, cMin, cSec, 1, 1, 11);
    //adjusttime

    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if ( ((epoch % 3600) / 60) < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ( (epoch % 60) < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(epoch % 60); // print the second
  }
  
  delay(5000);
}
void handleStatus() {

  String message;
  message += "{ \"lampOn\": ";
  message += lampState;
  message += ", ";
  message += "\"alarmSet\": ";
  message += "1, ";
  message += "\"alarmHour\": ";
  message += alarmHour;
  message += ", ";
  message += "\"alarmMin\": ";
  message += alarmMin;
  message += " }";

  //  String message = "[{\"NAME\":\"";
  //  message += "";
  //  message += "\",";
  //  message += "\"IP\":\"";
  //  message += WiFi.localIP().toString();
  //  message += "\"";
  //  message += "}]";
  //String message = "{ "\"lampOn"\: 0, "\alarmSet": 1, "alarmHour": 22, "alarmMin": 30
  //};
  server.send(200, "text/plain", message );
}
void handleLamp() {
  //im the client.

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(server.arg("plain"));
  root.printTo(Serial);
  int lampStatus = root["lampOn"];
  if (lampStatus == 0) {
    lightOff();
  } else {
    lightOn();
  }

  //Serial.println(lampStatus);

  //  Serial.print("Number of args received: ");
  //  Serial.println(server.args());            //Get number of parameters
  //
  //
  //  for (int i = 0; i < server.args(); i++) {
  //    Serial.print(server.argName(i));
  //    Serial.println(server.arg(i));
  //  }


  //  StaticJsonBuffer<200> jsonBuffer;
  //  JsonObject& root = jsonBuffer.parseObject(server.arg("plain"));
  //  if (!root.success()) {
  //    Serial.println("parseObject() failed");
  //
  //  }
  //  Serial.println(sizeof(root));
  //  int lampStatus = root["lampOn"];
  //  Serial.println(lampStatus);
  //  Serial.println("lampon");

  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
}
void handleSet() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(server.arg("plain"));
  root.printTo(Serial);
  //int lampStatus = root["lampOn"];
  alarmHour = root["alarmHour"];
  alarmMin = root["alarmMin"];
  setAlarm();


  //  Serial.print("Number of args received: ");
  //  Serial.println(server.args());            //Get number of parameters
  //
  //
  //  for (int i = 0; i < server.args(); i++) {
  //    Serial.print(server.argName(i));
  //    Serial.println(server.arg(i));
  //  }

  //
  //
  //  //im the client.
  //  StaticJsonBuffer<200> jsonBuffer;
  //  JsonObject& root = jsonBuffer.parseObject(server.arg("plain"));
  //  if (!root.success()) {
  //    Serial.println("parseObject() failed");
  //
  //  }
  //  Serial.println("SetAlarm");
  //  Serial.println(sizeof(root));

  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
}
void Repeats() {
  Serial.println("15 second timer");
}
void setAlarm() {


  //Alarm.free(id);
  //AlarmID_t aTimer = Alarm.timerRepeat(10, RepeatTest);
  //Alarm.disable(aTimer);
  //Alarm.free(aTimer);
  //give each alarm and id, then disable/free
  //Set the alarms
  Serial.println("Alarms set");//message +=alarmHour;
  //  Alarm.alarmRepeat(alarmHour - 1, 0, 0, setRed); //max of 5 alarms
  //  Alarm.alarmRepeat(alarmHour - 1, 15, 0, set1);
  //  Alarm.alarmRepeat(alarmHour - 1, 30, 00, set2);
  //  Alarm.alarmRepeat(alarmHour - 1, 45, 00, set3);
  //  Alarm.alarmRepeat(alarmHour , 0, 0, set4);

  //final time alarm
  Alarm.alarmOnce(alarmHour - 2 , alarmMin, 0, set2);
  Alarm.alarmOnce(alarmHour - 1 , alarmMin, 0, set3);
  Alarm.alarmOnce(alarmHour , alarmMin, 0, set4);
  Serial.print ("Alarm Set for: ");
  Serial.print(alarmHour);
  Serial.print(":");
  Serial.println(alarmMin);
  alarmSet = 1;
  //test
  //AlarmID_t aTimer4 = Alarm.timerRepeat(15, Repeats);
}
void setup() {

  //?6
  pixels.setBrightness(BRIGHTNESS);
  pixels.begin(); // This initializes the NeoPixel library.
  //pinMode(piezoPin, OUTPUT);
  tm1637.init();
  tm1637.set(5);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());



  //rtc.begin();
  //  // manual adjust
  //  // January 21, 2014 at 3am you would call:
  //  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  //  // automatic adjust, this will set from the timestamp of the sketch compile
  //  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //  DateTime now = rtc.now();
  // setTime(now.hour(), now.minute(), now.second(), now.month(), now.day(), now.year());
  //setTime(8,29,0,1,1,11); // set time to Saturday 8:29:00am Jan 1 2011

  pinMode(resetButton, INPUT);

  //server
  server.on("/status", handleStatus);
  server.on("/lamp", handleLamp);
  server.on("/set", handleSet);
  server.begin(); //Start the server
  //gettime

  //setalarm

  lightOff();
  getTime();
  //setAlarm();
  //Alarm.timerRepeat(15, Repeats);
  //party mode. enter test circuit. Once you have started, press the rest button to revert to noprmal mode
  //partyMode();
  //need to delete the alarms
}
void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println();
}
void printDigits(int digits){
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
void loop() {
  //  DateTime now = rtc.now();
  //  hh = now.hour(), DEC;
  //  mm = now.minute(), DEC;
  //  if (buzzerOn == 1) {
  //    digitalWrite(piezoPin, HIGH);
  //    delay(100);
  //    digitalWrite(piezoPin, LOW);
  //    //flash lights? This is where you add flash code or call a function to do so
  //  } else{
  //    digitalWrite(piezoPin, LOW);
  //  }
  //  //Display Clock
  //  //setClockDisplay();
  //  //Alarm.delay(1);
  //  if (digitalRead(resetButton) == HIGH) {
  //    setBlue();
  //    digitalWrite(piezoPin, HIGH);
  //    delay(100);
  //    buzzerOn = 0;
  //    lightOff();
  //  }
  //getTime();//ntp server
  //Serial.println(WiFi.localIP());
  Alarm.delay(1);
  server.handleClient();
  digitalClockDisplay();
  delay(1000);
  //cxlock test
  //    tm1637.point(POINT_OFF);
  //    tm1637.display(0, 1);
  //    tm1637.display(1, 9);
  //    tm1637.display(2, 9);
  //    tm1637.display(3, 9);
}
