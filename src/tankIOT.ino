#include <WiFi.h>
#include <HTTPClient.h>
#include "spiffs_eeprom_esp32.h"

//------------------------------------------------------------------------
// const char* ssid = "aterm-6922cd-a";                      //TODO SSIDを設定  
// const char* password = "5794229c5110a";     //TODO  パスワードを設定
// const char* devID = "TNK20230527001";
// const int row_pos = 2;    //スプレットシート行指定
// google scriptのscriptID
// const char *GScriptId = "AKfycbxdEZO-7tEn0_qwiihLbv3C2u95sqfhBSToV8EkpfWUjWJNx8-I3C0BoNkjuGcz58oR";           //TODO GASのScriptIdを設定
//------------------------------------------------------------------------
const char* host = "script.google.com"; 
char sw_json[100];         // postするjson
String responseString;
// タンク値をGASへ送信
boolean sendGasTank(int sensor, int baty){
  Serial.println(eeprom.gasData.GScriptId);
  const String sw_published_url = String("/macros/s/") + eeprom.gasData.GScriptId+ "/exec";
  const String published_url = String("https://") + host + sw_published_url;

  //WiFi接続
  if(!connectWifi()){
    return false;
  }
  // float sensor;　//センサー値取得してセット
  sprintf(sw_json, "{\"devID\": \"%s\" , \"sensor\": %d , \"batteryData\": %d}", eeprom.gasData.devID, sensor, baty);
  Serial.println(published_url);
  boolean ret = postRequest(published_url ,sw_json);
  // 後処理
  WiFi.disconnect(true);
  return ret;
}
//WiFi接続
boolean connectWifi(){
  static int connectedCount = 0;
  Serial.println(eeprom.wifi.ssid);
  Serial.println(eeprom.wifi.pass);
  WiFi.begin(eeprom.wifi.ssid, eeprom.wifi.pass);  //  Wi-Fi接続
  while (WiFi.status() != WL_CONNECTED) {  //  Wi-Fi AP接続待ち
      delay(500);
      Serial.print(".");
      connectedCount++;
      if(connectedCount > 30){
        connectedCount = 0;
        WiFi.disconnect(true);
        return false;
      }
  }
  Serial.print("WiFi connected\r\nIP address: ");
  Serial.println(WiFi.localIP());  
  return true;
}
// void setup() {
//   Serial.begin(115200);
//   delay(1000);
//   Serial.println("Starting...");
// }
// void loop() {
//   sendGasTank();
//   delay(300000);//30sec
// }
boolean getRequest(String url){
  HTTPClient http;
  boolean isSuccess = false;
  // configure traged server and url
  http.begin(url); //HTTP
  Serial.print("[HTTP GET] begin...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();
  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP GET] Return... code: %d\n", httpCode);
    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      // 200
      Serial.println("[HTTP GET] Success!!");
      responseString = http.getString();
      Serial.println(responseString);
      isSuccess = true;       
    }
  } else {
    Serial.printf("[HTTP GET] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();  
  return isSuccess;  
}
boolean postRequest(String url, String json){
  //HTTPClient code start
  HTTPClient http;
  boolean isSuccess = false;
  Serial.println(json);
  Serial.print("[HTTP POST] begin...\n");
  if (!http.begin(url)){
    Serial.print("[HTTP POST] http.begin() failed \n");
    return false;
  }
  // Locationをとるためにこれを書かないといけない
  const char* headerNames[] = { "Location"};
  http.collectHeaders(headerNames, sizeof(headerNames)/sizeof(headerNames[0]));
  Serial.print("[HTTP POST] ...\n");
  // start connection and send HTTP header
  int httpCode = http.POST(json);
  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP POST] Return... code: %d\n", httpCode);     
    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      // 200
      Serial.println("[HTTP] Success!!");
      String payload = http.getString();
      Serial.println(payload);
      isSuccess = true;
    }else if (httpCode == HTTP_CODE_FOUND ) {//HTTP_CODE_FOUND    
      // 302 … ページからreturnが戻った場合はリダイレクトとなりこのエラーコードとなる
      String payload = http.getString();
      Serial.println(payload);
      // ヘッダのLocation（リダイレクト先URL）を取り出す
      Serial.println("Location");
      Serial.println(http.header("Location"));
      // リダイレクト先にGetリクエスト
      isSuccess = getRequest(http.header("Location"));
    }
  } else {
    Serial.printf("[HTTP POST] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
  return isSuccess;
}
