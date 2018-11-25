#include <FS.h>
#define VER "0.1"
#define HOSTNAME "disp_"
extern "C" {
  ADC_MODE(ADC_TOUT);
#include "user_interface.h"
}
char disp_buf[22];
uint32_t next_disp = 120; //下次开机
String hostname = HOSTNAME;
float v;
uint8_t proc; //用lcd ram 0 传递过来的变量， 用于通过重启，进行功能切换
//0,1-正常 2-AP 3-OTA  4-http update
#define AP_MODE 2
#define OTA_MODE 3
#define HTTP_UPDATE_MODE 4

#include "pin.h"
#include "nvram.h"
//#include "setting.h"
#include "cd4051.h"
#include "_1306.h";
#include "ota.h"
#include "wifi_client.h"
#include "ap_web.h"
#include "http_update.h"
void setup()
{
  uint8_t i;
  Serial.begin(115200);
  Serial.println();
  Serial.println("Software Ver=" VER);
  hostname += String(ESP.getChipId(), HEX);
  WiFi.hostname(hostname);
  Serial.println("Hostname: " + hostname);
  delay(100);
  get_batt();
  _1306_setup();
  Serial.print("电池电压");
  Serial.println(v);
  Serial.flush();
  load_nvram();
  proc=nvram.proc;
  switch (proc) {
    case HTTP_UPDATE_MODE: //http_update
      nvram.proc = 0;
      disp("Http update");
      break;
    case OTA_MODE:
      nvram.proc = HTTP_UPDATE_MODE;//ota以后，
      disp(" OTA by WiFi ");
      break;
    case AP_MODE:
      nvram.proc = OTA_MODE; //ota
      save_nvram();
      AP();
      return;
      break;
    default:
      nvram.proc = AP_MODE;
      sprintf(disp_buf, " %3.2f ", v);
      disp(disp_buf);
      break;
  }
  save_nvram();
  //更新时闪烁

  if (wifi_connect() == false) {
    Serial.print("不能链接到AP\r\n20分钟后再试试\r\n本次上电时长");
    Serial.print(millis());
    Serial.println("ms");
    poweroff(1200);
    return;
  }
  if (proc == AP_MODE) return;
  if (proc == OTA_MODE) {
    ota_setup();
    return;
  }
  if (proc == HTTP_UPDATE_MODE) {
    if (http_update() == true) {
      nvram.proc = 0;
      save_nvram();
      disp("Http Update ok!");
      delay(2000);
    }

    ESP.restart();
    return;
  }
  uint16_t httpCode = http_get();
  if (httpCode >= 400) {
    Serial.print("不能链接到web\r\n20分钟后再试试\r\n本次上电时长");
    Serial.print(millis());
    Serial.println("ms");
    poweroff(1200);
    return;
  }
  //if (v < 3.6)
  //else
  Serial.print("uptime=");
  Serial.print(millis());
  if (next_disp == 0) next_disp = 120;
  Serial.print("ms,sleep=");
  Serial.println(next_disp);
  wifi_set_sleep_type(LIGHT_SLEEP_T);
  //poweroff(next_disp);
  wifi_set_sleep_type(LIGHT_SLEEP_T);
}
bool power_off = false;
void poweroff(uint32_t sec) {
  wdt_disable();
  Serial.print(sec % 60);
  Serial.println("秒");
  Serial.println("bye!");
  Serial.flush();
  system_deep_sleep_set_option(0);
  digitalWrite(LED_BUILTIN, LOW);
  ESP.deepSleep((uint64_t) 1000000 * sec, WAKE_RF_DEFAULT);
  //system_deep_sleep((uint64_t)1000000 * sec);
  power_off = true;
}
uint32_t get_value(){
        analogRead(A0);
  return (analogRead(A0)
        + analogRead(A0)
        + analogRead(A0)
        + analogRead(A0)
        + analogRead(A0)
        + analogRead(A0)
        + analogRead(A0)
        + analogRead(A0))/8;
}
float get_batt() {//锂电池电压
  select4051(Vcc_adc); 
//81.633=(499*97.6)/(499+97.6);
  v = (499.0+81.633)*get_value() / 81.633 / 1023.0 ;
  return v;
}
uint32_t sec0;
void loop()
{
 uint32_t s0=millis()/1000;
 uint8_t s=s0-sec0;
  Serial.print(__FILE__);Serial.println(__LINE__);
  if (power_off) return;
  switch (proc) {
    case OTA_MODE:
      ota_loop(s);
      break;
    case AP_MODE:
      digitalWrite(13,LOW);
      ap_loop(s);
      break;
   default:
     if(s>0)
     _1306_loop();
     Serial.print(__FILE__);Serial.println(__LINE__);
  }
  Serial.print(__FILE__);Serial.println(__LINE__);
sec0=s0;
}
