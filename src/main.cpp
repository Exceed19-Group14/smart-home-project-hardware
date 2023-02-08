#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wifi.h>
#include <Bounce2.h>
#include <string.h>

#define GREEN 33
#define YELLOW 25
#define RED 26
#define LDR 32
#define BUTTON 27


//const int _size = 2 * JSON_OBJECT_SIZE(20);
//StaticJsonDocument<_size> JSONPost;
//StaticJsonDocument<_size> JSONGet;

Bounce debouncer = Bounce();
const int ledPin =  2;      // the number of the LED pin
const int threshold =  40;      // the number of the LED pin
int ledState = LOW;             // ledState used to set the LED
int l6_val = 0;

void click () {
  debouncer.update();
    if ( debouncer.rose() ) { 
        ledcWrite(0, 255);
    }
  
  // Serial.println("Click");
}

void setup() {
  // put your setup code here, to run once:
  //pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  ledcSetup(0, 5000, 8);
  ledcSetup(1, 5000, 8);
  ledcSetup(2, 5000, 8);
  ledcAttachPin(GREEN, 0);
  ledcAttachPin(YELLOW, 1);
  ledcAttachPin(RED, 2);

  debouncer.attach(T6, INPUT_PULLUP);
  debouncer.interval(25); 
  touchAttachInterrupt(T6 , click , threshold );

}
void loop() {    
  
}