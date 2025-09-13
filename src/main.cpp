// https://forum.arduino.cc/t/rtc-solved-how-to-set-date-and-time-permanent/183180/5

#include <Arduino.h>
#include <DHT.h>
#include <RTClib.h>
#include <Wire.h>
#include <SD.h>
#include <WiFi.h>
#include "WifiPass.h" //little library with my home wifi password;

const int DHT_pin = 4;
const int SD_SDI_pin = 5;

File myFile;

float hum;
float temp;
bool logging = false;

//WiFi stuff https://randomnerdtutorials.com/esp32-web-server-arduino-ide/
WiFiServer server(80);
String header;
String output26State = "off";
unsigned long current_time = millis();
unsigned long previous_time = 0;
const long timeout_time = 2000;

const char* PARAM_INPUT_1 = "input1";

const int output26 = 26;

DHT dht(DHT_pin, DHT22);

RTC_DS3231 rtc; //type of rtc used

void wifiConnect(void);

void setup(){
    Serial.begin(115200);

    //Find the default SPI pins for the board:
    Serial.print("MOSI: ");
    Serial.println(MOSI);
    Serial.print("MISO: ");
    Serial.println(MISO);
    Serial.print("SCK: ");
    Serial.println(SCK);
    Serial.print("SS: ");
    Serial.println(SS);  

    pinMode(DHT_pin, INPUT);
    pinMode(output26, OUTPUT);

    Serial.println("connecting to wifi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.print("SSID: ");
    Serial.println(ssid);

    while (WiFi.waitForConnectResult() != WL_CONNECTED){
        Serial.println("WiFi Failed");
        return;
    }

    Serial.print("ESP IP: ");
    Serial.println(WiFi.localIP()); //ESP32 IP

    Serial.println("WiFi connected");

    server.begin();

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

    hum = dht.readHumidity();
    temp = dht.readTemperature(1);

    Serial.print("humidity: ");
    Serial.println(hum);
    Serial.print("Temp: ");
    Serial.println(temp);

    if (now.second() == 0){
        Serial.println("on the minute");

        myFile = SD.open("/esp32.txt", FILE_APPEND); //needs to be APPEND not WRITE or it wont work

        if (myFile) {
          Serial.println("WRITING INTO FILE");
          myFile.print("time: "); //write in the time
          myFile.print(now.year(), DEC);
          myFile.print('/');
          myFile.print(now.month(), DEC);
          myFile.print('/');
          myFile.print(now.day(), DEC);
          myFile.print(" ");
          myFile.print(now.hour(), DEC);
          myFile.print(':');
          myFile.print(now.minute(), DEC); //no need to write seconds, always 0

          myFile.print(", humidity: "); // write a line to esp32.txt
          myFile.print(hum); // write another line to esp32.txt
          myFile.print(", temp(F): ");
          myFile.println(temp); //newline after this
          myFile.close();
        } 
        else {
          Serial.print(F("SD Card: Issue encountered while attempting to open the file esp32.txt"));
        }
    }

    wifiConnect();

    Serial.println(logging);

    if (logging == true){
        Serial.println("temperature logging turned on");
        // code to write to file in here, turned on by on-board button or server
    }

    delay(1000);
}

void wifiConnect(){


  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    current_time = millis();
    previous_time = current_time;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && current_time - previous_time <= timeout_time) {  // loop while the client's connected
      current_time = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              logging = true;
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              logging = false;
              digitalWrite(output26, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  Serial.println(output26State);

}