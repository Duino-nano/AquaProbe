#ifndef __SPIFFS_H__
#define __SPIFFS_H__

#include "spiffs_eeprom_esp32.h"
#include "spiffs_httpd_esp32.h"
#include <SPIFFS.h>
#include <WiFi.h>


#define WIFI_SSID      "AquaProbeSeting"
#define WIFI_PWD       "123456789"
#define WIFI_CHANNEL   1

extern EEPROM_struct eeprom;
extern String  Startperiod;
extern String  Endperiod;
extern String  Startratio;
extern String  Endratio;
extern String  Starttimeh;
extern String  Starttimem;
extern String  Endtimeh;
extern String  Endtimem;
extern String  Level_limit;
extern int web_end_count;
extern void web_begin();
extern void web_main();
//#define SWPIN 32
#endif
