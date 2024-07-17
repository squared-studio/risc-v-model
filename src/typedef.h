#ifndef TYPEDEF_H__
#define TYPEDEF_H__

#include "stdint.h"

typedef struct {
  char*    func     ;
  uint32_t rd    :5 ;
  uint32_t rs1   :5 ;
  uint32_t rs2   :5 ;
  uint32_t rs3   :5 ;
  uint32_t imm   :32;
  uint32_t shamt :5 ;
  uint32_t succ  :4 ;
  uint32_t pred  :4 ;
  uint32_t fm    :4 ;
  uint32_t csr   :12;
  uint32_t uimm  :32;
  uint32_t rl    :1 ;
  uint32_t aq    :1 ;
  uint32_t rm    :3 ;
} decoded_instr_t;

#endif
