#include "typedef.h"
#include "stdio.h"
#include "general.c"

#ifndef DECODER_C__
#define DECODER_C__

#define ___ 0
#define INT 1
#define FLT 2

decoded_instr_t decode (uint32_t code) {

  uint32_t temp;
  uint32_t aimm; // Shift amount
  uint32_t bimm; // B-type instr immediate
  uint32_t cimm; // CSR instr {immediate:6,CSR:12}
  uint32_t iimm; // I-type instr immediate
  uint32_t jimm; // J-type instr immediate
  uint32_t simm; // S-type instr immediate
  uint32_t uimm; // U-type instr immediate
  uint32_t timm; // atomic instr
  uint32_t rimm; // round mode float instr

  aimm = bit_select(code, 25, 20);

  temp = 0;
  temp = temp | (bit_select(code,  7,  7) << 11);
  temp = temp | (bit_select(code, 11,  8) <<  1);
  temp = temp | (bit_select(code, 30, 25) <<  5);
  temp = temp | (bit_select(code, 31, 31) << 12);
  bimm = sign_ext(temp, 13);

  temp = 0;
  temp = temp | (bit_select(code, 19, 15) << 12);
  temp = temp | (bit_select(code, 31, 20) << 0);
  cimm = temp;

  iimm = sign_ext(bit_select(code, 31, 20), 12);

  temp = 0;
  temp = temp | (bit_select(code, 19, 12) << 12);
  temp = temp | (bit_select(code, 20, 20) << 11);
  temp = temp | (bit_select(code, 30, 21) <<  1);
  temp = temp | (bit_select(code, 31, 31) << 20);
  jimm = sign_ext(temp, 21);

  rimm = bit_select(code, 14, 12);

  temp = 0;
  temp = temp | (bit_select(code, 11,  7) << 0);
  temp = temp | (bit_select(code, 31, 25) << 5);
  simm  = sign_ext(temp, 12);

  temp = 0;
  temp = temp | (bit_select(code, 25, 25) << 0);  // rl bit
  temp = temp | (bit_select(code, 26, 26) << 1);  // aq bit
  timm = temp;  // {aq,rl}

  uimm = sign_ext(code & 0xFFFFF000, 32);

  decoded_instr_t op;
  op.func  = INVALID;
  op.rd    = bit_select(code, 11, 7);
  op.rs1   = bit_select(code, 19, 15);
  op.rs2   = bit_select(code, 24, 20);
  op.rs3   = bit_select(code, 31, 27);
  op.rl    = bit_select(code, 25, 25);
  op.aq    = bit_select(code, 26, 26);
  op.rm    = bit_select(code, 14, 12);

  // 0 none
  // 1 int
  // 2 float
  int rd;
  int r1;
  int r2;
  int r3;

  if ((code & 0x0000007F) == 0x00000037) { op.func = LUI;       op.imm = uimm;      rd = INT;     r1 = ___;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000007F) == 0x00000017) { op.func = AUIPC;     op.imm = uimm;      rd = INT;     r1 = ___;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000007F) == 0x0000006F) { op.func = JAL;       op.imm = jimm;      rd = INT;     r1 = ___;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00000067) { op.func = JALR;      op.imm = iimm;      rd = INT;     r1 = ___;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00000063) { op.func = BEQ;       op.imm = bimm;      rd = ___;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00001063) { op.func = BNE;       op.imm = bimm;      rd = ___;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00004063) { op.func = BLT;       op.imm = bimm;      rd = ___;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00005063) { op.func = BGE;       op.imm = bimm;      rd = ___;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00006063) { op.func = BLTU;      op.imm = bimm;      rd = ___;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00007063) { op.func = BGEU;      op.imm = bimm;      rd = ___;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00000003) { op.func = LB;        op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00001003) { op.func = LH;        op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00002003) { op.func = LW;        op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00004003) { op.func = LBU;       op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00005003) { op.func = LHU;       op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00000023) { op.func = SB;        op.imm = simm;      rd = ___;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00001023) { op.func = SH;        op.imm = simm;      rd = ___;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00002023) { op.func = SW;        op.imm = simm;      rd = ___;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00000013) { op.func = ADDI;      op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00002013) { op.func = SLTI;      op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00003013) { op.func = SLTIU;     op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00004013) { op.func = XORI;      op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00006013) { op.func = ORI;       op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00007013) { op.func = ANDI;      op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x00001013) { op.func = SLLI;      op.imm =    0;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x00005013) { op.func = SRLI;      op.imm =    0;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x40005013) { op.func = SRAI;      op.imm =    0;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x00000033) { op.func = ADD;       op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x40000033) { op.func = SUB;       op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x00001033) { op.func = SLL;       op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x00002033) { op.func = SLT;       op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x00003033) { op.func = SLTU;      op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x00004033) { op.func = XOR;       op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x00005033) { op.func = SRL;       op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x40005033) { op.func = SRA;       op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x00006033) { op.func = OR;        op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x00007033) { op.func = AND;       op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0x0000707F) == 0x0000000F) { op.func = FENCE;     op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFFFFFFF) == 0x8330000F) { op.func = FENCE_TSO; op.imm = iimm;      rd = ___;     r1 = ___;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFFFFFFF) == 0x0100000F) { op.func = PAUSE;     op.imm = iimm;      rd = ___;     r1 = ___;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFFFFFFF) == 0x00000073) { op.func = ECALL;     op.imm = iimm;      rd = ___;     r1 = ___;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFFFFFFF) == 0x00100073) { op.func = EBREAK;    op.imm = iimm;      rd = ___;     r1 = ___;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00006003) { op.func = LWU;       op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00003003) { op.func = LD;        op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00003023) { op.func = SD;        op.imm = simm;      rd = ___;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFC00707F) == 0x00001013) { op.func = SLLI;      op.imm = aimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFC00707F) == 0x00005013) { op.func = SRLI;      op.imm = aimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFC00707F) == 0x40005013) { op.func = SRAI;      op.imm = aimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x0000001B) { op.func = ADDIW;     op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x0000101B) { op.func = SLLIW;     op.imm = aimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x0000501B) { op.func = SRLIW;     op.imm = aimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x4000501B) { op.func = SRAIW;     op.imm = aimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x0000003B) { op.func = ADDW;      op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x4000003B) { op.func = SUBW;      op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x0000103B) { op.func = SLLW;      op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x0000503B) { op.func = SRLW;      op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x4000503B) { op.func = SRAW;      op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0x0000707F) == 0x0000100F) { op.func = FENCE_I;   op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00001073) { op.func = CSRRW;     op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00002073) { op.func = CSRRS;     op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00003073) { op.func = CSRRC;     op.imm = iimm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00005073) { op.func = CSRRWI;    op.imm = iimm;      rd = INT;     r1 = ___;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00006073) { op.func = CSRRSI;    op.imm = iimm;      rd = INT;     r1 = ___;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00007073) { op.func = CSRRCI;    op.imm = iimm;      rd = INT;     r1 = ___;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x02000033) { op.func = MUL;       op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x02001033) { op.func = MULH;      op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x02002033) { op.func = MULHSU;    op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x02003033) { op.func = MULHU;     op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x02004033) { op.func = DIV;       op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x02005033) { op.func = DIVU;      op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x02006033) { op.func = REM;       op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x02007033) { op.func = REMU;      op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x0200003B) { op.func = MULW;      op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x0200403B) { op.func = DIVW;      op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x0200503B) { op.func = DIVUW;     op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x0200603B) { op.func = REMW;      op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x0200703B) { op.func = REMUW;     op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF9F0707F) == 0x1000202F) { op.func = LR_W;      op.imm =    0;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xF800707F) == 0x1800202F) { op.func = SC_W;      op.imm =    0;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0x0800202F) { op.func = AMOSWAP_W; op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0x0000202F) { op.func = AMOADD_W;  op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0x2000202F) { op.func = AMOXOR_W;  op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0x6000202F) { op.func = AMOAND_W;  op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0x4000202F) { op.func = AMOOR_W;   op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0x8000202F) { op.func = AMOMIN_W;  op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0xA000202F) { op.func = AMOMAX_W;  op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0xC000202F) { op.func = AMOMINU_W; op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0xE000202F) { op.func = AMOMAXU_W; op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF9F0707F) == 0x1000302F) { op.func = LR_D;      op.imm = timm;      rd = INT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xF800707F) == 0x1800302F) { op.func = SC_D;      op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0x0800302F) { op.func = AMOSWAP_D; op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0x0000302F) { op.func = AMOADD_D;  op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0x2000302F) { op.func = AMOXOR_D;  op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0x6000302F) { op.func = AMOAND_D;  op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0x4000302F) { op.func = AMOOR_D;   op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0x8000302F) { op.func = AMOMIN_D;  op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0xA000302F) { op.func = AMOMAX_D;  op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0xC000302F) { op.func = AMOMINU_D; op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0xF800707F) == 0xE000302F) { op.func = AMOMAXU_D; op.imm = timm;      rd = INT;     r1 = INT;     r2 = INT;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00002007) { op.func = FLW;       op.imm = iimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00002027) { op.func = FSW;       op.imm = simm;      rd = ___;     r1 = INT;     r2 = FLT;     r3 = ___;}
  if ((code & 0x0600007F) == 0x00000043) { op.func = FMADD_S;   op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = FLT;}
  if ((code & 0x0600007F) == 0x00000047) { op.func = FMSUB_S;   op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = FLT;}
  if ((code & 0x0600007F) == 0x0000004B) { op.func = FNMSUB_S;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = FLT;}
  if ((code & 0x0600007F) == 0x0000004F) { op.func = FNMADD_S;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = FLT;}
  if ((code & 0xFE00007F) == 0x00000053) { op.func = FADD_S;    op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00007F) == 0x08000053) { op.func = FSUB_S;    op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00007F) == 0x10000053) { op.func = FMUL_S;    op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00007F) == 0x18000053) { op.func = FDIV_S;    op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0x58000053) { op.func = FSQRT_S;   op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x20000053) { op.func = FSGNJ_S;   op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x20001053) { op.func = FSGNJN_S;  op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x20002053) { op.func = FSGNJX_S;  op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x28000053) { op.func = FMIN_S;    op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x28001053) { op.func = FMAX_S;    op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xC0000053) { op.func = FCVT_W_S;  op.imm = rimm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xC0100053) { op.func = FCVT_WU_S; op.imm = rimm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0707F) == 0xE0000053) { op.func = FMV_X_W;   op.imm =    0;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0xA0002053) { op.func = FEQ_S;     op.imm =    0;      rd = INT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0xA0001053) { op.func = FLT_S;     op.imm =    0;      rd = INT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0xA0000053) { op.func = FLE_S;     op.imm =    0;      rd = INT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFFF0707F) == 0xE0001053) { op.func = FCLASS_S;  op.imm =    0;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xD0000053) { op.func = FCVT_S_W;  op.imm = rimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xD0100053) { op.func = FCVT_S_WU; op.imm = rimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0707F) == 0xF0000053) { op.func = FMV_W_X;   op.imm =    0;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xC0200053) { op.func = FCVT_L_S;  op.imm = rimm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xC0300053) { op.func = FCVT_LU_S; op.imm = rimm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xD0200053) { op.func = FCVT_S_L;  op.imm = rimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xD0300053) { op.func = FCVT_S_LU; op.imm = rimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00003007) { op.func = FLD;       op.imm = iimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00003027) { op.func = FSD;       op.imm = simm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0600007F) == 0x02000043) { op.func = FMADD_D;   op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = FLT;}
  if ((code & 0x0600007F) == 0x02000047) { op.func = FMSUB_D;   op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = FLT;}
  if ((code & 0x0600007F) == 0x0200004B) { op.func = FNMSUB_D;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = FLT;}
  if ((code & 0x0600007F) == 0x0200004F) { op.func = FNMADD_D;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = FLT;}
  if ((code & 0xFE00007F) == 0x02000053) { op.func = FADD_D;    op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00007F) == 0x0A000053) { op.func = FSUB_D;    op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00007F) == 0x12000053) { op.func = FMUL_D;    op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00007F) == 0x1A000053) { op.func = FDIV_D;    op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0x5A000053) { op.func = FSQRT_D;   op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x22000053) { op.func = FSGNJ_D;   op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x22001053) { op.func = FSGNJN_D;  op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x22002053) { op.func = FSGNJX_D;  op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x2A000053) { op.func = FMIN_D;    op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x2A001053) { op.func = FMAX_D;    op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0x40100053) { op.func = FCVT_S_D;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0x42000053) { op.func = FCVT_D_S;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0xA2002053) { op.func = FEQ_D;     op.imm =    0;      rd = INT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0xA2001053) { op.func = FLT_D;     op.imm =    0;      rd = INT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0xA2000053) { op.func = FLE_D;     op.imm =    0;      rd = INT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFFF0707F) == 0xE2001053) { op.func = FCLASS_D;  op.imm =    0;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xC2000053) { op.func = FCVT_W_D;  op.imm = rimm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xC2100053) { op.func = FCVT_WU_D; op.imm = rimm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xD2000053) { op.func = FCVT_D_W;  op.imm = rimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xD2100053) { op.func = FCVT_D_WU; op.imm = rimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xC2200053) { op.func = FCVT_L_D;  op.imm = rimm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xC2300053) { op.func = FCVT_LU_D; op.imm = rimm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0707F) == 0xE2000053) { op.func = FMV_X_D;   op.imm =    0;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xD2200053) { op.func = FCVT_D_L;  op.imm = rimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xD2300053) { op.func = FCVT_D_LU; op.imm = rimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0707F) == 0xF2000053) { op.func = FMV_D_X;   op.imm =    0;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00004007) { op.func = FLQ;       op.imm = iimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00004027) { op.func = FSQ;       op.imm = simm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0600007F) == 0x06000043) { op.func = FMADD_Q;   op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = FLT;}
  if ((code & 0x0600007F) == 0x06000047) { op.func = FMSUB_Q;   op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = FLT;}
  if ((code & 0x0600007F) == 0x0600004B) { op.func = FNMSUB_Q;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = FLT;}
  if ((code & 0x0600007F) == 0x0600004F) { op.func = FNMADD_Q;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = FLT;}
  if ((code & 0xFE00007F) == 0x06000053) { op.func = FADD_Q;    op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00007F) == 0x0E000053) { op.func = FSUB_Q;    op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00007F) == 0x16000053) { op.func = FMUL_Q;    op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00007F) == 0x1E000053) { op.func = FDIV_Q;    op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0x5E000053) { op.func = FSQRT_Q;   op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x26000053) { op.func = FSGNJ_Q;   op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x26001053) { op.func = FSGNJN_Q;  op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x26002053) { op.func = FSGNJX_Q;  op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x2E000053) { op.func = FMIN_Q;    op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x2E001053) { op.func = FMAX_Q;    op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0x40300053) { op.func = FCVT_S_Q;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0x46000053) { op.func = FCVT_Q_S;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0x42300053) { op.func = FCVT_D_Q;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0x46100053) { op.func = FCVT_Q_D;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0xA6002053) { op.func = FEQ_Q;     op.imm =    0;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0xA6001053) { op.func = FLT_Q;     op.imm =    0;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0xA6000053) { op.func = FLE_Q;     op.imm =    0;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0707F) == 0xE6001053) { op.func = FCLASS_Q;  op.imm =    0;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xC6000053) { op.func = FCVT_W_Q;  op.imm = rimm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xC6100053) { op.func = FCVT_WU_Q; op.imm = rimm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xD6000053) { op.func = FCVT_Q_W;  op.imm = rimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xD6100053) { op.func = FCVT_Q_WU; op.imm = rimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xC6200053) { op.func = FCVT_L_Q;  op.imm = rimm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xC6300053) { op.func = FCVT_LU_Q; op.imm = rimm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xD6200053) { op.func = FCVT_Q_L;  op.imm = rimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xD6300053) { op.func = FCVT_Q_LU; op.imm = rimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00001007) { op.func = FLH;       op.imm = iimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0000707F) == 0x00001027) { op.func = FSH;       op.imm = simm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0x0600007F) == 0x04000043) { op.func = FMADD_H;   op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = FLT;}
  if ((code & 0x0600007F) == 0x04000047) { op.func = FMSUB_H;   op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = FLT;}
  if ((code & 0x0600007F) == 0x0400004B) { op.func = FNMSUB_H;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = FLT;}
  if ((code & 0x0600007F) == 0x0400004F) { op.func = FNMADD_H;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = FLT;}
  if ((code & 0xFE00007F) == 0x04000053) { op.func = FADD_H;    op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00007F) == 0x0C000053) { op.func = FSUB_H;    op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00007F) == 0x14000053) { op.func = FMUL_H;    op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00007F) == 0x1C000053) { op.func = FDIV_H;    op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0x5C000053) { op.func = FSQRT_H;   op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x24000053) { op.func = FSGNJ_H;   op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x24001053) { op.func = FSGNJN_H;  op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x24002053) { op.func = FSGNJX_H;  op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x2C000053) { op.func = FMIN_H;    op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFE00707F) == 0x2C001053) { op.func = FMAX_H;    op.imm =    0;      rd = FLT;     r1 = FLT;     r2 = FLT;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0x40200053) { op.func = FCVT_S_H;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0x44000053) { op.func = FCVT_H_S;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0x42200053) { op.func = FCVT_D_H;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0x44100053) { op.func = FCVT_H_D;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0x46200053) { op.func = FCVT_Q_H;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0x44300053) { op.func = FCVT_H_Q;  op.imm = rimm;      rd = FLT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0xA4002053) { op.func = FEQ_H;     op.imm =    0;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0xA4001053) { op.func = FLT_H;     op.imm =    0;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFE00707F) == 0xA4000053) { op.func = FLE_H;     op.imm =    0;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0707F) == 0xE4001053) { op.func = FCLASS_H;  op.imm =    0;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xC4000053) { op.func = FCVT_W_H;  op.imm = rimm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xC4100053) { op.func = FCVT_WU_H; op.imm = rimm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0707F) == 0xE4000053) { op.func = FMV_X_H;   op.imm =    0;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xD4000053) { op.func = FCVT_H_W;  op.imm = rimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xD4100053) { op.func = FCVT_H_WU; op.imm = rimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0707F) == 0xF4000053) { op.func = FMV_H_X;   op.imm =    0;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xC4200053) { op.func = FCVT_L_H;  op.imm = rimm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xC4300053) { op.func = FCVT_LU_H; op.imm = rimm;      rd = INT;     r1 = FLT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xD4200053) { op.func = FCVT_H_L;  op.imm = rimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFF0007F) == 0xD4300053) { op.func = FCVT_H_LU; op.imm = rimm;      rd = FLT;     r1 = INT;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFFFFFFF) == 0x00D00073) { op.func = WRS_NTO;   op.imm = iimm;      rd = ___;     r1 = ___;     r2 = ___;     r3 = ___;}
  if ((code & 0xFFFFFFFF) == 0x01D00073) { op.func = WRS_STO;   op.imm = iimm;      rd = ___;     r1 = ___;     r2 = ___;     r3 = ___;}

  if (rd == 0) op.rd  = 0; else if (rd == 2) op.rd  = op.rd  + 32;
  if (r1 == 0) op.rs1 = 0; else if (r1 == 2) op.rs1 = op.rs1 + 32;
  if (r2 == 0) op.rs2 = 0; else if (r2 == 2) op.rs2 = op.rs2 + 32;
  if (r3 == 0) op.rs3 = 0; else if (r3 == 2) op.rs3 = op.rs3 + 32;

  return op;

}

#endif
