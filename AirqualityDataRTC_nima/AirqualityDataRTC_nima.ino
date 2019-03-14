/***********************************************************************************
             Air Quality || light animation
             Deep Performance Dwelling, Team MTL, Solar Decathlon
             Topological Media Lab

  looking at air quality index stored in an array, one measure per hour.
  Scrolling through it hourly and gradually going up to new value.
  The sircuit is 2 Dacs connected to an op amp. outputting a 0 to 10 vdc signal
  An RTC to be able to go to the correct data once turned on.


    Author Nima Navab, Thierry Dumont
    Last Edit: June 8, 2018
***********************************************************************************/

#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;
Adafruit_MCP4725 dac2;
//RTC_DS3231 rtc;

//const int numberDays = 2;//number of days in the forecast, should be 31 to match a month
//const int numberHours = 24;

//forecast array
int dailyAQI[4][24] = {

  {0, 35, 0, 35, 0, 35, 11, 10, 9, 10, 10, 16, 24, 26, 22, 17, 20, 20, 24, 26, 25, 23, 24, 23},
  {20, 18, 17, 14, 14, 17, 19, 14, 10, 12, 15, 12, 18, 21, 26, 28, 31, 31, 31, 30, 29, 28, 24, 16 },
  {0, 23, 21, 13, 11, 9, 11, 10, 9, 10, 10, 16, 24, 26, 22, 17, 20, 20, 24, 26, 25, 23, 24, 23},
  {20, 18, 17, 14, 14, 17, 19, 14, 10, 12, 15, 12, 18, 21, 26, 28, 31, 31, 31, 30, 29, 28, 24, 16 }

};

/*example of Toronto AQ
  int dailyAQI[4][24] = {
  {0, 23, 21, 13, 11, 9, 11, 10, 9, 10, 10, 16, 24, 26, 22, 17, 20, 20, 24, 26, 25, 23, 24, 23},
  {20, 18, 17, 14, 14, 17, 19, 14, 10, 12, 15, 12, 18, 21, 26, 28, 31, 31, 31, 30, 29, 28, 24, 16 },
  };*/

int AQIMin = 0;//min value for scaling
int AQIMax = 35;//max value for scaling CHANGE
int dacMax = 4095;//maximum value for DAC Output

int brightness = 4095;    // how bright the LED is
int fadeAmount = 2;    // many steps
int delayTime = 5;//delay for fade in

int brightness2 = 0;    // Color temperature
int fadeAmount2 = 1;    // how many steps



void setup() {
  //Serial.begin(9600);
  dac.begin(0x62);
  dac2.begin(0x63);
  //Dim and colort temp start at 0
  dac2.setVoltage(brightness2, false);
  dac.setVoltage(brightness, false);

  //#ifndef ESP8266
  //  while (!Serial); // for Leonardo/Micro/Zero
  //#endif
  //
  //  if (rtc.lostPower()) {
  //    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //  }
  //
  //
  //}

  //bool hourChanged = true;//start true so ramps up
  //int d = 0;//Day variable
  //int h = 0;//Hour Variable
  //int btnCount = 0;//btn for debug

  void loop() {
    // put your main code here, to run repeatedly:
    //  DateTime now = rtc.now();

    //  //Fornow chose one seting for odd numbers and one for even numbers;
    //  if (d != now.day()) {
    //    d = now.day();
    //  }
    //
    //  //CHANGE THIS IF YOUWANT TO MATCH RTC
    //  if ( (d % 2) == 0) {
    //    d = 0;
    //  } else {
    //    d = 1;
    //  }


    int AQI = dailyAQI[d][h];
    int AQIout = map(AQI, AQIMin, AQIMax, 0, dacMax);

    //  if (brightness < dacMax) {
    //    //turn fully on before colortemp
    //    brightness = brightness + fadeAmount;
    ////    hourChanged = false;
    //  } else
    brightness2 = brightness2 + fadeAmount2;
    if (brightness2 <= 0 || brightness2 >= dacMax) {
      fadeAmount2 = -fadeAmount2;
      delayTime = 30;//once reached Intensity reset delay to 30 mil
      //if not debug look at the current time
      //    if (h != (now.hour() - 1)) {
      //      hourChanged = true;
      //      h = (now.hour() - 1);
      //    }
    }//end Dim if
    //
    //  if (hourChanged) {
    //
    //    //check if new value
    //    if (brightness2 != AQIout) {
    //      //ramp from current to new by x increments
    //      if (brightness2 < AQIout) {
    //        brightness2 = brightness2 + fadeAmount2;
    //      } else {
    //        brightness2 = brightness2 - fadeAmount2;
    //      }
    //    } else {
    //      //if value same close this
    //      hourChanged = false;
    //    }
    //
    //  }

    //Dim
    //dac.setVoltage(brightness, false);
    //ColorTemp-
    dac2.setVoltage(brightness2, false);
    //Serial.println(brightness2);
    delay(80);
  }
