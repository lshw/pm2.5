struct {
  uint32_t crc32;
  byte data[507];
  uint8_t proc;
} nvram;
uint32_t calculateCRC32(const uint8_t *data, size_t length);

void init_nvram(){
  nvram.proc=0;
  memset(nvram.data,0,sizeof(nvram.data));
  nvram.crc32=calculateCRC32((uint8_t*) &nvram.data[0], sizeof(nvram)-sizeof(nvram.crc32));
}

bool load_nvram(){
  if (ESP.rtcUserMemoryRead(0, (uint32_t*) &nvram, sizeof(nvram))) {
    if (calculateCRC32((uint8_t*) &nvram.data[0], sizeof(nvram)-sizeof(nvram.crc32)) == nvram.crc32) 
    {
      return true;
    }
  }
  init_nvram();
  return false;
}

void save_nvram(){
nvram.crc32=calculateCRC32((uint8_t*) &nvram.data[0], sizeof(nvram)-sizeof(nvram.crc32));
ESP.rtcUserMemoryWrite(0, (uint32_t*) &nvram, sizeof(nvram));
}


uint32_t calculateCRC32(const uint8_t *data, size_t length) {
  uint32_t crc = 0xffffffff;
  while (length--) {
    uint8_t c = *data++;
    for (uint32_t i = 0x80; i > 0; i >>= 1) {
      bool bit = crc & 0x80000000;
      if (c & i) {
        bit = !bit;
      }
      crc <<= 1;
      if (bit) {
        crc ^= 0x04c11db7;
      }
    }
  }
  return crc;
}
