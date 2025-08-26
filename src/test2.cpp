//just some state machine planning

#include <Arduino.h>

const int standby_LED = 2; //pin
const int logging_LED = 15;

enum State { //modes the device can operate in
    STANDBY = 0,
    LOGGING = 1
};

State state; //initialize state

void runStandby(void);

void runLogging(void);

void runStandby(){
    digitalWrite(standby_LED, HIGH);
    digitalWrite(logging_LED, LOW);

}

void runLogging(){
    digitalWrite(logging_LED, HIGH);
    digitalWrite(standby_LED, LOW);
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