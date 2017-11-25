//下载方式:
//按住download键，然后按下reset，松开reset，松开download，此时进入download模式，就可以开始下载程序了
//型号选 esp8266模块，80Mhz，4Mrom/3M SPIFFS
extern "C" {
  ADC_MODE(ADC_TOUT);
#include "user_interface.h"
}
#include "nokia_5110_lcd.h"
//adc 10bit
#define PM25_EN 12  //拉高时接通pm2.5到ADC腿
#define Lbat 16  //拉高时，测量电池充电电流,采样电阻0.33欧
#define voc 9 //拉高时，测量甲醛
#define Vcc_adc 5 //拉高时接通电池电压到adc，拉高时，如果测量此腿不是高电平，则表示S4开关按下
#define SDA_Lout 2 //拉高时测量充电宝输出电流
#define DHT11 10 //温湿度数字腿

#define VCC2_EN 13 //持续拉低会给pm2.5和甲醛以及lcd背光上电 ，周期性短暂拉低，可以只开lcd背光
#define LCD_RESET 5
#define LCD_SCE 4
#define LCD_DC_PM_LED 0 // 开pm2.5的led 跟lcd/dc复用
#define LCD_SDIN 15
#define LCD_SCLK 6 //
#define OL(a)  pinMode(a,OUTPUT);digitalWrite(a,LOW);
#define OH(a)  pinMode(a,OUTPUT);digitalWrite(a,HIGH);

void setup() {
  Serial.begin(115200);
  Serial.println("\r\nhello world!");
  OL(PM25_EN);//关PM2.5ADC
  OL(Lbat);//关充电电流ADC
  OL(SDA_Lout);//关充电宝输出电流ADC
  OL(Vcc_adc);//关电源电压ADC
  Serial.flush();
  LCDInit(); //Init the LCD
  LCDClear();

  gotoXY(0, 4);
  LCDPrint("pm2.5 test");
  Serial.println(analogRead(A0));
}

// the loop function runs over and over again forever
void loop() {
  uint16_t adc;
  char disp[100];
  OL(Vcc_adc);
  OH(Lbat);//打开充电电流ADC
  delay(100);
  adc = analogRead(A0);//采样充电电流
  Serial.println(adc);
  OL(Lbat);
  gotoXY(0, 0);
  adc = (uint32_t) adc * 1000 * 3 / 1024;
  sprintf(disp, "%d ma  ", adc);
  LCDPrint(disp) ; //基准电压1V 采样电阻0.33
  Serial.print("18650充电电流=");
  Serial.print(disp);
  OL(Vcc_adc);

  OH(SDA_Lout);//打开充电电流ADC
  delay(100);
  adc = analogRead(A0);//采样充电电流
  Serial.println(adc);
  OL(SDA_Lout);
  gotoXY(0, 1);
  adc = (uint32_t) adc * 1000 * 3 / 1024;
  sprintf(disp, "%d ma  ", adc);
  LCDPrint(disp) ; //基准电压1V 采样电阻0.33
  Serial.print("充电宝输出电流=");
  Serial.print(disp);
  /*
    OH(Vcc_adc);
    delay(100);
    adc = analogRead(A0);
    OL(Vcc_adc);
    gotoXY(0, 2);
    LCDPrint("vcc=");
    LCDPrint((uint32_t)adc * 1000 / 1024 * (4990 + 976) / 976);
    LCDPrint("mV");
  */
  /*
    OL(VCC2_EN);
    OH(PM25_EN);
    OL(LCD_DC_PM_LED);
    delay(500);

    adc = analogRead(A0);
    //OH(LCD_DC_PM_LED);
    //OL(PM25_EN);
    gotoXY(0, 1);
    LCDPrint("PM2.5=");

    adc=(uint32_t)adc * 1000 / 1024 * 6;
    sprintf(disp,"%d mV",adc);
    LCDPrint(disp);
    Serial.print("PM2.5=");
    Serial.print(disp);
  */
  Serial.print("adc read vdd 33 ");
  Serial.println(system_get_vdd33());
  digitalWrite(2, LOW);   // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is acive low on the ESP-01)
  delay(1000);                      // Wait for a second
  digitalWrite(2, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
}

