#include "stdio.h"

#ifndef GENERAL_C__
#define GENERAL_C__

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

uint32_t hex2int (char *hex) {
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

uint32_t read_intel_hex (char* filename, uint8_t mem_array[]) {
    FILE* ptr = fopen(filename, "r");
    if (ptr == NULL) {
        printf("No such file.\n");
        return 1;
    }

    uint32_t addr;
    char buf[64];
    while (fscanf(ptr, "%s", buf) == 1) {
        if (buf[0]=='@') {
            for (int i = 0; buf[i] != '\0'; i++){
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

char to_hex_char (uint64_t data, int nibble) {
    uint64_t tmp;
    tmp = data >> (4*nibble);
    tmp = tmp & 0xF;
    if (tmp == 15) return 'F';
    if (tmp == 14) return 'E';
    if (tmp == 13) return 'D';
    if (tmp == 12) return 'C';
    if (tmp == 11) return 'B';
    if (tmp == 10) return 'A';
    if (tmp ==  9) return '9';
    if (tmp ==  8) return '8';
    if (tmp ==  7) return '7';
    if (tmp ==  6) return '6';
    if (tmp ==  5) return '5';
    if (tmp ==  4) return '4';
    if (tmp ==  3) return '3';
    if (tmp ==  2) return '2';
    if (tmp ==  1) return '1';
    if (tmp ==  0) return '0';
}

#define FILE_NAME_GENERAL_C__                           \
    char filename [50]="./";                            \
    for (int i = 0; i < 11; i++) {                      \
        filename[2+i] = to_hex_char(addr, 15-i);        \
    }                                                   \
    filename[13] = '0';                                 \
    filename[14] = '0';                                 \
    filename[15] = '0';                                 \
    filename[16] = '0';                                 \
    filename[17] = '0';                                 \
    filename[18] = '.';                                 \
    filename[19] = 'r';                                 \
    filename[20] = 'a';                                 \
    filename[21] = 'm';                                 \
    filename[22] = '\0';                                \


uint32_t load_cache (uint64_t addr, uint8_t mem_array[]) {
    FILE_NAME_GENERAL_C__

    FILE* ptr = fopen(filename, "r");
    if (ptr == NULL) {
        for (int i = 0; i < (1024*1024); i++) {
            mem_array[i] = 0;
        }
    }
    uint32_t offset = 0;
    char ch;
    for (int i = 0; i < (1024*1024); i++) {
        ch = fgetc(ptr);
        mem_array[offset] = ch;
        offset++;
    }
    fclose(ptr);
    return 0;
}

uint32_t store_cache (uint64_t addr, uint8_t mem_array[]) {
    FILE_NAME_GENERAL_C__

    FILE* ptr = fopen(filename, "w");
    for (int i = 0; i < (1024*1024); i++) {
        fprintf(ptr, "%c", mem_array[i]);
    }
    fclose(ptr);
    return 0;
}

#endif
