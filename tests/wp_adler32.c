#include "writepng.h"

int main(int argc, char** argv){
  switch(argc){
    case 3:  break;
    default: return 1;
  }
  const char*    data = argv[1];
  const uint32_t len   = strlen(data);
  const char* expected = argv[2];
  char buf[256] = {0};
  uint32_t got = wp_adler32(data, len);
  snprintf(buf, 256, "%x", got);
  switch(strcmp(expected, buf)){
    case 0:  return 0;
    default: break;
  }
  fprintf(stderr, "Expected: %s. Got: %s\n", expected, buf);
  return 1;
}
