#include <Arduino.h>
#include <Wire.h>
#include "spiffs_esp32.h"
#include "spiffs_eeprom_esp32.h"
#include <VL53L1X.h>

VL53L1X sensor;
#define ADDRESS 0x52

#define PROBE_PIN   32
#define BATY_PIN    33
#define BATY_EN     25
#define SENSOR_EN   4

#define PWM_PIN 12
int moistureData = 0;
int batteryData = 0;
unsigned long pwmData = 0;
int readMoisture();
void initMeasure(void);
int readBATY();
float mvAnalogIn(uint8_t PIN, float offset);
//EEPROM_struct eeprom;
void initMeasure(void){
  pinMode(BATY_EN,OUTPUT);
  pinMode(SENSOR_EN,OUTPUT);
}


int readMoisture(){
  long total = 0;
  long data = 0;
  long max = 0;
  long min = 0;
  int i;
  digitalWrite(SENSOR_EN,HIGH);
  delay(100);
  for(i=0; i<10; i++){
    data = analogRead(PROBE_PIN);
    if(i == 0){ //1回目は初期化
      max = data;
      min = data;
    }
    if(data > max){
      max = data;
    }
    if(data < min){
      min = data;
    }
    total += data;
  }
  total -= (min+max);
  total /= 8;
  digitalWrite(SENSOR_EN,LOW);
  // Serial.print("SENSOR:");
  // Serial.println(data);
  return total;
}


int readBATY(){
  long total = 0;
  long data = 0;
  long max = 0;
  long min = 0;
  int i;
  digitalWrite(BATY_EN,HIGH);
  delay(100);
  for(i=0; i<10; i++){
    data = long(mvAnalogIn(BATY_PIN, 0));
    if(i == 0){ //1回目は初期化
      max = data;
      min = data;
    }
    if(data > max){
      max = data;
    }
    if(data < min){
      min = data;
    }
    total += data;
  }
  total -= (min+max);
  total *= 2; //電圧の2分の一で測定をしている為
  total /= 8;
  // Serial.print("SENSOR:");
  // Serial.println(data);
  digitalWrite(BATY_EN,LOW);
  return int(total);
}

float mvAnalogIn(uint8_t PIN, float offset){
    int in0,in3;
    float ad0,ad3;
    
    analogSetPinAttenuation(PIN,ADC_11db);
    in3=analogRead(PIN);
    
    if( in3 > 2599 ){
        ad3 = -1.457583e-7 * (float)in3 * (float)in3
            + 1.510116e-3 * (float)in3
            - 0.680858 + offset;
    }else{
        ad3 = 8.378998e-4 * (float)in3 + 8.158714e-2 + offset;
    }
    // Serial.print("ADC (ATT=3;11dB) = ");
    // Serial.print(ad3,3);
    // Serial.print(" [V], ");
    // Serial.println(in3);
    if( in3 < 200 ){
        analogSetPinAttenuation(PIN,ADC_0db);
        in0=analogRead(PIN);
        ad0 = 2.442116e-4 * (float)in0 + offset;
        // Serial.print("ADC (ATT=0; 0dB) = ");
        // Serial.print(ad0,3);
        // Serial.print(" [V], "); 
        // Serial.println(in0);
        if( in3 >= 100 ){
            ad3 = ad3 * ((float)in3 - 100.) / 100.
                + ad0 * (200. - (float)in3) / 100.;
        }else{
            ad3 = ad0;
        }
    }
    return ad3 * 1000.;
}

