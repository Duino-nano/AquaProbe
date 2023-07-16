#ifndef __SPIFFS_HTTPD_H__
#define __SPIFFS_HTTPD_H__

#include <WebServer.h>
#include "spiffs_eeprom_esp32.h"

#define HTTPD_PORT       80
#define INDEX_HTML_FILE  "/index.html"

/* HTTPDの初期設定 */
void setupHttpd(void);
/* ルートディレクトリアクセス時のハンドラ */
void setupHttpdRoot(void);
/* 未登録URIアクセス時のハンドラ */
void handleConfirmFile(void);
/*  EEPROM取得Ajaxのハンドラ */
void setupHttpdEeprom(void);
/* SPIFFSからファイルを読み出す */
void handleFileRead(String);


#endif
