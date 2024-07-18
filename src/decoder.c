#include "typedef.h"
#include "stdio.h"
#include "general.c"

#ifndef DECODER_C__
#define DECODER_C__

#define B_TYPE(__FUNC__)                                 \
  result.func = #__FUNC__;                               \
  int temp = 0;                                          \
  temp = temp | (bit_select(code,  7,  7) << 11);        \
  temp = temp | (bit_select(code, 11,  8) <<  1);        \
  temp = temp | (bit_select(code, 30, 25) <<  5);        \
  temp = temp | (bit_select(code, 31, 31) << 12);        \
  result.imm  = sign_ext(temp, 13);                      \


#define I_TYPE(__FUNC__)                                 \
  result.func  = #__FUNC__;                              \
  result.imm   = sign_ext(bit_select(code, 31, 20), 12); \


#define J_TYPE(__FUNC__)                                 \
  result.func = #__FUNC__;                               \
  int temp = 0;                                          \
  temp = temp | (bit_select(code, 19, 12) << 12);        \
  temp = temp | (bit_select(code, 20, 20) << 11);        \
  temp = temp | (bit_select(code, 30, 21) <<  1);        \
  temp = temp | (bit_select(code, 31, 31) << 20);        \
  result.imm  = sign_ext(temp, 21);                      \


#define R_TYPE(__FUNC__)                                 \
  result.func = #__FUNC__;                               \
  int temp = 0;                                          \
  temp = temp | (bit_select(code, 11,  7) << 0);         \
  temp = temp | (bit_select(code, 31, 25) << 5);         \
  result.imm  = sign_ext(temp, 12);                      \
  

#define R4_TYPE(__FUNC__)                                \
  result.func = #__FUNC__;                               \


#define S_TYPE(__FUNC__)                                 \
  result.func = #__FUNC__;                               \


#define U_TYPE(__FUNC__)                                 \
  result.func = #__FUNC__;                               \
  result.imm  = sign_ext(code & 0xFFFFF000, 32);         \


decoded_instr_t decode (uint32_t code) {
  decoded_instr_t result;
  result.func  = "INVALID";
  result.rd    = bit_select(code, 11, 7);
  result.rs1   = bit_select(code, 19, 15);
  result.rs2   = bit_select(code, 24, 20);
  result.rs3   = bit_select(code, 31, 27);
  result.rl    = bit_select(code, 25, 25);
  result.aq    = bit_select(code, 26, 26);
  result.rm    = bit_select(code, 14, 12);
  result.shamt = bit_select(code, 25, 20);
  result.succ  = bit_select(code, 23, 20);
  result.pred  = bit_select(code, 27, 24);
  result.fm    = bit_select(code, 31, 28);
  result.csr   = bit_select(code, 31, 20);
  result.imm   = bit_select(code, 19, 15);



  //          88888888ba   8b           d8   ad888888b,   ad888888b,  88
  //          88      "8b  `8b         d8'  d8"     "88  d8"     "88  88
  //          88      ,8P   `8b       d8'           a8P          a8P  88
  //          88aaaaaa8P'    `8b     d8'         aad8"        ,d8P"   88
  //          88""""88'       `8b   d8'          ""Y8,      a8P"      88
  //          88    `8b        `8b d8'              "8b   a8P'        88
  //          88     `8b        `888'       Y8,     a88  d8"          88
  //          88      `8b        `8'         "Y888888P'  88888888888  88

  if ((code & 0x0000007F) == 0x00000037) { U_TYPE(LUI);       }
  if ((code & 0x0000007F) == 0x00000017) { U_TYPE(AUIPC);     }
  if ((code & 0x0000007F) == 0x0000006F) { J_TYPE(JAL);       }
  if ((code & 0x0000707F) == 0x00000067) { I_TYPE(JALR);      }
  if ((code & 0x0000707F) == 0x00000063) { B_TYPE(BEQ);       }
  if ((code & 0x0000707F) == 0x00001063) { B_TYPE(BNE);       }
  if ((code & 0x0000707F) == 0x00004063) { B_TYPE(BLT);       }
  if ((code & 0x0000707F) == 0x00005063) { B_TYPE(BGE);       }
  if ((code & 0x0000707F) == 0x00006063) { B_TYPE(BLTU);      }
  if ((code & 0x0000707F) == 0x00007063) { B_TYPE(BGEU);      }
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



  //          88888888ba   8b           d8   ad8888ba,          ,d8    88
  //          88      "8b  `8b         d8'  8P'    "Y8        ,d888    88
  //          88      ,8P   `8b       d8'  d8               ,d8" 88    88
  //          88aaaaaa8P'    `8b     d8'   88,dd888bb,    ,d8"   88    88
  //          88""""88'       `8b   d8'    88P'    `8b  ,d8"     88    88
  //          88    `8b        `8b d8'     88       d8  8888888888888  88
  //          88     `8b        `888'      88a     a8P           88    88
  //          88      `8b        `8'        "Y88888P"            88    88

  if ((code & 0x0000707F) == 0x00006003) { I_TYPE(LWU);       }
  if ((code & 0x0000707F) == 0x00003003) { I_TYPE(LD);        }
  if ((code & 0x0000707F) == 0x00003023) { S_TYPE(SD);        }
  if ((code & 0xFC00707F) == 0x00001013) { I_TYPE(SLLI);      }
  if ((code & 0xFC00707F) == 0x00005013) { I_TYPE(SRLI);      }
  if ((code & 0xFC00707F) == 0x40005013) { I_TYPE(SRAI);      }
  if ((code & 0x0000707F) == 0x0000001B) { I_TYPE(ADDIW);     }
  if ((code & 0xFE00707F) == 0x0000101B) { I_TYPE(SLLIW);     }
  if ((code & 0xFE00707F) == 0x0000501B) { I_TYPE(SRLIW);     }
  if ((code & 0xFE00707F) == 0x4000501B) { I_TYPE(SRAIW);     }
  if ((code & 0xFE00707F) == 0x0000003B) { R_TYPE(ANDW);      }
  if ((code & 0xFE00707F) == 0x4000003B) { R_TYPE(SUBW);      }
  if ((code & 0xFE00707F) == 0x0000103B) { R_TYPE(SLLW);      }
  if ((code & 0xFE00707F) == 0x0000503B) { R_TYPE(SRLW);      }
  if ((code & 0xFE00707F) == 0x4000503B) { R_TYPE(SRAW);      }



  //          888888888888  88     ad88                                                  88
  //                   ,88  ""    d8"                                                    ""
  //                 ,88"         88
  //               ,88"     88  MM88MMM  ,adPPYba,  8b,dPPYba,    ,adPPYba,   ,adPPYba,  88
  //             ,88"       88    88    a8P_____88  88P'   `"8a  a8"     ""  a8P_____88  88
  //           ,88"         88    88    8PP"""""""  88       88  8b          8PP"""""""  88
  //          88"           88    88    "8b,   ,aa  88       88  "8a,   ,aa  "8b,   ,aa  88
  //          888888888888  88    88     `"Ybbd8"'  88       88   `"Ybbd8"'   `"Ybbd8"'  88

  if ((code & 0x0000707F) == 0x0000100F) { I_TYPE(FENC_I);    }



  //          888888888888  88
  //                   ,88  ""
  //                 ,88"
  //               ,88"     88   ,adPPYba,  ,adPPYba,  8b,dPPYba,
  //             ,88"       88  a8"     ""  I8[    ""  88P'   "Y8
  //           ,88"         88  8b           `"Y8ba,   88
  //          88"           88  "8a,   ,aa  aa    ]8I  88
  //          888888888888  88   `"Ybbd8"'  `"YbbdP"'  88

  if ((code & 0x0000707F) == 0x00001073) { I_TYPE(CSRRW);     }
  if ((code & 0x0000707F) == 0x00002073) { I_TYPE(CSRRS);     }
  if ((code & 0x0000707F) == 0x00003073) { I_TYPE(CSRRC);     }
  if ((code & 0x0000707F) == 0x00005073) { I_TYPE(CSRRWI);    }
  if ((code & 0x0000707F) == 0x00006073) { I_TYPE(CSRRSI);    }
  if ((code & 0x0000707F) == 0x00007073) { I_TYPE(CSRRCI);    }



  //          88888888ba   8b           d8   ad888888b,   ad888888b,  88b           d88
  //          88      "8b  `8b         d8'  d8"     "88  d8"     "88  888b         d888
  //          88      ,8P   `8b       d8'           a8P          a8P  88`8b       d8'88
  //          88aaaaaa8P'    `8b     d8'         aad8"        ,d8P"   88 `8b     d8' 88
  //          88""""88'       `8b   d8'          ""Y8,      a8P"      88  `8b   d8'  88
  //          88    `8b        `8b d8'              "8b   a8P'        88   `8b d8'   88
  //          88     `8b        `888'       Y8,     a88  d8"          88    `888'    88
  //          88      `8b        `8'         "Y888888P'  88888888888  88     `8'     88

  if ((code & 0xFE00707F) == 0x02000033) { R_TYPE(MUL);       }
  if ((code & 0xFE00707F) == 0x02001033) { R_TYPE(MULH);      }
  if ((code & 0xFE00707F) == 0x02002033) { R_TYPE(MULHSU);    }
  if ((code & 0xFE00707F) == 0x02003033) { R_TYPE(MULHU);     }
  if ((code & 0xFE00707F) == 0x02004033) { R_TYPE(DIV);       }
  if ((code & 0xFE00707F) == 0x02005033) { R_TYPE(DIVU);      }
  if ((code & 0xFE00707F) == 0x02006033) { R_TYPE(REM);       }
  if ((code & 0xFE00707F) == 0x02007033) { R_TYPE(REMU);      }



  //          88888888ba   8b           d8   ad8888ba,          ,d8    88b           d88
  //          88      "8b  `8b         d8'  8P'    "Y8        ,d888    888b         d888
  //          88      ,8P   `8b       d8'  d8               ,d8" 88    88`8b       d8'88
  //          88aaaaaa8P'    `8b     d8'   88,dd888bb,    ,d8"   88    88 `8b     d8' 88
  //          88""""88'       `8b   d8'    88P'    `8b  ,d8"     88    88  `8b   d8'  88
  //          88    `8b        `8b d8'     88       d8  8888888888888  88   `8b d8'   88
  //          88     `8b        `888'      88a     a8P           88    88    `888'    88
  //          88      `8b        `8'        "Y88888P"            88    88     `8'     88

  if ((code & 0xFE00707F) == 0x0200003B) { R_TYPE(MULW);      }
  if ((code & 0xFE00707F) == 0x0200403B) { R_TYPE(DIVW);      }
  if ((code & 0xFE00707F) == 0x0200503B) { R_TYPE(DIVUW);     }
  if ((code & 0xFE00707F) == 0x0200603B) { R_TYPE(REMW);      }
  if ((code & 0xFE00707F) == 0x0200703B) { R_TYPE(REMUW);     }



  //          88888888ba   8b           d8   ad888888b,   ad888888b,         db
  //          88      "8b  `8b         d8'  d8"     "88  d8"     "88        d88b
  //          88      ,8P   `8b       d8'           a8P          a8P       d8'`8b
  //          88aaaaaa8P'    `8b     d8'         aad8"        ,d8P"       d8'  `8b
  //          88""""88'       `8b   d8'          ""Y8,      a8P"         d8YaaaaY8b
  //          88    `8b        `8b d8'              "8b   a8P'          d8""""""""8b
  //          88     `8b        `888'       Y8,     a88  d8"           d8'        `8b
  //          88      `8b        `8'         "Y888888P'  88888888888  d8'          `8b

  if ((code & 0xF9F0707F) == 0x1000202F) { R_TYPE(LR_W);      }
  if ((code & 0xF800707F) == 0x1800202F) { R_TYPE(SC_W);      }
  if ((code & 0xF800707F) == 0x0800202F) { R_TYPE(AMOSWAP_W); }
  if ((code & 0xF800707F) == 0x0000202F) { R_TYPE(AMOADD_W);  }
  if ((code & 0xF800707F) == 0x2000202F) { R_TYPE(AMOXOR_W);  }
  if ((code & 0xF800707F) == 0x6000202F) { R_TYPE(AMOAND_W);  }
  if ((code & 0xF800707F) == 0x4000202F) { R_TYPE(AMOOR_W);   }
  if ((code & 0xF800707F) == 0x8000202F) { R_TYPE(AMOMIN_W);  }
  if ((code & 0xF800707F) == 0xA000202F) { R_TYPE(AMOMAX_W);  }
  if ((code & 0xF800707F) == 0xC000202F) { R_TYPE(AMOMINU_W); }
  if ((code & 0xF800707F) == 0xE000202F) { R_TYPE(AMOMAXU_W); }



  //          88888888ba   8b           d8   ad8888ba,          ,d8         db
  //          88      "8b  `8b         d8'  8P'    "Y8        ,d888        d88b
  //          88      ,8P   `8b       d8'  d8               ,d8" 88       d8'`8b
  //          88aaaaaa8P'    `8b     d8'   88,dd888bb,    ,d8"   88      d8'  `8b
  //          88""""88'       `8b   d8'    88P'    `8b  ,d8"     88     d8YaaaaY8b
  //          88    `8b        `8b d8'     88       d8  8888888888888  d8""""""""8b
  //          88     `8b        `888'      88a     a8P           88   d8'        `8b
  //          88      `8b        `8'        "Y88888P"            88  d8'          `8b

  if ((code & 0xF9F0707F) == 0x1000302F) { R_TYPE(LR_D);      }
  if ((code & 0xF800707F) == 0x1800302F) { R_TYPE(SC_D);      }
  if ((code & 0xF800707F) == 0x0800302F) { R_TYPE(AMOSWAP_D); }
  if ((code & 0xF800707F) == 0x0000302F) { R_TYPE(AMOADD_D);  }
  if ((code & 0xF800707F) == 0x2000302F) { R_TYPE(AMOXOR_D);  }
  if ((code & 0xF800707F) == 0x6000302F) { R_TYPE(AMOAND_D);  }
  if ((code & 0xF800707F) == 0x4000302F) { R_TYPE(AMOOR_D);   }
  if ((code & 0xF800707F) == 0x8000302F) { R_TYPE(AMOMIN_D);  }
  if ((code & 0xF800707F) == 0xA000302F) { R_TYPE(AMOMAX_D);  }
  if ((code & 0xF800707F) == 0xC000302F) { R_TYPE(AMOMINU_D); }
  if ((code & 0xF800707F) == 0xE000302F) { R_TYPE(AMOMAXU_D); }



  //          88888888ba   8b           d8   ad888888b,   ad888888b,  88888888888
  //          88      "8b  `8b         d8'  d8"     "88  d8"     "88  88
  //          88      ,8P   `8b       d8'           a8P          a8P  88
  //          88aaaaaa8P'    `8b     d8'         aad8"        ,d8P"   88aaaaa
  //          88""""88'       `8b   d8'          ""Y8,      a8P"      88"""""
  //          88    `8b        `8b d8'              "8b   a8P'        88
  //          88     `8b        `888'       Y8,     a88  d8"          88
  //          88      `8b        `8'         "Y888888P'  88888888888  88

  if ((code & 0x0000707F) == 0x00002007) { R_TYPE(FLW);       }
  if ((code & 0x0000707F) == 0x00002027) { S_TYPE(FSW);       }
  if ((code & 0x0600007F) == 0x00000043) { R4_TYPE(FMADD_S);  }
  if ((code & 0x0600007F) == 0x00000047) { R4_TYPE(FMSUB_S);  }
  if ((code & 0x0600007F) == 0x0000004B) { R4_TYPE(FNMSUB_S); }
  if ((code & 0x0600007F) == 0x0000004F) { R4_TYPE(FNMADD_S); }
  if ((code & 0xFE00007F) == 0x00000053) { R_TYPE(FADD_S);    }
  if ((code & 0xFE00007F) == 0x08000053) { R_TYPE(FSUB_S);    }
  if ((code & 0xFE00007F) == 0x10000053) { R_TYPE(FMUL_S);    }
  if ((code & 0xFE00007F) == 0x18000053) { R_TYPE(FDIV_S);    }
  if ((code & 0xFFF0007F) == 0x58000053) { R_TYPE(FSQRT_S);   }
  if ((code & 0xFE00707F) == 0x20000053) { R_TYPE(FSGNJ_S);   }
  if ((code & 0xFE00707F) == 0x20001053) { R_TYPE(FSGNJN_S);  }
  if ((code & 0xFE00707F) == 0x20002053) { R_TYPE(FSGNJX_S);  }
  if ((code & 0xFE00707F) == 0x28000053) { R_TYPE(FMIN_S);    }
  if ((code & 0xFE00707F) == 0x28001053) { R_TYPE(FMAX_S);    }
  if ((code & 0xFFF0007F) == 0xC0000053) { R_TYPE(FCVT_W_S);  }
  if ((code & 0xFFF0007F) == 0xC0100053) { R_TYPE(FCVT_WU_S); }
  if ((code & 0xFFF0707F) == 0xE0000053) { R_TYPE(FMV_X_W);   }
  if ((code & 0xFE00707F) == 0xA0002053) { R_TYPE(FEQ_S);     }
  if ((code & 0xFE00707F) == 0xA0001053) { R_TYPE(FLT_S);     }
  if ((code & 0xFE00707F) == 0xA0000053) { R_TYPE(FLE_S);     }
  if ((code & 0xFFF0707F) == 0xE0001053) { R_TYPE(FCLASS_S);  }
  if ((code & 0xFFF0007F) == 0xD0000053) { R_TYPE(FCVT_S_W);  }
  if ((code & 0xFFF0007F) == 0xD0100053) { R_TYPE(FCVT_S_WU); }
  if ((code & 0xFFF0707F) == 0xF0000053) { R_TYPE(FMV_W_X);   }



  //          88888888ba   8b           d8   ad8888ba,          ,d8    88888888888
  //          88      "8b  `8b         d8'  8P'    "Y8        ,d888    88
  //          88      ,8P   `8b       d8'  d8               ,d8" 88    88
  //          88aaaaaa8P'    `8b     d8'   88,dd888bb,    ,d8"   88    88aaaaa
  //          88""""88'       `8b   d8'    88P'    `8b  ,d8"     88    88"""""
  //          88    `8b        `8b d8'     88       d8  8888888888888  88
  //          88     `8b        `888'      88a     a8P           88    88
  //          88      `8b        `8'        "Y88888P"            88    88

  if ((code & 0xFFF0007F) == 0xC0200053) { R_TYPE(FCVT_L_S);  }
  if ((code & 0xFFF0007F) == 0xC0300053) { R_TYPE(FCVT_LU_S); }
  if ((code & 0xFFF0007F) == 0xD0200053) { R_TYPE(FCVT_S_L);  }
  if ((code & 0xFFF0007F) == 0xD0300053) { R_TYPE(FCVT_S_LU); }



  //          88888888ba   8b           d8   ad888888b,   ad888888b,  88888888ba,
  //          88      "8b  `8b         d8'  d8"     "88  d8"     "88  88      `"8b
  //          88      ,8P   `8b       d8'           a8P          a8P  88        `8b
  //          88aaaaaa8P'    `8b     d8'         aad8"        ,d8P"   88         88
  //          88""""88'       `8b   d8'          ""Y8,      a8P"      88         88
  //          88    `8b        `8b d8'              "8b   a8P'        88         8P
  //          88     `8b        `888'       Y8,     a88  d8"          88      .a8P
  //          88      `8b        `8'         "Y888888P'  88888888888  88888888Y"'

  if ((code & 0x0000707F) == 0x00003007) { R_TYPE(FLD);       }
  if ((code & 0x0000707F) == 0x00003027) { S_TYPE(FSD);       }
  if ((code & 0x0600007F) == 0x02000043) { R4_TYPE(FMADD_D);  }
  if ((code & 0x0600007F) == 0x02000047) { R4_TYPE(FMSUB_D);  }
  if ((code & 0x0600007F) == 0x0200004B) { R4_TYPE(FNMSUB_D); }
  if ((code & 0x0600007F) == 0x0200004F) { R4_TYPE(FNMADD_D); }
  if ((code & 0xFE00007F) == 0x02000053) { R_TYPE(FADD_D);    }
  if ((code & 0xFE00007F) == 0x0A000053) { R_TYPE(FSUB_D);    }
  if ((code & 0xFE00007F) == 0x12000053) { R_TYPE(FMUL_D);    }
  if ((code & 0xFE00007F) == 0x1A000053) { R_TYPE(FDIV_D);    }
  if ((code & 0xFFF0007F) == 0x5A000053) { R_TYPE(FSQRT_D);   }
  if ((code & 0xFE00707F) == 0x22000053) { R_TYPE(FSGNJ_D);   }
  if ((code & 0xFE00707F) == 0x22001053) { R_TYPE(FSGNJN_D);  }
  if ((code & 0xFE00707F) == 0x22002053) { R_TYPE(FSGNJX_D);  }
  if ((code & 0xFE00707F) == 0x2A000053) { R_TYPE(FMIN_D);    }
  if ((code & 0xFE00707F) == 0x2A001053) { R_TYPE(FMAX_D);    }
  if ((code & 0xFFF0007F) == 0x40100053) { R_TYPE(FCVT_S_D);  }
  if ((code & 0xFFF0007F) == 0x42000053) { R_TYPE(FCVT_D_S);  }
  if ((code & 0xFE00707F) == 0xA2002053) { R_TYPE(FEQ_D);     }
  if ((code & 0xFE00707F) == 0xA2001053) { R_TYPE(FLT_D);     }
  if ((code & 0xFE00707F) == 0xA2000053) { R_TYPE(FLE_D);     }
  if ((code & 0xFFF0707F) == 0xE2001053) { R_TYPE(FCLASS_D);  }
  if ((code & 0xFFF0007F) == 0xC2000053) { R_TYPE(FCVT_W_D);  }
  if ((code & 0xFFF0007F) == 0xC2100053) { R_TYPE(FCVT_WU_D); }
  if ((code & 0xFFF0007F) == 0xD2000053) { R_TYPE(FCVT_D_W);  }
  if ((code & 0xFFF0007F) == 0xD2100053) { R_TYPE(FCVT_D_WU); }



  //          88888888ba   8b           d8   ad8888ba,          ,d8    88888888ba,
  //          88      "8b  `8b         d8'  8P'    "Y8        ,d888    88      `"8b
  //          88      ,8P   `8b       d8'  d8               ,d8" 88    88        `8b
  //          88aaaaaa8P'    `8b     d8'   88,dd888bb,    ,d8"   88    88         88
  //          88""""88'       `8b   d8'    88P'    `8b  ,d8"     88    88         88
  //          88    `8b        `8b d8'     88       d8  8888888888888  88         8P
  //          88     `8b        `888'      88a     a8P           88    88      .a8P
  //          88      `8b        `8'        "Y88888P"            88    88888888Y"'

  if ((code & 0xFFF0007F) == 0xC2200053) { R_TYPE(FCVT_L_D);  }
  if ((code & 0xFFF0007F) == 0xC2300053) { R_TYPE(FCVT_LU_D); }
  if ((code & 0xFFF0707F) == 0xE2000053) { R_TYPE(FMV_X_D);   }
  if ((code & 0xFFF0007F) == 0xD2200053) { R_TYPE(FCVT_D_L);  }
  if ((code & 0xFFF0007F) == 0xD2300053) { R_TYPE(FCVT_D_LU); }
  if ((code & 0xFFF0707F) == 0xF2000053) { R_TYPE(FMV_D_X);   }



  //          88888888ba   8b           d8   ad888888b,   ad888888b,    ,ad8888ba,
  //          88      "8b  `8b         d8'  d8"     "88  d8"     "88   d8"'    `"8b
  //          88      ,8P   `8b       d8'           a8P          a8P  d8'        `8b
  //          88aaaaaa8P'    `8b     d8'         aad8"        ,d8P"   88          88
  //          88""""88'       `8b   d8'          ""Y8,      a8P"      88          88
  //          88    `8b        `8b d8'              "8b   a8P'        Y8,    "88,,8P
  //          88     `8b        `888'       Y8,     a88  d8"           Y8a.    Y88P
  //          88      `8b        `8'         "Y888888P'  88888888888    `"Y8888Y"Y8a

  if ((code & 0x0000707F) == 0x00004007) { R_TYPE(FLQ);       }
  if ((code & 0x0000707F) == 0x00004027) { S_TYPE(FSQ);       }
  if ((code & 0x0600007F) == 0x06000043) { R4_TYPE(FMADD_Q);  }
  if ((code & 0x0600007F) == 0x06000047) { R4_TYPE(FMSUB_Q);  }
  if ((code & 0x0600007F) == 0x0600004B) { R4_TYPE(FNMSUB_Q); }
  if ((code & 0x0600007F) == 0x0600004F) { R4_TYPE(FNMADD_Q); }
  if ((code & 0xFE00007F) == 0x06000053) { R_TYPE(FADD_Q);    }
  if ((code & 0xFE00007F) == 0x0E000053) { R_TYPE(FSUB_Q);    }
  if ((code & 0xFE00007F) == 0x16000053) { R_TYPE(FMUL_Q);    }
  if ((code & 0xFE00007F) == 0x1E000053) { R_TYPE(FDIV_Q);    }
  if ((code & 0xFFF0007F) == 0x5E000053) { R_TYPE(FSQRT_Q);   }
  if ((code & 0xFE00707F) == 0x26000053) { R_TYPE(FSGNJ_Q);   }
  if ((code & 0xFE00707F) == 0x26001053) { R_TYPE(FSGNJN_Q);  }
  if ((code & 0xFE00707F) == 0x26002053) { R_TYPE(FSGNJX_Q);  }
  if ((code & 0xFE00707F) == 0x2E000053) { R_TYPE(FMIN_Q);    }
  if ((code & 0xFE00707F) == 0x2E001053) { R_TYPE(FMAX_Q);    }
  if ((code & 0xFFF0007F) == 0x40300053) { R_TYPE(FCVT_S_Q);  }
  if ((code & 0xFFF0007F) == 0x46000053) { R_TYPE(FCVT_Q_S);  }
  if ((code & 0xFFF0007F) == 0x42300053) { R_TYPE(FCVT_D_Q);  }
  if ((code & 0xFFF0007F) == 0x46100053) { R_TYPE(FCVT_Q_D);  }
  if ((code & 0xFE00707F) == 0xA6002053) { R_TYPE(FEQ_Q);     }
  if ((code & 0xFE00707F) == 0xA6001053) { R_TYPE(FLT_Q);     }
  if ((code & 0xFE00707F) == 0xA6000053) { R_TYPE(FLE_Q);     }
  if ((code & 0xFFF0707F) == 0xE6001053) { R_TYPE(FCLASS_Q);  }
  if ((code & 0xFFF0007F) == 0xC6000053) { R_TYPE(FCVT_W_Q);  }
  if ((code & 0xFFF0007F) == 0xC6100053) { R_TYPE(FCVT_WU_Q); }
  if ((code & 0xFFF0007F) == 0xD6000053) { R_TYPE(FCVT_Q_W);  }
  if ((code & 0xFFF0007F) == 0xD6100053) { R_TYPE(FCVT_Q_WU); }



  //          88888888ba   8b           d8   ad8888ba,          ,d8      ,ad8888ba,
  //          88      "8b  `8b         d8'  8P'    "Y8        ,d888     d8"'    `"8b
  //          88      ,8P   `8b       d8'  d8               ,d8" 88    d8'        `8b
  //          88aaaaaa8P'    `8b     d8'   88,dd888bb,    ,d8"   88    88          88
  //          88""""88'       `8b   d8'    88P'    `8b  ,d8"     88    88          88
  //          88    `8b        `8b d8'     88       d8  8888888888888  Y8,    "88,,8P
  //          88     `8b        `888'      88a     a8P           88     Y8a.    Y88P
  //          88      `8b        `8'        "Y88888P"            88      `"Y8888Y"Y8a

  if ((code & 0xFFF0007F) == 0xC6200053) { R_TYPE(FCVT_L_Q);  }
  if ((code & 0xFFF0007F) == 0xC6300053) { R_TYPE(FCVT_LU_Q); }
  if ((code & 0xFFF0007F) == 0xD6200053) { R_TYPE(FCVT_Q_L);  }
  if ((code & 0xFFF0007F) == 0xD6300053) { R_TYPE(FCVT_Q_LU); }



  //          88888888ba   8b           d8   ad888888b,   ad888888b,  888888888888   ad88  88
  //          88      "8b  `8b         d8'  d8"     "88  d8"     "88           ,88  d8"    88
  //          88      ,8P   `8b       d8'           a8P          a8P         ,88"   88     88
  //          88aaaaaa8P'    `8b     d8'         aad8"        ,d8P"        ,88"   MM88MMM  88,dPPYba,
  //          88""""88'       `8b   d8'          ""Y8,      a8P"         ,88"       88     88P'    "8a
  //          88    `8b        `8b d8'              "8b   a8P'         ,88"         88     88       88
  //          88     `8b        `888'       Y8,     a88  d8"          88"           88     88       88
  //          88      `8b        `8'         "Y888888P'  88888888888  888888888888  88     88       88

  if ((code & 0x0000707F) == 0x00001007) { R_TYPE(FLH);       }
  if ((code & 0x0000707F) == 0x00001027) { S_TYPE(FSH);       }
  if ((code & 0x0600007F) == 0x04000043) { R4_TYPE(FMADD_H);  }
  if ((code & 0x0600007F) == 0x04000047) { R4_TYPE(FMSUB_H);  }
  if ((code & 0x0600007F) == 0x0400004B) { R4_TYPE(FNMSUB_H); }
  if ((code & 0x0600007F) == 0x0400004F) { R4_TYPE(FNMADD_H); }
  if ((code & 0xFE00007F) == 0x04000053) { R_TYPE(FADD_H);    }
  if ((code & 0xFE00007F) == 0x0C000053) { R_TYPE(FSUB_H);    }
  if ((code & 0xFE00007F) == 0x14000053) { R_TYPE(FMUL_H);    }
  if ((code & 0xFE00007F) == 0x1C000053) { R_TYPE(FDIV_H);    }
  if ((code & 0xFFF0007F) == 0x5C000053) { R_TYPE(FSQRT_H);   }
  if ((code & 0xFE00707F) == 0x24000053) { R_TYPE(FSGNJ_H);   }
  if ((code & 0xFE00707F) == 0x24001053) { R_TYPE(FSGNJN_H);  }
  if ((code & 0xFE00707F) == 0x24002053) { R_TYPE(FSGNJX_H);  }
  if ((code & 0xFE00707F) == 0x2C000053) { R_TYPE(FMIN_H);    }
  if ((code & 0xFE00707F) == 0x2C001053) { R_TYPE(FMAX_H);    }
  if ((code & 0xFFF0007F) == 0x40200053) { R_TYPE(FCVT_S_H);  }
  if ((code & 0xFFF0007F) == 0x44000053) { R_TYPE(FCVT_H_S);  }
  if ((code & 0xFFF0007F) == 0x42200053) { R_TYPE(FCVT_D_H);  }
  if ((code & 0xFFF0007F) == 0x44100053) { R_TYPE(FCVT_H_D);  }
  if ((code & 0xFFF0007F) == 0x46200053) { R_TYPE(FCVT_Q_H);  }
  if ((code & 0xFFF0007F) == 0x44300053) { R_TYPE(FCVT_H_Q);  }
  if ((code & 0xFE00707F) == 0xA4002053) { R_TYPE(FEQ_H);     }
  if ((code & 0xFE00707F) == 0xA4001053) { R_TYPE(FLT_H);     }
  if ((code & 0xFE00707F) == 0xA4000053) { R_TYPE(FLE_H);     }
  if ((code & 0xFFF0707F) == 0xE4001053) { R_TYPE(FCLASS_H);  }
  if ((code & 0xFFF0007F) == 0xC4000053) { R_TYPE(FCVT_W_H);  }
  if ((code & 0xFFF0007F) == 0xC4100053) { R_TYPE(FCVT_WU_H); }
  if ((code & 0xFFF0707F) == 0xE4000053) { R_TYPE(FMV_X_H);   }
  if ((code & 0xFFF0007F) == 0xD4000053) { R_TYPE(FCVT_H_W);  }
  if ((code & 0xFFF0007F) == 0xD4100053) { R_TYPE(FCVT_H_WU); }
  if ((code & 0xFFF0707F) == 0xF4000053) { R_TYPE(FMV_H_X);   }



  //          88888888ba   8b           d8   ad8888ba,          ,d8   888888888888   ad88  88
  //          88      "8b  `8b         d8'  8P'    "Y8        ,d888            ,88  d8"    88
  //          88      ,8P   `8b       d8'  d8               ,d8" 88          ,88"   88     88
  //          88aaaaaa8P'    `8b     d8'   88,dd888bb,    ,d8"   88        ,88"   MM88MMM  88,dPPYba,
  //          88""""88'       `8b   d8'    88P'    `8b  ,d8"     88      ,88"       88     88P'    "8a
  //          88    `8b        `8b d8'     88       d8  8888888888888  ,88"         88     88       88
  //          88     `8b        `888'      88a     a8P           88   88"           88     88       88
  //          88      `8b        `8'        "Y88888P"            88   888888888888  88     88       88

  if ((code & 0xFFF0007F) == 0xC4200053) { R_TYPE(FCVT_L_H);  }
  if ((code & 0xFFF0007F) == 0xC4300053) { R_TYPE(FCVT_LU_H); }
  if ((code & 0xFFF0007F) == 0xD4200053) { R_TYPE(FCVT_H_L);  }
  if ((code & 0xFFF0007F) == 0xD4300053) { R_TYPE(FCVT_H_LU); }



  //          888888888888
  //                   ,88
  //                 ,88"
  //               ,88"     ,adPPYYba,  8b      db      d8  8b,dPPYba,  ,adPPYba,
  //             ,88"       ""     `Y8  `8b    d88b    d8'  88P'   "Y8  I8[    ""
  //           ,88"         ,adPPPPP88   `8b  d8'`8b  d8'   88           `"Y8ba,
  //          88"           88,    ,88    `8bd8'  `8bd8'    88          aa    ]8I
  //          888888888888  `"8bbdP"Y8      YP      YP      88          `"YbbdP"'

  if ((code & 0xFFFFFFFF) == 0x00D00073) { I_TYPE(WRS_NTO);   }
  if ((code & 0xFFFFFFFF) == 0x01D00073) { I_TYPE(WRS_STO);   }



  return result;

}

#endif
