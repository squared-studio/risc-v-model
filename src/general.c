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

uint32_t hex2int(char *hex) {
    uint32_t val = 0;
    while (*hex) {
        uint8_t byte = *hex++;
        if (byte >= '0' && byte <= '9')
            byte = byte - '0';
        else if (byte >= 'a' && byte <= 'f')
            byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <= 'F')
            byte = byte - 'A' + 10;
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}

uint32_t read_intel_hex(char* filename, uint8_t mem_array[]) {
    FILE* ptr = fopen(filename, "r");
    if (ptr == NULL) {
        printf("No such file.\n");
        return 1;
    }

    uint32_t addr;
    char buf[64];
    while (fscanf(ptr, "%s", buf) == 1) {
        if (buf[0]=='@') {
            for (int i = 1; buf[i] != '\0'; i++){
                buf[i] = buf[i+1];
            }
            addr = hex2int(buf);
        } else {
            mem_array[addr] = hex2int(buf);
            addr++;
        }
    }

    fclose(ptr);
    return 0;
}
