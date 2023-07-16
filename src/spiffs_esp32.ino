#include "spiffs_esp32.h"
#include "spiffs_eeprom_esp32.h"
#include "measure.h"





const IPAddress ip(192, 168, 6, 1);
const IPAddress subnet(255, 255, 255, 0);
bool settingflg = false;
WebServer server(HTTPD_PORT);
EEPROM_struct eeprom;
String  Startperiod;
String  Endperiod;
String  Startratio;
String  Endratio;
String  Starttimeh;
String  Starttimem;
String  Endtimeh;
String  Endtimem;
String  Level_limit;
int web_end_count = 0;

void web_main();

void web_begin() {
  
  // SPIFFS 初期設定
  SPIFFS.begin();

 

  
  delay(1);
  //wakeconfigset();
  // Wi-Fi接続
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_SSID, WIFI_PWD, WIFI_CHANNEL);
  WiFi.softAPConfig(ip, ip, subnet);
  Serial.print(F("Wi-Fi SSID: "));Serial.println(WIFI_SSID);
  Serial.print(F("Wi-Fi Password: "));Serial.println(WIFI_PWD);
  Serial.print(F("AP IP address: "));Serial.println(WiFi.softAPIP().toString());

  // HTTP 初期設定
  setupHttpd();

}

/********************************************************************************
 * webメイン処理
 * data:セットボタン入力
 * return 0:通常　1:設定用webアプリ起動中
 * ******************************************************************************/
void web_main() {
  server.handleClient();
}

