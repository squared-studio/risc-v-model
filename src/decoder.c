#include "typedef.h"
#include "stdio.h"
#include "general.c"

#define B_TYPE(__FUNC__)                                \
  result.func = #__FUNC__;                              \
  result.rs1  = bit_select(code, 19, 15);               \
  result.rs2  = bit_select(code, 24, 20);               \
  int temp;                                             \
  temp = 0;                                             \
  temp = temp | (bit_select(code,  7,  7) << 11);       \
  temp = temp | (bit_select(code, 11,  8) <<  1);       \
  temp = temp | (bit_select(code, 30, 25) <<  5);       \
  temp = temp | (bit_select(code, 31, 31) << 12);       \
  result.imm  = sign_ext(temp,13);                      \


#define I_TYPE(__FUNC__)                                \
  result.func = #__FUNC__;                              \
  result.rd   = bit_select(code, 11, 7);                \
  result.rs1  = bit_select(code, 19, 15);               \
  result.imm  = sign_ext(bit_select(code, 31, 20), 12); \


#define J_TYPE(__FUNC__)                                \
  result.func = #__FUNC__;                              \
  result.rd   = bit_select(code, 11, 7);                \
  int temp;                                             \
  temp = 0;                                             \
  temp = temp | (bit_select(code, 19, 12) << 12);       \
  temp = temp | (bit_select(code, 20, 20) << 11);       \
  temp = temp | (bit_select(code, 30, 21) <<  1);       \
  temp = temp | (bit_select(code, 31, 31) << 20);       \
  result.imm  = sign_ext(temp,21);                      \


#define U_TYPE(__FUNC__)                                \
  result.func = #__FUNC__;                              \
  result.rd   = bit_select(code, 11, 7);                \
  result.imm  = sign_ext(code & 0xFFFFF000, 32);        \


decoded_instr_t decode (uint32_t code) {
  decoded_instr_t result;
  result.func = "INVALID";
  result.rd  = 0;
  result.rs1 = 0;
  result.rs2 = 0;
  result.rs3 = 0;
  result.imm = 0;

  if ((code & 0x0000007F) == 0x00000037) { U_TYPE(LUI);     }
  if ((code & 0x0000007F) == 0x00000017) { U_TYPE(AUIPC);   }
  if ((code & 0x0000007F) == 0x0000006F) { J_TYPE(JAL);     }
  if ((code & 0x0000707F) == 0x00000067) { I_TYPE(JALR);    }
  if ((code & 0x0000707F) == 0x00000033) { B_TYPE(BEQ);     }
  if ((code & 0x0000707F) == 0x00001033) { B_TYPE(BNE);     }
  if ((code & 0x0000707F) == 0x00004033) { B_TYPE(BLT);     }
  if ((code & 0x0000707F) == 0x00005033) { B_TYPE(BGE);     }
  if ((code & 0x0000707F) == 0x00006033) { B_TYPE(BLTU);    }
  if ((code & 0x0000707F) == 0x00007033) { B_TYPE(BGEU);    }

  return result;
}
