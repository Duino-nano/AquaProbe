#ifndef __SPIFFS_EEPROM_H__
#define __SPIFFS_EEPROM_H__

#include <EEPROM.h>

#define DEFAULT_GENDER    0
#define DEFAULT_USERNAME  "hoge"

typedef struct{
  char ssid[200];
  char pass[200];
}WIFI;

typedef struct{
  char devID[200];
  char GScriptId[500]; 
}GASDATA;

typedef struct {
  WIFI    wifi;
  GASDATA gasData;
} EEPROM_struct;

/* EEPROMから一括して読み出す */
void getEEPROM(void);
/* EEPROMに一括して書き出す */
void setEEPROM(void);
/* EEPROMデータをJSON形式で返す */
String getEepromJson(void);

extern EEPROM_struct eeprom;

#endif
