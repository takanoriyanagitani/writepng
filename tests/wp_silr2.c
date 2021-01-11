#include "writepng.h"

int on_buf(
  uint8_t* buf,
  const uint32_t bufsz,
  uint8_t* rgba,
  const uint32_t width
){
  switch(wp_set_idat_line_rgba_raw(buf, bufsz, rgba, 1920)){
    case wp_error_ok: break;
    default:          return 1;
  }

  const uint32_t* csize = (const uint32_t*)buf;
  switch(wp_bswap5(*csize) == (7+1 + 1920*4 + 4)){
    case true: break;
    default:   return 1;
  }

  switch(memcmp(buf+4, "IDAT", 4)){
    case 0:  break;
    default: return 1;
  }

  const uint8_t* cmf = buf+4+4;
  switch(*cmf){
    case 0x78: break;
    default:   return 1;
  }

  const uint8_t* flg = cmf+1;
  switch(*flg){
    case 0x01: break;
    default:   return 1;
  }

  const uint8_t* btype = flg+1;
  fprintf(stdout, "checking btype...\n");
  switch(*btype){
    case 0x01: break;
    default:   return 1;
  }

  const uint16_t* dlen = (const uint16_t*)(btype+1);
  fprintf(stdout, "checking length...\n");
  switch(*dlen){
    case 1 + 4*1920: break;
    default:         return 1;
  }

  const uint16_t* nlen = dlen+1;
  switch(*dlen+*nlen){
    case 0xffff: break;
    default:     return 1;
  }

  fprintf(stdout, "checking filter...\n");
  const uint8_t* pixdata = (const uint8_t*)(nlen+1);
  switch(pixdata[0]){
    case 0:  break;
    default: return 1;
  }

  fprintf(stdout, "checking pixdata...\n");
  for(int i=0; i<4*width; i++){
    switch(pixdata[1+i]){
      case 0:  break;
      default: return 1;
    }
  }

  fprintf(stdout, "checking adler...\n");
  const uint32_t* adler = (const uint32_t*)(pixdata + 1 + 4*1920);
  switch(wp_bswap5(*adler)){
    case 0x1e010001: break;
    default:         return 1;
  }

  fprintf(stdout, "checking crc...\n");
  const uint32_t* crc = adler+1;
  switch(wp_bswap5(*crc)){
    case 0x4ad5bc9f: return 0;
    default:         break;
  }
  return 1;
}

int on_buf_rgba1x1(
  uint8_t* buf,
  const uint32_t bufsz,
  uint8_t* rgba,
  const uint32_t width
){
  switch(wp_set_idat_line_rgba_raw(buf, bufsz, rgba, 1)){
    case wp_error_ok: break;
    default:          return 1;
  }

  const uint32_t* csize = (const uint32_t*)buf;
  switch(wp_bswap5(*csize) == (7+1 + 1*4 + 4)){
    case true: break;
    default:   return 1;
  }

  switch(memcmp(buf+4, "IDAT", 4)){
    case 0:  break;
    default: return 1;
  }

  const uint8_t* cmf = buf+4+4;
  switch(*cmf){
    case 0x78: break;
    default:   return 1;
  }

  const uint8_t* flg = cmf+1;
  switch(*flg){
    case 0x01: break;
    default:   return 1;
  }

  const uint8_t* btype = flg+1;
  fprintf(stdout, "checking btype...\n");
  switch(*btype){
    case 0x01: break;
    default:   return 1;
  }

  const uint16_t* dlen = (const uint16_t*)(btype+1);
  fprintf(stdout, "checking length...\n");
  switch(*dlen){
    case 1 + 4*1: break;
    default:         return 1;
  }

  const uint16_t* nlen = dlen+1;
  switch(*dlen+*nlen){
    case 0xffff: break;
    default:     return 1;
  }

  const uint8_t* pixdata = (const uint8_t*)(nlen+1);
  switch(pixdata[0]){
    case 0:  break;
    default: return 1;
  }
  switch(pixdata[1+0]){ case '0': break; default:  return 1; }
  switch(pixdata[1+1]){ case '1': break; default:  return 1; }
  switch(pixdata[1+2]){ case '2': break; default:  return 1; }
  switch(pixdata[1+3]){ case '3': break; default:  return 1; }

  const uint32_t* adler = (const uint32_t*)(pixdata + 1 + 4);
  switch(wp_bswap5(*adler)){
    case 0x01ef00c7: break;
    default:         return 1;
  }

  fprintf(stdout, "checking crc...\n");
  const uint32_t* crc = adler+1;
  switch(wp_bswap5(*crc)){
    case 0x2e5c59c5: return 0;
    default:         break;
  }
  fprintf(stderr, "got: %x\n", wp_bswap5(*crc));
  return 1;
}

int fullhd(){
  switch(wp_set_idat_line_rgba_raw(NULL, 0, NULL, 1920)){
    case wp_error_mem_short: break;
    default:                 return 1;
  }
  const uint32_t bufsz = 0
    +4+4      // chunk size, IDAT
    +2+1+4    // CMF-FLG, BFINAL-BTYPE, len-nlen
    +1+1920*4 // filter type, 1920*rgba
    +4
    +4
  ;
  uint8_t* buf  = calloc(bufsz*2, 1);
  uint8_t* rgba = buf+bufsz;

  int e = on_buf(buf, bufsz, rgba, 1);
  free(buf);
  return wp_error_ok == e ? 0 : 1;
}

int rgba1x1(){
  const uint32_t bufsz = 0
    +4+4   // chunk size, IDAT
    +2+1+4 // CMF-FLG, BFINAL-BTYPE, len-nlen
    +1+1*4 // filter type, 1*rgba
    +4     // adler32
    +4     // crc32
  ;
  uint8_t* buf  = calloc(bufsz*2, 1);
  uint8_t* rgba = buf+bufsz;

  rgba[0]='0';
  rgba[1]='1';
  rgba[2]='2';
  rgba[3]='3';

  int e = on_buf_rgba1x1(buf, bufsz, rgba, 1920);
  free(buf);
  return wp_error_ok == e ? 0 : 1;
}

int main(int argc, char** argv){
  switch(argc){
    case 2:  break;
    default: return 1;
  }
  const char* type = argv[1];
  switch(type[0]){
    case 'f': return fullhd();
    case '1': return rgba1x1();
    default:  return 1;
  }
}
