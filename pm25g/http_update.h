#ifndef __HTTP_UPDATE_H__
#define __HTTP_UPDATE_H__
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#define USE_SERIAL Serial

bool http_update()
{
  USE_SERIAL.print("下载firmware from ");
  USE_SERIAL.println("http://www.wf163.com:88/pm25g.bin");
  t_httpUpdate_return  ret = ESPhttpUpdate.update("http://www.wf163.com:88/pm25g.bin");

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      USE_SERIAL.printf("HTTP_UPDATE_FAILD Error (%d): %s\r\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      nvram.proc=0;
      save_nvram();
      ESP.restart();
      break;

    case HTTP_UPDATE_NO_UPDATES:
      USE_SERIAL.println("HTTP_UPDATE_NO_UPDATES");
      break;

    case HTTP_UPDATE_OK:
      USE_SERIAL.println("HTTP_UPDATE_OK");
      return true;
      break;
  }
  delay(1000);
  return false;
}
#endif //__HTTP_UPDATE_H__
