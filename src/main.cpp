#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Bounce2.h>
#include <string.h>

#define GREEN 33
#define YELLOW 25
#define RED 26
#define LDR 32
#define BUTTON 27

TaskHandle_t TaskA = NULL;
TaskHandle_t TaskB = NULL;

//const int _size = 2 * JSON_OBJECT_SIZE(20);
//StaticJsonDocument<_size> JSONPost;
//StaticJsonDocument<_size> JSONGet;

Bounce debouncer = Bounce();
const int ledPin =  2;      // the number of the LED pin
const int threshold =  40;      // the number of the LED pin
int ledState = LOW;             // ledState used to set the LED
int count = 0, check_ldr = 0, ldr_auto = 0;

void Connect_Wifi() {
  const char *ssid = "group15";
  const char *password = "thisisapassword";
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("OK! IP=");
  Serial.println(WiFi.localIP());
}

int state = 0;
int my_mode = 0;
int bright_level = 0;
int led_num = 0;
int bright_want = 0;
int CountGreen = 0, CountYellow = 0, CountRed = 0;
int ModeGreen = 0, ModeYellow = 0, ModeRed = 0;
int brightGreen = 0, brightYellow = 0, brightRed = 0;
const String BaseUrl = "http://group14.exceed19.online/room/";

void ChangeLight(){
  if (CountGreen == 0)
    ledcWrite(0, 0);
  else if (CountGreen == 1)
    ledcWrite(0, brightGreen);
  else if (CountYellow == 0)
    ledcWrite(0, 0);
  else if (CountYellow == 1)
    ledcWrite(0, brightYellow);
  else if (CountRed == 0)
    ledcWrite(0, 0);
  else if (CountRed == 1)
    ledcWrite(0, brightRed);
}

void GET_data(void *param) {
  while(1){
  DynamicJsonDocument JSONGet(2048);
  HTTPClient http;
  http.begin(BaseUrl);
  int httpResponseCode = http.GET();
  if (httpResponseCode >= 200 && httpResponseCode < 300) {
    Serial.print("===================\n");
    String payload = http.getString();
    deserializeJson(JSONGet, payload);

    CountGreen = JSONGet[0]["state"].as<int>();
    CountYellow = JSONGet[1]["state"].as<int>();
    CountRed = JSONGet[2]["state"].as<int>();
    ModeGreen = JSONGet[0]["mode"].as<int>();
    ModeYellow = JSONGet[1]["mode"].as<int>();
    ModeRed = JSONGet[2]["mode"].as<int>();
    brightGreen = JSONGet[0]["brightness_level"].as<int>();
    brightYellow = JSONGet[1]["brightness_level"].as<int>();
    brightRed = JSONGet[2]["brightness_level"].as<int>();

    Serial.printf("C1: %d %d %d\n", CountGreen, ModeGreen, brightGreen);
    Serial.printf("C2: %d %d %d\n", CountYellow, ModeYellow, brightYellow);
    Serial.printf("C3: %d %d %d\n", CountRed, ModeRed, brightRed);
    // ChangeLight();
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  } 
  http.end();
}
}

void green(){
  if (ModeGreen == 0){
    if (touchRead(T6) < 25 && CountGreen == 0){
      ledcWrite(0, brightGreen);
    } else if (touchRead(T6) < 25 && CountGreen == 1){
      ledcWrite(0, 0);
    } else if (touchRead(T6) > 40 && CountGreen == 0){
      CountGreen = 1;
    } else if (touchRead(T6) > 40 && CountGreen == 1){
      CountGreen = 0;
    }
  }
}

void yellow(){
  if (ModeYellow == 0){
    if (touchRead(T9) < 25 && CountYellow == 0){
      ledcWrite(1, brightYellow);
    } else if (touchRead(T9) < 25 && CountYellow == 1){
      ledcWrite(1, 0);
    } else if (touchRead(T9) > 40 && CountYellow == 0){
      CountYellow = 1;
    } else if (touchRead(T9) > 40 && CountYellow == 1){
      CountYellow = 0;
    }
  }
}

void red(){
  if (ModeRed == 0){
    if (touchRead(T5) < 25 && CountRed == 0){
      ledcWrite(2, brightRed);
    } else if (touchRead(T5) < 25 && CountRed == 1){
      ledcWrite(2, 0);
    } else if (touchRead(T5) > 40 && CountRed == 0){
      CountRed = 1;
    } else if (touchRead(T5) > 40 && CountRed == 1){
      CountRed = 0;
    }
  }
}

void LDR_control_auto_green(){
  if (CountGreen == 0){
  check_ldr = (map(analogRead(LDR),2000,4095,0,255));
      if (check_ldr < 150) {
        ledcWrite(0, brightGreen);
      }
  }
}

void LDR_control_auto_yellow(){
  if (CountYellow == 0){
  check_ldr = (map(analogRead(LDR),2000,4095,0,255));
      if (check_ldr < 150) {
        ledcWrite(1, brightYellow);
      }
  }
}

void LDR_control_auto_red(){
  if (CountRed == 1){
  check_ldr = (map(analogRead(LDR),2000,4095,0,255));
      if (check_ldr < 150) {
        ledcWrite(0, brightRed);
        // func force open
      }
  }
}

void LDR_control_auto(void *param){
  while(1){
    if (ModeGreen == 1)
      LDR_control_auto_green();
    else if (ModeYellow == 1)
      LDR_control_auto_yellow();
    else if (ModeRed == 1)
      LDR_control_auto_red();
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

  Connect_Wifi();

  touchAttachInterrupt(T6 , green , threshold );
  touchAttachInterrupt(T9 , yellow , threshold );
  touchAttachInterrupt(T5 , red , threshold );


  xTaskCreatePinnedToCore(GET_data, "GET_data", 10000, NULL, 1, &TaskA, 0);
  // xTaskCreatePinnedToCore(LDR_control_auto, "LDR_control_auto", 10000, NULL, 1, &TaskB, 1);

}
void loop() {    

}