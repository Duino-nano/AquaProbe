#include <Arduino.h>
#include <Wire.h>
#include "spiffs_httpd_esp32.h"
#include "measure.h"
#include "spiffs_esp32.h"
#include "spiffs_eeprom_esp32.h"
#include "tankIOT.h"


#define ADDRESS 0x52
#define TEST_BUTTN  34
#define LED_PIN 19


typedef enum{
  eLED_OFF,
  eLED_SETTING,
  eLED_DISCON,
  eLED_ON,
}eLED_MODE;

  uint16_t data_cnt = 0;
  uint16_t distance = 0;
  uint8_t distance_tmp = 0;
  int setingFlag = 0; 
  //再起動までの時間
  unsigned long interval = 60; // unit:sec

  // hw_timer_t * timer = NULL; //timer 初期化
void IRAM_ATTR data_logging();
void setup() 
{
  delay(100);
  WiFi.disconnect(true);
  Serial.begin(115200);
  delay(100);
  pinMode(TEST_BUTTN, INPUT_PULLUP);
  EEPROM.begin(sizeof(eeprom));
  getEEPROM();
    // ディープスリープ時間の設定
  esp_sleep_enable_timer_wakeup(interval*1000000ULL);
  initMeasure();
  
  if(!digitalRead(TEST_BUTTN)){
    web_begin();
    setingFlag = 1;
  }
}

void loop() {
  //センサーデータ取得
  moistureData = readMoisture();
  batteryData = readBATY();
  if(setingFlag){   //セッティングモードの場合はwebアプリを起動
   while(1){
      //センサーデータ取得
      moistureData = readMoisture();
      batteryData = readBATY();
      web_main();
   }
  }else{
    /*通常時はGASへデータをアップロード*/
      sendGasTank(moistureData, batteryData);
      WiFi.disconnect(true);
      Serial.print("moistureData");
      Serial.println(moistureData);

       // スリープに移行
      Serial.println("### DEEP SLEEP START");
      esp_deep_sleep_start();
      delay(1000);
  }
}
