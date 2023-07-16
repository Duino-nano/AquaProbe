#include "spiffs_eeprom_esp32.h"
#include "spiffs_esp32.h"
/**
 * EEPROMから一括して読み出す
 */
void getEEPROM(void) {
  EEPROM.get(0, eeprom);
}

/**
 * EEPROMに一括して書き出す
 */
void setEEPROM(void) {
  EEPROM.put(0, eeprom);
  EEPROM.commit();
  //wakeconfigset();
  Serial.println("EEPROM write!!");

  getEEPROM();//EEPROMへ保存した値を読み込む
}

/**
 * EEPROMデータをJSON形式で返す
 */
String getEepromJson(void) {
  
  // EEPROMから値を読み出す
 getEEPROM();
  
  String gender = "";
  String username = "";

//  if(eeprom.person.gender != 0 && eeprom.person.gender != 1) {

    // EEPROM初期状態ではデフォルト値を設定
    gender = String(DEFAULT_GENDER);
    username = String(DEFAULT_USERNAME);

//  } else {

    // EEPROMの値を設定
    //gender = String(eeprom.confs.gender);
    //username = String(eeprom.confs.username);

 // }

  // EEPROMの値をJSON形式にする
  String result = "{";
  result += "\"person\":";
  result += "{";
  result += "\"gender\":" + gender;
  result += ",";
  result += "\"username\":\"" + username + "\"";
  result += "}";
  result += "}";

  return result;
  
}
