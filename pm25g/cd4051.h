#ifndef __CD4051_H__
#define __CD4051_H__
uint16_t select4051(uint8_t addr) {
  if (addr & B100) digitalWrite(ADDR_C, HIGH);
  else digitalWrite(ADDR_C, LOW);
  if (addr & B10) digitalWrite(ADDR_B, HIGH);
  else digitalWrite(ADDR_B, LOW);
  if (addr & 1) digitalWrite(ADDR_A, HIGH);
  else digitalWrite(ADDR_A, LOW);
  delay(1);
  return analogRead(A0);
}
#endif //__CD4051_H__
