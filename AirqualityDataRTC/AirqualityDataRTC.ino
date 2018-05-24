// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_MCP4725.h>

RTC_DS3231 rtc;


Adafruit_MCP4725 dac;
Adafruit_MCP4725 dac2;

const int numberDays = 2;
const int numberHours = 24;


int dailyAQI[4][24] = {

  {1, 99, 1, 99, 1, 99, 1, 99, 1, 99, 1, 99, 1, 99, 1, 99, 1, 99, 1, 99, 1, 99, 1, 99},
  {1, 99, 1, 99, 1, 99, 1, 99, 1, 99, 1, 99, 1, 99, 1, 99, 1, 99, 1, 99, 1, 99, 1, 99},

};


/*int dailyAQI[4][24] = {

  {0, 23, 21, 13, 11, 9, 11, 10, 9, 10, 10, 16, 24, 26, 22, 17, 20, 20, 24, 26, 25, 23, 24, 23},
  {20, 18, 17, 14, 14, 17, 19, 14, 10, 12, 15, 12, 18, 21, 26, 28, 31, 31, 31, 30, 29, 28, 24, 16 },

};*/

int brightness = 0;    // how bright the LED is
int fadeAmount = 1;    // how many points to fade the LED by 2 and delay 20

int brightness2 = 0;    // how bright the LED is
int fadeAmount2 = 1;    // how many points to fade the LED by

int dacMax = 4095;


//DEBUG STUFF
bool debug = true;
const int buttonPin = 2;

int buttonState = 0;



void setup() {

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  // put your setup code here, to run once:
  Serial.begin(9600);

  delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  //DEBUG STUFF
  // initialize the pushbutton pin as an input:
  if (debug) {
    pinMode(buttonPin, INPUT);
    pinMode(13, OUTPUT);
  }

  dac.begin(0x62);
  dac2.begin(0x63);

}

bool hourChanged = true;//start true so ramps up

int d = 0;
int h = 0;

int btnCount = 0;




void loop() {
  // put your main code here, to run repeatedly:
  DateTime now = rtc.now();

  //Fornow chose one seting for odd numbers and one for even numbers;
  if (d != now.day()) {
    d = now.day();
  }

  if ( (d % 2) == 0) {
    d = 0;
  } else {
    d = 1;
  }

  //DEBUG STUFF
  // initialize the pushbutton pin as an input:
  if (debug) {


    buttonState = digitalRead(buttonPin);

    // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH:
    if (buttonState == HIGH) {
      // turn LED on:
      btnCount += 1;
      if (btnCount >= 24) {
        btnCount = 0;
      }
      hourChanged = true;
      digitalWrite(13, HIGH);
    }
    else {
      // turn LED off:
      digitalWrite(13, LOW);
    }
    h = btnCount;
  } else {
    //if not debug look at the current time
    if (h != (now.hour() - 1)) {
      hourChanged = true;
      h = (now.hour() - 1);
    }
  }



  int AQI = dailyAQI[d][h];
  int AQIout = map(AQI, 0, 100, 0, dacMax);

  if (hourChanged) {

    if (brightness < dacMax) {
      //turn fully on before colortemp
      brightness = brightness + fadeAmount2;
      Serial.print("brightness   ");
      Serial.println(brightness);
    } else {
      Serial.print("brightness22   ");
      Serial.println(brightness2);
      //check if new value
      if (brightness2 != AQIout) {
        //ramp from current to new by x increments
        if (brightness2 < AQIout) {
          brightness2 = brightness2 + fadeAmount2;
        } else {
          brightness2 = brightness2 - fadeAmount2;
        }
      } else {
        //if value same close this
        hourChanged = false;
      }
    }//end Dim if
  }

  //Dim
  dac.setVoltage(brightness, false);
  //ColorTemp-
  dac2.setVoltage(brightness2, false);

  delay(30);
}
