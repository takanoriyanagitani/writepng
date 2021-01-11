#include "writepng.h"

int ok(){
  uint8_t buf[256] = {0};
  switch(wp_set_iend(buf, 256)){
    case wp_error_ok: break;
    default:          return 1;
  }

  const uint32_t* csize = (const uint32_t*)buf;
  switch(*csize){
    case 0:  break;
    default: return 1;
  }

  switch(memcmp(buf+4, "IEND", 4)){
    case 0:  break;
    default: return 1;
  }

  const uint32_t* crc = csize+2;
  switch(wp_bswap5(*crc)){
    case 0xae426082: return 0;
    default:         return 1;
  }
}

int ng(){
  switch(wp_set_iend(NULL,0)){
    case wp_error_mem_short: return 0;
    default:                 return 1;
  }
}

int main(int argc, char** argv){
  switch(argc){
    case 2:  break;
    default: return 1;
  }
  const char* type = argv[1];
  switch(type[0]){
    case 'o': return ok();
    case 'n': return ng();
    default:  return 1;
  }
}
