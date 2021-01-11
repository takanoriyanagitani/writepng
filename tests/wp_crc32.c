#include "writepng.h"

int iend(const char* expected){
  return 0;
}

int main(int argc, char** argv){
  switch(argc){
    case 3:  break;
    default: return 1;
  }
  const char* expected = argv[2];
  const char* input    = argv[1];
  char buf[256] = {0};
  uint32_t c = wp_crc32(input, strlen(input));
  snprintf(buf, 256, "%x", c);
  switch(strcmp(buf, expected)){
    case 0:  return 0;
    default: break;
  }
  fprintf(stderr, "expected: %s. got: %s\n", expected, buf);
  return 1;
}
