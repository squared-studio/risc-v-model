#include "typedef.h"
#include "stdio.h"
#include "general.c"

#define B_TYPE(__FUNC__)                                 \
  result.func = #__FUNC__;                               \
  result.rs1  = bit_select(code, 19, 15);                \
  result.rs2  = bit_select(code, 24, 20);                \
  int temp = 0;                                          \
  temp = temp | (bit_select(code,  7,  7) << 11);        \
  temp = temp | (bit_select(code, 11,  8) <<  1);        \
  temp = temp | (bit_select(code, 30, 25) <<  5);        \
  temp = temp | (bit_select(code, 31, 31) << 12);        \
  result.imm  = sign_ext(temp, 13);                      \


#define I_TYPE(__FUNC__)                                 \
  result.func  = #__FUNC__;                              \
  result.rd    = bit_select(code, 11, 7);                \
  result.rs1   = bit_select(code, 19, 15);               \
  result.imm   = sign_ext(bit_select(code, 31, 20), 12); \
  result.shamt = bit_select(code, 24, 20);               \
  result.succ  = bit_select(code, 23, 20);               \
  result.pred  = bit_select(code, 27, 24);               \
  result.fm    = bit_select(code, 31, 28);               \


#define J_TYPE(__FUNC__)                                 \
  result.func = #__FUNC__;                               \
  result.rd   = bit_select(code, 11, 7);                 \
  int temp = 0;                                          \
  temp = temp | (bit_select(code, 19, 12) << 12);        \
  temp = temp | (bit_select(code, 20, 20) << 11);        \
  temp = temp | (bit_select(code, 30, 21) <<  1);        \
  temp = temp | (bit_select(code, 31, 31) << 20);        \
  result.imm  = sign_ext(temp, 21);                      \


#define R_TYPE(__FUNC__)                                 \
  result.func = #__FUNC__;                               \
  result.rd   = bit_select(code, 11,  7);                \
  result.rs1  = bit_select(code, 19, 15);                \
  result.rs2  = bit_select(code, 24, 20);                \
  int temp = 0;                                          \
  temp = temp | (bit_select(code, 11,  7) << 0);         \
  temp = temp | (bit_select(code, 31, 25) << 5);         \
  result.imm  = sign_ext(temp, 12);                      \


#define R4_TYPE(__FUNC__)                                \
  result.func = #__FUNC__;                               \
  result.rd   = bit_select(code, 11,  7);                \
  result.rs1  = bit_select(code, 19, 15);                \
  result.rs2  = bit_select(code, 24, 20);                \
  result.rs3  = bit_select(code, 31, 27);                \


#define S_TYPE(__FUNC__)                                 \
  result.func = #__FUNC__;                               \
  result.rs1  = bit_select(code, 19, 15);                \
  result.rs2  = bit_select(code, 24, 20);                \


#define U_TYPE(__FUNC__)                                 \
  result.func = #__FUNC__;                               \
  result.rd   = bit_select(code, 11, 7);                 \
  result.imm  = sign_ext(code & 0xFFFFF000, 32);         \


decoded_instr_t decode (uint32_t code) {
  decoded_instr_t result;
  result.func  = "INVALID";

  //    $$$$$$$\  $$\    $$\  $$$$$$\   $$$$$$\  $$$$$$\ 
  //    $$  __$$\ $$ |   $$ |$$ ___$$\ $$  __$$\ \_$$  _|
  //    $$ |  $$ |$$ |   $$ |\_/   $$ |\__/  $$ |  $$ |  
  //    $$$$$$$  |\$$\  $$  |  $$$$$ /  $$$$$$  |  $$ |  
  //    $$  __$$<  \$$\$$  /   \___$$\ $$  ____/   $$ |  
  //    $$ |  $$ |  \$$$  /  $$\   $$ |$$ |        $$ |  
  //    $$ |  $$ |   \$  /   \$$$$$$  |$$$$$$$$\ $$$$$$\ 
  //    \__|  \__|    \_/     \______/ \________|\______|

  if ((code & 0x0000007F) == 0x00000037) { U_TYPE(LUI);       }
  if ((code & 0x0000007F) == 0x00000017) { U_TYPE(AUIPC);     }
  if ((code & 0x0000007F) == 0x0000006F) { J_TYPE(JAL);       }
  if ((code & 0x0000707F) == 0x00000067) { I_TYPE(JALR);      }
  if ((code & 0x0000707F) == 0x00000033) { B_TYPE(BEQ);       }
  if ((code & 0x0000707F) == 0x00001033) { B_TYPE(BNE);       }
  if ((code & 0x0000707F) == 0x00004033) { B_TYPE(BLT);       }
  if ((code & 0x0000707F) == 0x00005033) { B_TYPE(BGE);       }
  if ((code & 0x0000707F) == 0x00006033) { B_TYPE(BLTU);      }
  if ((code & 0x0000707F) == 0x00007033) { B_TYPE(BGEU);      }
  if ((code & 0x0000707F) == 0x00000003) { I_TYPE(LB);        }
  if ((code & 0x0000707F) == 0x00001003) { I_TYPE(LH);        }
  if ((code & 0x0000707F) == 0x00002003) { I_TYPE(LW);        }
  if ((code & 0x0000707F) == 0x00004003) { I_TYPE(LBU);       }
  if ((code & 0x0000707F) == 0x00005003) { I_TYPE(LHU);       }
  if ((code & 0x0000707F) == 0x00000023) { S_TYPE(SB);        }
  if ((code & 0x0000707F) == 0x00001023) { S_TYPE(SH);        }
  if ((code & 0x0000707F) == 0x00002023) { S_TYPE(SW);        }
  if ((code & 0x0000707F) == 0x00000013) { I_TYPE(ADDI);      }
  if ((code & 0x0000707F) == 0x00002013) { I_TYPE(SLTI);      }
  if ((code & 0x0000707F) == 0x00003013) { I_TYPE(SLTIU);     }
  if ((code & 0x0000707F) == 0x00004013) { I_TYPE(XORI);      }
  if ((code & 0x0000707F) == 0x00006013) { I_TYPE(ORI);       }
  if ((code & 0x0000707F) == 0x00007013) { I_TYPE(ANDI);      }
  if ((code & 0xFE00707F) == 0x00001013) { I_TYPE(SLLI);      }
  if ((code & 0xFE00707F) == 0x00005013) { I_TYPE(SRLI);      }
  if ((code & 0xFE00707F) == 0x40005013) { I_TYPE(SRAI);      }
  if ((code & 0xFE00707F) == 0x00000033) { R_TYPE(ADD);       }
  if ((code & 0xFE00707F) == 0x40000033) { R_TYPE(SUB);       }
  if ((code & 0xFE00707F) == 0x00001033) { R_TYPE(SLL);       }
  if ((code & 0xFE00707F) == 0x00002033) { R_TYPE(SLT);       }
  if ((code & 0xFE00707F) == 0x00003033) { R_TYPE(SLTU);      }
  if ((code & 0xFE00707F) == 0x00004033) { R_TYPE(XOR);       }
  if ((code & 0xFE00707F) == 0x00005033) { R_TYPE(SRL);       }
  if ((code & 0xFE00707F) == 0x40005033) { R_TYPE(SRA);       }
  if ((code & 0xFE00707F) == 0x00006033) { R_TYPE(OR);        }
  if ((code & 0xFE00707F) == 0x00007033) { R_TYPE(AND);       }
  if ((code & 0x0000707F) == 0x0000000F) { I_TYPE(FENCE);     }
  if ((code & 0xFFFFFFFF) == 0x8330000F) { I_TYPE(FENCE_TSO); }
  if ((code & 0xFFFFFFFF) == 0x0100000F) { I_TYPE(PAUSE);     }
  if ((code & 0xFFFFFFFF) == 0x00000073) { I_TYPE(ECALL);     }
  if ((code & 0xFFFFFFFF) == 0x00100073) { I_TYPE(EBREAK);    }

  return result;

}
