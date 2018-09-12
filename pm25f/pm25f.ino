//下载方式:
//按住download键，然后按下reset，松开reset，松开download，此时进入download模式，就可以开始下载程序了
//型号选 esp8266模块，80Mhz，4Mrom/3M SPIFFS
extern "C" {
  ADC_MODE(ADC_TOUT);
#include "user_interface.h"
}

#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 0, /* data=*/ 2);   // ESP32 Thing, HW I2C with pin remapping

//CD4051 address
#define ADDR_A 14
#define ADDR_B 12
#define ADDR_C 13

#define SW4 0  //CD4051-0 开关4
#define SW3 1 //CD4051-1 开关3 
#define Vcc_adc 2 //CD4051-3电源电压
#define Lout 3 //CD4051-2充电宝输出电流
#define Lbat 4  //CD4051-4测量电池充电电流,采样电阻0.33欧
#define SW5 5 //复用
#define PM25 6   //CD4051-6
#define VOC 7 //CD4051-7甲醛

#define DHT11 5 //GPIO5 温湿度数字腿
#define VCC2_EN 4 //GPIO4
#define PM_LED 15 //GPIO15 拉低 打开pm2.5的led

uint16_t select4051(uint8_t addr) {
  if (addr & B100) digitalWrite(ADDR_C, HIGH);
  else digitalWrite(ADDR_C, LOW);
  if (addr & B10) digitalWrite(ADDR_B, HIGH);
  else digitalWrite(ADDR_B, LOW);
  if (addr & 1) digitalWrite(ADDR_A, HIGH);
  else digitalWrite(ADDR_A, LOW);
  delay(2);
  return analogRead(A0);
}

void setup() {
  Serial.begin(115200);
  Serial.println("\r\nhello world!");
  pinMode(ADDR_A, OUTPUT);
  pinMode(ADDR_B, OUTPUT);
  pinMode(ADDR_C, OUTPUT);
  pinMode(PM_LED, OUTPUT);
  digitalWrite(PM_LED, HIGH);

  u8g2.begin();
  u8g2.enableUTF8Print();    // enable UTF8 support for the Arduino print() function

}
void test() {
  uint16_t dat;
  float val,val0;
  u8g2.setFont(u8g2_font_unifont_t_chinese3);  // use chinese2 for all the glyphs of "你好世界"
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  Serial.print(F("keyword SW4="));
  Serial.println(select4051(SW4));
  Serial.print(F("keyword SW5="));
  Serial.println(select4051(SW5));
  Serial.print(F("充电宝输出电流="));
  dat = select4051(Lout);
  val = 1.0 * dat / 1024 / 0.33;
  Serial.print(dat);
  Serial.print(" ");
  Serial.println(val);

  u8g2.setCursor(0, 14);
  u8g2.print("电池电压:");
  Serial.print(F("电池电压="));
  dat = select4051(Vcc_adc);
  val = 1.0 * dat / 1023 / 97.6 * (499 + 97.6);
  val = val * 4.14 / 4.31;
  Serial.print(dat);
  Serial.print(" ");
  Serial.println(val);
  u8g2.print(val);

  u8g2.setCursor(0, 34);
  u8g2.print("充电电流:");    // Chinese "Hello World"

  Serial.print(F("电池充电电流="));
  dat = select4051(Lbat);
  val = 1.0 * dat / 1024 / 0.33;
  Serial.println(val);
  u8g2.print(val);

  u8g2.setCursor(0, 54);
  u8g2.print("PM:");
  Serial.print(F("PM2.5="));
  dat = select4051(PM25);
  uint32_t dat1=0,dat0=0;
  for(dat=0;dat<20;dat++) {
  dat0 += analogRead(A0);
  digitalWrite(PM_LED, LOW);
  delayMicroseconds(280);
  dat1 += analogRead(A0);
  digitalWrite(PM_LED, HIGH);
  delay(10);
  }
  dat=dat1/20;
  val = 1.0 * dat / 1023 / 2.7 * (10 + 2.7);
  Serial.print(dat);
  Serial.print(" ");
  Serial.println(val);
  Serial.print("PM2.5_=");
  
  dat=dat0/20;
  val0 = 1.0 * dat / 1023 / 2.7 * (10 + 2.7);
  Serial.print(dat);
  Serial.print(" ");
  Serial.println(val);
  
  u8g2.print(val-val0);
  u8g2.print(",VOC");
  Serial.print(F("VOC="));
  dat0=0;
  for(dat=0;dat<20;dat++)
  dat0 += select4051(VOC);
  dat=dat0/20;
  
  val = 1.0 * dat / 1023 / 470 * (2700 + 470);
  u8g2.print(val);
  Serial.print(dat);
  Serial.print(" ");
  Serial.println(val);
  Serial.print(F("keyword SW3="));
  Serial.println(select4051(SW3));
  Serial.flush();
  u8g2.sendBuffer();
}

// the loop function runs over and over again forever
void loop() {
  // Serial.print(F("keyword SW3="));
  // Serial.println(select4051(SW3));

  test();
  delay(1000);
}

