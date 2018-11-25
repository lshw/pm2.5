#ifndef __WIFI_CLIENT_H__
#define __WIFI_CLIENT_H__
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//#include <WiFiClientSecure.h>
#include <ESP8266WiFiMulti.h>
void AP();
ESP8266WiFiMulti WiFiMulti;
HTTPClient http;
File fp;
bool wifi_connect() {
  uint32_t i;
  WiFi.mode(WIFI_STA);
  delay(10);

  char ch;
  String ssid = "";
  String passwd = "";

  boolean is_ssid = true;
  if (SPIFFS.begin()) {
    fp = SPIFFS.open("/ssid.txt", "r");
    if (!fp) {
      AP();
      proc=AP_MODE;
      return true;
    }
    Serial.print("载入wifi设置文件:/ssid.txt ");
    if (fp) {
      uint16_t Fsize = fp.size();
      Serial.print(Fsize);
      Serial.println("字节");
      for (i = 0; i < Fsize; i++) {
        ch = fp.read();
        switch (ch) {
          case 0xd:
          case 0xa:
            if (ssid != "") {
              Serial.print("Ssid:"); Serial.println(ssid);
              Serial.print("Passwd:"); Serial.println(passwd);
              WiFiMulti.addAP(ssid.c_str(), passwd.c_str());
            }
            is_ssid = true;
            ssid = "";
            passwd = "";
            break;
          case ' ':
          case ':':
            is_ssid = false;
            break;
          default:
            if (is_ssid)
              ssid = ssid + String(ch);
            else
              passwd = passwd + String(ch);
        }
      }
      if (ssid != "" && passwd != "") {
        Serial.print("ssid:"); Serial.println(ssid);
        Serial.print("passwd:"); Serial.println(passwd);
        WiFiMulti.addAP(ssid.c_str(), passwd.c_str());
      }
    }
  }
  Serial.println("正在连接wifi.");
  // ... Give ESP 10 seconds to connect to station.
  unsigned long startTime = millis();
  i = 0;
  while (WiFiMulti.run() != WL_CONNECTED && millis() - startTime < 40000)
  {
    Serial.write('.');
    //Serial.print(WiFi.status());
    delay(1000);
  }
  Serial.println();
  if (WiFiMulti.run() == WL_CONNECTED)
  {
    nvram.proc = 0;
    save_nvram();
    Serial.println("wifi已链接");
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("PSK: ");
    Serial.println(WiFi.psk());
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
  }
  else
    return false;
}

uint16_t http_get() {
  String url = "http://www.wf163.com:88/lxy.php?ver="  VER  "&sn=" + hostname
               + "&batt=" + String(v);

  Serial.println( url); //串口输出
  http.begin( url ); //HTTP提交

  int httpCode;

  for (uint8_t i = 0; i < 10; i++) {
    httpCode = http.GET();
    if (httpCode < 0) {
      Serial.write('E');
      continue;
    }
    // httpCode will be negative on error
    if (httpCode >= 200 & httpCode <= 299) {
      // HTTP header has been send and Server response header has been handled
      Serial.print("[HTTP] GET... code:");
      Serial.println(httpCode);
      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
	payload.toCharArray(disp_buf, 15); //.1.2.3.4.5,1800
	uint8_t    i1 = payload.indexOf(',');
	Serial.println(disp_buf);
	Serial.println();
	if(  disp_buf[0]=='U'
	    &&disp_buf[1]=='P' 
	    &&disp_buf[2]=='D' 
	    &&disp_buf[3]=='A' 
	    &&disp_buf[4]=='T' 
	    &&disp_buf[5]=='E') {
	  nvram.proc = HTTP_UPDATE_MODE;
	  save_nvram();
	  Serial.flush();
	  ESP.restart();	
	} 
        next_disp = disp_buf[i1 + 1] & 0xf;
        if (disp_buf[i1 + 2] >= '0' && disp_buf[i1 + 2] <= '9') {
          next_disp = next_disp * 10 + (disp_buf[i1 + 2] & 0xf);
          if (disp_buf[i1 + 3] >= '0' && disp_buf[i1 + 3] <= '9') {
            next_disp = next_disp * 10 + (disp_buf[i1 + 3] & 0xf);
            if (disp_buf[i1 + 4] >= '0' && disp_buf[i1 + 4] <= '9') {
              next_disp = next_disp * 10 + (disp_buf[i1 + 4] & 0xf);
            }
          }
        }
        next_disp = next_disp * 10;
        disp_buf[i1] = 0;
        disp(disp_buf);
        break;
      }
    } else {

      if (httpCode > 0)
        sprintf(disp_buf, ".E %03d", httpCode);
      disp(disp_buf);
      Serial.print("http error code ");
      Serial.println(httpCode);
      break;
    }
  }
  http.end();
  return httpCode;
}
#endif __WIFI_CLIENT_H__
