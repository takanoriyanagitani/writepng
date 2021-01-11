#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <string.h>

#include <zlib.h>

typedef enum {
  wp_error_ok        = 0,
  wp_error_mem_short = 1,
} wp_error;

uint32_t wp_adler32(const uint8_t* data, uint32_t len){
  uint32_t a = adler32(0L, Z_NULL, 0);
  return adler32(a, data, len);
}

uint32_t wp_crc32(const uint8_t* data, uint32_t len){
  uint32_t c = crc32(0L, Z_NULL, 0);
  return crc32(c, data, len);
}

int wp_set_header(uint8_t* buf, const uint32_t len){
  switch(8 <= len){
    case true: break;
    default:   return wp_error_mem_short;
  }
  buf[0]=0x89;
  snprintf(buf+1, 6, "PNG\r\n");
  buf[6]=0x1a;
  buf[7]=0x0a;
  return wp_error_ok;
}

uint32_t wp_bswap5(const uint32_t x){ return __builtin_bswap32(x); }

int wp_set_ihdr_rgba_raw(
  uint8_t* buf,
  const uint32_t len,
  const uint32_t width,
  const uint32_t height
){
  switch(21 <= len){
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

  return wp_error_ok;
}
