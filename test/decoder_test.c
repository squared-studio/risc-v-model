#include "typedef.h"
#include "decoder.c"
#include "stdio.h"
#include "stdint.h"

uint8_t mem [] = {};

int main () {

  for (int i = 0; i < 160; i++) mem[i] = 0;
  read_intel_hex("build/risc_v_tests/console_print.s/console_print.s.hex", mem);

  uint32_t code;
  decoded_instr_t instr;
  for (int i = 0; i < 40; i++) {
    code = 0;
    code = code | mem[4*i+3]; code = code << 8;
    code = code | mem[4*i+2]; code = code << 8;
    code = code | mem[4*i+1]; code = code << 8;
    code = code | mem[4*i+0];
    instr = decode(code);
    printf("0x%08x %s\n", code, instr.func);
  }

  return 0;

}
