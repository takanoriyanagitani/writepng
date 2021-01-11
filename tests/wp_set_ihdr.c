#include "writepng.h"

int svga(){
  uint8_t buf[256] = {0};
  switch(wp_set_ihdr_rgba_raw(buf, 256, 800, 600)){
    case wp_error_ok: break;
    default:          return 1;
  }

  const uint32_t* size = (uint32_t*)buf;
  switch(wp_bswap5(*size)){
    case 13: break;
    default: return 1;
  }

  switch(memcmp(buf+4, "IHDR", 4)){
    case 0:  break;
    default: return 1;
  }

  const uint32_t* width = size+2;
  switch(wp_bswap5(*width)){
    case 800: break;
    default:  return 1;
  }

  const uint32_t* height = width+1;
  switch(wp_bswap5(*height)){
    case 600: break;
    default:  return 1;
  }

  const uint8_t* bit = buf+16;
  switch(*bit){
    case 8:  break;
    default: return 1;
  }

  const uint8_t* color = bit+1;
  switch(*color){
    case 6:  break;
    default: return 1;
  }

  const uint8_t* comp = color+1;
  switch(*comp){
    case 0:  break;
    default: return 1;
  }

  const uint8_t* filter = comp+1;
  switch(*filter){
    case 0:  break;
    default: return 1;
  }

  const uint8_t* interlace = filter+1;
  switch(*interlace){
    case 0:  break;
    default: return 1;
  }
  return 0;
}

int main(int argc, char** argv){
  switch(argc){
    case 2:  break;
    default: return 1;
  }
  const char* type = argv[1];
  switch(type[0]){
    case 's': return svga();
    default:  return 1;
  }
}
