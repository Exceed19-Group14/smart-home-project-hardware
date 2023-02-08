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
int count = 0, check_ldr = 0, ldr_auto = 0;

void Connect_Wifi() {
  const char *ssid = "Illya";
  const char *password = "teen12345";
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("OK! IP=");
  Serial.println(WiFi.localIP());
}

void green(){
  if (ldr_auto == 0){
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
}

void yellow(){
  if (ldr_auto == 0){
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
}

void red(){
  if (ldr_auto == 0){
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
}

void LDR_control_auto_green(){
  check_ldr = (map(analogRead(LDR),2000,4095,0,255));
      if (check_ldr < 150) {
        ledcWrite(0, 255);
      } else {
        ledcWrite(0, 0);
      }
}

void LDR_control_auto_yellow(){
  check_ldr = (map(analogRead(LDR),2000,4095,0,255));
      if (check_ldr < 150) {
        ledcWrite(1, 255);
      } else {
        ledcWrite(1, 0);
      }
}

void LDR_control_auto_red(){
  check_ldr = (map(analogRead(LDR),2000,4095,0,255));
      if (check_ldr < 150) {
        ledcWrite(1, 255);
      } else {
        ledcWrite(1, 0);
      }
}

void LDR_control_auto(){
  LDR_control_auto_green();
  LDR_control_auto_yellow();
  LDR_control_auto_red();
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

  Connect_Wifi();

  touchAttachInterrupt(T6 , green , threshold );
  touchAttachInterrupt(T9 , yellow , threshold );
  touchAttachInterrupt(T5 , red , threshold );

  LDR_control_auto();

}
void loop() {    
  delay(500);
  Serial.printf("%d %d %d\n", touchRead(T6), touchRead(T9), touchRead(T5));
}