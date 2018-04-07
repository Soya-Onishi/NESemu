#include"../cpu_circuit.h"
#include"../memory.h"
#include"../status_flag_manager.h"

#define RESET_VEC_LOWER 0xfffc
#define RESET_VEC_UPPER 0xfffd

#define NMI_VEC_LOWER 0xfffa
#define NMI_VEC_UPPER 0xfffb

#define IRQ_VEC_LOWER 0xfffe
#define IRQ_VEC_UPPER 0xffff

#define BRK_VEC_LOWER IRQ_VEC_LOWER
#define BRK_VEC_UPPER IRQ_VEC_UPPER

int reset_implied() {
  unsigned short new_pc;

  registers.status |= STATUS_I;
  new_pc = memory_read(RESET_VEC_LOWER);
  new_pc |= (unsigned short)memory_read(RESET_VEC_UPPER) << 8;

  registers.pc = new_pc;

  return 0;
}

int nmi_implied() {
  
  registers.status &= ~STATUS_B;
  memory_write(registers.stack--, (unsigned char)(registers.pc >> 8));
  memory_write(registers.stack--, (unsigned char)(registers.pc));
  memory_write(registers.stack--, registers.status);
  
  registers.status |= STATUS_I;
  registers.pc = memory_read(NMI_VEC_LOWER);
  registers.pc |= (unsigned short)memory_read(NMI_VEC_UPPER) << 8;

  return 0;
}

int irq_implied() {
  if(registers.status & STATUS_I) return 0;

  registers.status &= ~STATUS_B;
  memory_write(registers.stack--, (unsigned char)(registers.pc >> 8));
  memory_write(registers.stack--, (unsigned char)(registers.pc));
  memory_write(registers.stack--, registers.status);
  
  registers.status |= STATUS_I;
  registers.pc = memory_read(IRQ_VEC_LOWER);
  registers.pc |= (unsigned short)memory_read(IRQ_VEC_UPPER) << 8;

  return 0;
}

int brk_implied() {
  unsigned short pushed_pc;
  
  if(registers.status & STATUS_I) return 0;

  pushed_pc = registers.pc + 2;
  registers.status |= STATUS_B;

  memory_write(registers.stack--, (unsigned char)(pushed_pc >> 8));
  memory_write(registers.stack--, (unsigned char)(pushed_pc));
  memory_write(registers.stack--, registers.status);

  registers.status |= STATUS_I;

  registers.pc = memory_read(BRK_VEC_LOWER);
  registers.pc = (unsigned short)memory_read(BRK_VEC_LOWER) << 8;
  registers.pc -= 1; // 1 is byte length of BRK instruction

  return 0;
}

int rti_implied() {
  unsigned char pc_lower, pc_upper, status;

  status = memory_read(++registers.stack);
  pc_lower = memory_read(++registers.stack);
  pc_upper = memory_read(++registers.stack);

  registers.status = status;
  registers.pc = ((unsigned short)pc_upper << 8) + pc_lower;
  registers.pc -= 1; // 1 is byte length of RTI instruction

  return 0;
}