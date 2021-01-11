#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <string.h>

#include <zlib.h>

uint32_t wp_bswap5(const uint32_t x){ return __builtin_bswap32(x); }

typedef enum {
  wp_error_ok        = 0,
  wp_error_mem_short = 1,
} wp_error;

uint32_t wp_adler32(const void* data, uint32_t len){
  uint32_t a = adler32(0L, Z_NULL, 0);
  return adler32(a, data, len);
}

uint32_t wp_crc32(const void* data, uint32_t len){
  uint32_t c = crc32(0L, Z_NULL, 0);
  return crc32(c, data, len);
}

int wp_set_header(uint8_t* buf, const uint32_t len){
  switch(8 <= len){
    case true: break;
    default:   return wp_error_mem_short;
  }
  buf[0]=0x89;
  snprintf((char*)buf+1, 6, "PNG\r\n");
  buf[6]=0x1a;
  buf[7]=0x0a;
  return wp_error_ok;
}

int wp_set_ihdr_rgba_raw(
  uint8_t* buf,
  const uint32_t len,
  const uint32_t width,
  const uint32_t height
){
  switch(25 <= len){
    case true: break;
    default:   return wp_error_mem_short;
  }

  uint32_t* size = (uint32_t*)buf;
  *size = wp_bswap5(13);

  memcpy(buf+4, "IHDR", 4);

  uint32_t* w = size+2;
  *w = wp_bswap5(width);

  uint32_t* h = w+1;
  *h = wp_bswap5(height);

  uint8_t* bit = buf+16;
  *bit = 8;

  uint8_t* color = bit+1;
  *color = 6;

  uint8_t* comp = color+1;
  *comp = 0;

  uint8_t* filter = comp+1;
  *filter = 0;

  uint8_t* interlace = filter+1;
  *interlace = 0;

  uint32_t* chk = (uint32_t*)(interlace+1);
  *chk = wp_bswap5(wp_crc32(buf+4, 4+13));

  return wp_error_ok;
}

int wp_set_idat_line_rgba_raw(
  uint8_t* buf,
  const uint32_t len,
  const uint8_t* data,
  const uint32_t width
){
  const uint32_t org_size = 0
    + 1       // filter type
    + 4*width // 4*rgba
  ;
  const uint32_t def_required = 0
    + 2        // CMF,FLG
    + 1        // BFINAL,BTYPE
    + 4        // len,nlen
    + org_size
    + 4        // adler32
  ;
  const uint32_t len_required = 0
    + 4             // chunk size
    + 4             // IDAT
    + def_required
    + 4             // crc32(png)
  ;
  switch(len_required <= len){
    case true: break;
    default:   return wp_error_mem_short;
  }

  uint32_t* csize = (uint32_t*)buf;
  *csize = wp_bswap5(def_required);

  memcpy(buf+4, "IDAT", 4);

  uint8_t* cmf = buf+4+4;
  *cmf = 0x78; // CM=8, CINFO=7(deflate, window size=32k)

  uint8_t* flg = cmf+1;
  *flg = 0x01; // no dict, fastest compression

  uint16_t* dlen = (uint16_t*)(flg+1);
  *dlen = org_size;

  uint16_t* nlen = dlen+1;
  *nlen = *dlen ^ 0xffff;

  uint8_t* pixdata = (uint8_t*)(nlen+1);
  *pixdata = 0x00; // no filter
  memcpy(pixdata+1, data, 4*width);

  uint32_t* adler = (uint32_t*)(pixdata + 1 + 4*width);
  *adler = wp_bswap5(wp_adler32(pixdata, 1 + 4*width));

  uint32_t* c = adler+1;
  *c = wp_bswap5(wp_crc32(
    buf+4,         // IDAT
    0
    + 4            // IDAT
    + def_required
  ));

  return wp_error_ok;
}
