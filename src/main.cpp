// https://forum.arduino.cc/t/rtc-solved-how-to-set-date-and-time-permanent/183180/5

#include <Arduino.h>
#include <DHT.h>
#include <RTClib.h>
#include <Wire.h>
#include <SD.h>

const int DHT_pin = 4;

// #define DHT_type DHT22;

float hum;
float temp;

DHT dht(DHT_pin, DHT22);

RTC_DS3231 rtc; //type of rtc used

void setup(){

    //         //Find the default SPI pins for your board
    //         Serial.begin(115200);
    //         Serial.print("MOSI: ");
    //         Serial.println(MOSI);
    //         Serial.print("MISO: ");
    //         Serial.println(MISO);
    //         Serial.print("SCK: ");
    //         Serial.println(SCK);
    //         Serial.print("SS: ");
    //         Serial.println(SS);  

    pinMode(DHT_pin, INPUT);

    Serial.begin(9600);
    rtc.begin();
    dht.begin();

    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    if (rtc.lostPower()){
        // set to date and time sketch was compiled
        Serial.println("RTC lost power, setting the time");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // TODO: figure out why this doesn't work after battery taken out
        // rtc.adjust from block above currently used
    }
    else {
        Serial.println("RTC running with correct time");
    }
}

void loop(){
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    if (now.second() == 0){
        Serial.println("on the minute");
        // doSomething()
        // log temp
    }

    hum = dht.readHumidity();
    temp = dht.readTemperature();

    Serial.println("humidity: ");
    Serial.print(hum);
    Serial.println("Temp: ");
    Serial.print(temp);

    delay(1000);
}






























































// void setup() {
//     /*
//         //Find the default SPI pins for your board
//         Serial.begin(115200);
//         Serial.print("MOSI: ");
//         Serial.println(MOSI);
//         Serial.print("MISO: ");
//         Serial.println(MISO);
//         Serial.print("SCK: ");
//         Serial.println(SCK);
//         Serial.print("SS: ");
//         Serial.println(SS);  
//     */

//     Serial.begin(115200);
//     // dht.begin();
//     // pinMode(DHT_pin, INPUT);

//     // analogSetAttenuation(ADC_11db);

//     //pinMode(4, INPUT);

//     rtc.begin();

//     if (! rtc.begin()) {
//         Serial.println("Couldn't find RTC");
//         Serial.flush();
//         while (1) delay(10);
//         }

//     if (rtc.lostPower()) {
//         Serial.println("RTC lost power, let's set the time!");
//         // When time needs to be set on a new device, or after a power loss, the
//         // following line sets the RTC to the date & time this sketch was compiled
//         rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//         // This line sets the RTC with an explicit date & time, for example to set
//         // January 21, 2014 at 3am you would call:
//         //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

//         }

//     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

// }

  
// void loop() {

// // Get the current time from the RTC
// DateTime now = rtc.now();

// // Getting each time field in individual variables
// // And adding a leading zero when needed;
// String yearStr = String(now.year(), DEC);
// String monthStr = (now.month() < 10 ? "0" : "") + String(now.month(), DEC);
// String dayStr = (now.day() < 10 ? "0" : "") + String(now.day(), DEC);
// String hourStr = (now.hour() < 10 ? "0" : "") + String(now.hour(), DEC); 
// String minuteStr = (now.minute() < 10 ? "0" : "") + String(now.minute(), DEC);
// String secondStr = (now.second() < 10 ? "0" : "") + String(now.second(), DEC);
// String dayOfWeek = daysOfTheWeek[now.dayOfTheWeek()];

// // Complete time string
// String formattedTime = dayOfWeek + ", " + yearStr + "-" + monthStr + "-" + dayStr + " " + hourStr + ":" + minuteStr + ":" + secondStr;

// // Print the complete formatted time
// Serial.println(formattedTime);

// // Getting temperature
// Serial.print(rtc.getTemperature());
// Serial.println("ºC");

// Serial.println();
// delay(3000);

// }






























































/*
PLAN

button/other method to set time from computer

figure out how to keep time ticking when powered off

otherwise keep previously set timing when powered off using rtc battery

connect device to serial and hit button to set correct time?

temp measurements:
    dht
    rtc temp
    thermistor?

write to sd card temps every x minutes

button/switch for recording mode vs standby

just power on or off probably

switch to change sampling rate??

indicator LED?


            https://arduino.stackexchange.com/questions/93880/ds3231-not-keeping-time-when-arduino-is-turned-off

            #include <EEPROM.h>

            // Assume the flag is stored at address 0
            const int flagAddress = 0;

            void setup() {
            EEPROM.begin(512); // Adjust size as needed
            bool timeSetFlag;
            EEPROM.get(flagAddress, timeSetFlag); // Read the flag

            if (!timeSetFlag) {
                // Time-set command goes here
                timeSetFlag = true; // Update flag
                EEPROM.put(flagAddress, timeSetFlag);
                EEPROM.commit(); // Save changes to EEPROM
            }
            }

            void loop() {
            // Main code
            }

https://lastminuteengineers.com/ds3231-rtc-arduino-tutorial/


*/
