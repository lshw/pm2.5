#ifndef __1306_H__
#define __1306_H__
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 0, /* data=*/ 2);   // ESP32 Thing, HW I2C with pin remapping

void _1306_setup() {
  pinMode(ADDR_A, OUTPUT);
  pinMode(ADDR_B, OUTPUT);
  pinMode(ADDR_C, OUTPUT);

  u8g2.begin();
  u8g2.enableUTF8Print();    // enable UTF8 support for the Arduino print() function
  u8g2.setFont(u8g2_font_unifont_t_chinese3);  // use chinese2 for all the glyphs of "你好世界"
  u8g2.setFontDirection(0);

}
char disp_title[100];
void test1(){
  u8g2.clearBuffer();
  u8g2.setCursor(0, 12);
  u8g2.print(disp_title);
  u8g2.setCursor(0, 14+12);
  u8g2.print(select4051(0));
  u8g2.setCursor(64, 14+12);
  u8g2.print(select4051(1));
  u8g2.setCursor(0, 14+12*2);
  u8g2.print(select4051(2));
  u8g2.setCursor(64, 14+12*2);
  u8g2.print(select4051(3));
  u8g2.setCursor(0, 14+12*3);
  u8g2.print(select4051(4));
  u8g2.setCursor(64, 14+12*3);
  u8g2.print(select4051(5));
  u8g2.setCursor(0, 14+12*4);
  u8g2.print(select4051(6));
  u8g2.setCursor(64, 14+12*4);
  u8g2.print(select4051(7));
  u8g2.sendBuffer();
}
void test() {
  uint16_t dat;
  float val,val0;
  u8g2.setFont(u8g2_font_unifont_t_chinese3);  // use chinese2 for all the glyphs of "你好世界"
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  Serial.print(F("keyword SW3="));
  Serial.println(select4051(SW3));
  Serial.print(F("keyword SW2="));
  Serial.println(select4051(SW2));
  Serial.print(F("keyword SW1="));
  Serial.println(select4051(SW1));
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

  u8g2.setCursor(0, 49);
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
void _1306_loop() {
  test1();
  system_soft_wdt_feed();
}
void disp(char *msg) {
  strcpy(disp_title,msg);
  Serial.println(msg);
  test1();
}
#endif
