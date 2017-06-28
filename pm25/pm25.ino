extern "C" {
ADC_MODE(ADC_TOUT);
#include "user_interface.h"

}
#define PM25En 12
#define LbatEn 16
#define vocEn 9
#define vccAdcEn 8 //
#define LoutEn 7 //
#define DHT11En 10

#define PMLed 11 //
#define Vcc2En 13

#define LCD_RESET 5
#define LCD_SCE 4
#define LCD_DC 0
#define LCD_SDIN 15
#define LCD_SCLK 6 //
#define OL(a)  pinMode(a,OUTPUT);digitalWrite(a,LOW);
#define OH(a)  pinMode(a,OUTPUT);digitalWrite(a,HIGH);




void setup() {
OH(PM25En);
OL(2);
//OH(10);
OH(LbatEn);
OH(vocEn);
//OL(vccAdcEn);
//OH(LoutEn);
//OH(DHT11En);
OL(9);
//OH(PMLed);
Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {  
  Serial.println(analogRead(A0));
Serial.print("adc read vdd 33 ");
Serial.println(system_get_vdd33());
//Serial.print("adc read");
//Serial.println(adc.read());  
  digitalWrite(2, LOW);   // Turn the LED on (Note that LOW is the voltage level
                                    // but actually the LED is on; this is because 
                                    // it is acive low on the ESP-01)
  delay(1000);                      // Wait for a second
  digitalWrite(2, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
}
