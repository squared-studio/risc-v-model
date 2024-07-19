#include "typedef.h"
#include "decoder.cc"
#include "stdio.h"
#include "stdint.h"

uint8_t mem [160];

func_t ref[40] = {
  AUIPC, ADDI, ADDI, ECALL, JAL, ADDI, ADDI, ADDI, JAL, JAL, LUI, ADDIW, ADDI, ECALL, JAL, ADDI,
  ADDI, ADDI, JAL, JAL, AUIPC, ADDI, ADDI, ECALL, JALR, ADDI, ADDI, ADDI, ADDI, ECALL, ADDI, AUIPC,
  ADDI, ECALL, ADDI, BGE, JALR, ADDI, ECALL, INVALID
};

int main () {

  int error = 0;

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

    if (instr.func != ref[i]) error++;

  }

  if (error) printf("\033[1;31mFAILED..!\033[0m\n");
  else printf("\033[1;32mPASSED..!\033[0m\n");

  return error;

}
