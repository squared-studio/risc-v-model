#include "stdio.h"

int sign_ext (uint32_t num, uint32_t len) {
  int result;
  result = num >> (len - 1);
  if ((len == 32) || ((result & 0x1) == 0x0)) {
    result = num;
  } else {
    result = (0xFFFFFFFF << len) | num;
  }
  return result;
}

uint32_t bit_select (uint32_t num, uint32_t end, uint32_t start) {
  uint32_t snum;
  uint32_t mask;
  snum = num >> (start);
  mask = 0;
  for (int i = 0; i <= (end - start); i++) {
    mask = (mask << 1) | 1;
  }
  return snum & mask;
}