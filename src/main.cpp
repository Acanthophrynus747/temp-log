// https://forum.arduino.cc/t/rtc-solved-how-to-set-date-and-time-permanent/183180/5

#include <Arduino.h>
#include <DHT.h>
#include <RTClib.h>
#include <Wire.h>
#include <SD.h>

const int DHT_pin = 4;
const int SD_SDI_pin = 5;

File myFile;

// #define DHT_type DHT22;

float hum;
float temp;

DHT dht(DHT_pin, DHT22);

RTC_DS3231 rtc; //type of rtc used

void setup(){

    //Find the default SPI pins for your board
    Serial.begin(9600);

    Serial.print("MOSI: ");
    Serial.println(MOSI);
    Serial.print("MISO: ");
    Serial.println(MISO);
    Serial.print("SCK: ");
    Serial.println(SCK);
    Serial.print("SS: ");
    Serial.println(SS);  

    pinMode(DHT_pin, INPUT);

    rtc.begin();
    dht.begin();
    //SD.begin(SD_SDI_pin);

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

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

    if (!SD.begin(SD_SDI_pin)){
        while (1){
            Serial.println(F("SD CARD FAILED, OR NOT PRESENT!"));
            delay(1000);
        }
    }
    
    Serial.println(F("SD CARD INITIALIZED."));

    
    if (!SD.exists("/esp32.txt")) {
        Serial.println(F("esp32.txt doesn't exist. Creating esp32.txt file..."));
        // create a new file by opening a new file and immediately close it
        myFile = SD.open("/esp32.txt", FILE_WRITE);
        myFile.close();
    }

    // recheck if file is created or not
    if (SD.exists("/esp32.txt"))
        Serial.println(F("esp32.txt exists on SD Card."));
    else
        Serial.println(F("esp32.txt doesn't exist on SD Card."));

    
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
    temp = dht.readTemperature(1);

    Serial.print("humidity: ");
    Serial.println(hum);
    Serial.print("Temp: ");
    Serial.println(temp);

    myFile = SD.open("/esp32.txt", FILE_WRITE);

    if (myFile) {
        myFile.print("time: "); //write in the time
        myFile.print(now.year(), DEC);
        myFile.print('/');
        myFile.print(now.month(), DEC);
        myFile.print('/');
        myFile.print(now.day(), DEC);
        myFile.print(" ");
        myFile.print(now.hour(), DEC);
        myFile.print(':');
        myFile.print(now.minute(), DEC);
        myFile.print(':');
        myFile.print(now.second(), DEC);

        myFile.print(", humidity: "); // write a line to esp32.txt
        myFile.print(hum); // write another line to esp32.txt
        myFile.print(", temp(F): ");
        myFile.println(temp); //newline after this
        myFile.close();
    } else {
        Serial.print(F("SD Card: Issue encountered while attempting to open the file esp32.txt"));
    }

    delay(1000);
}







































// void setup() {

//     // analogSetAttenuation(ADC_11db);


//     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

// }

  
// void loop() {


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