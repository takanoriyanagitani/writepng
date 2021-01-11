#include "writepng.h"

int ok(){
  uint8_t buf[256] = {0};
  switch(wp_set_header(buf, 256)){
    case wp_error_ok: break;
    default:          return 1;
  }
  fprintf(stderr, "buf0: %x\n", buf[0]);
  switch(buf[0]){
    case 0x89: break;
    default:   return 1;
  }
  switch(memcmp(buf+1, "PNG\r\n", 5)){
    case 0:  break;
    default: return 1;
  }
  switch(buf[6]){
    case 0x1a: break;
    default:   return 1;
  }
  switch(buf[7]){
    case '\n': return 0;
    default:   return 1;
  }
}

int ng(){
  uint8_t buf[256] = {0};
  switch(wp_set_header(buf, 1)){
    case wp_error_mem_short: return 0;
    default:                 return 1;
  }
}

int main(int argc, char** argv){
  switch(argc){
    case 2:  break;
    default: return 1;
  }
  const char* okng = argv[1];
  switch(okng[0]){
    case 'o': return ok();
    case 'n': return ng();
    default:  return 1;
  }
  return 0;
}
