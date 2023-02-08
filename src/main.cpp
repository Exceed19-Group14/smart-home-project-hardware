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
int count = 0;

void click () {
  
  count++;
  
  // Serial.println("Click");
}

void green(){
  if (touchRead(T6) < 25 && count == 0){
    ledcWrite(0, 255);
  } else if (touchRead(T6) < 25 && count == 1){
    ledcWrite(0, 0);
  } else if (touchRead(T6) > 40 && count == 0){
    count = 1;
  } else if (touchRead(T6) > 40 && count == 1){
    count = 0;
  }
}

void yellow(){
  if (touchRead(T9) < 25 && count == 0){
    ledcWrite(1, 255);
  } else if (touchRead(T9) < 25 && count == 1){
    ledcWrite(1, 0);
  } else if (touchRead(T9) > 40 && count == 0){
    count = 1;
  } else if (touchRead(T9) > 40 && count == 1){
    count = 0;
  }
}

void red(){
  if (touchRead(T5) < 25 && count == 0){
    ledcWrite(2, 255);
  } else if (touchRead(T5) < 25 && count == 1){
    ledcWrite(2, 0);
  } else if (touchRead(T5) > 40 && count == 0){
    count = 1;
  } else if (touchRead(T5) > 40 && count == 1){
    count = 0;
  }
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

  touchAttachInterrupt(T6 , green , threshold );
  touchAttachInterrupt(T9 , yellow , threshold );
  touchAttachInterrupt(T5 , red , threshold );

}
void loop() {    
  delay(500);
  Serial.printf("%d %d %d\n", touchRead(T6), touchRead(T9), touchRead(T5));
}