#include"../cpu_circuit.h"
#include"../memory.h"
#include"../status_flag_manager.h"
#include"../ppu.h"

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

  intr_flags.reset = 0;

  registers.status |= STATUS_I;
  registers.stack -= 3;
  reset_ppu();
  
  ppu_cycle();

  ppu_cycle();
  ppu_cycle();
  ppu_cycle();

  new_pc = memory_read(RESET_VEC_LOWER);
  ppu_cycle();

  new_pc |= (unsigned short)memory_read(RESET_VEC_UPPER) << 8;
  ppu_cycle();

  registers.pc = new_pc;

  return 0;
}

int nmi_implied() {

  intr_flags.nmi = 0;
  return 0;
  
  registers.status &= ~STATUS_B;
  ppu_cycle();

  memory_write(registers.stack--, (unsigned char)(registers.pc >> 8));
  ppu_cycle();

  memory_write(registers.stack--, (unsigned char)(registers.pc));
  ppu_cycle();

  memory_write(registers.stack--, registers.status);
  ppu_cycle();

  registers.status |= STATUS_I;
  registers.pc = memory_read(NMI_VEC_LOWER);
  ppu_cycle();

  registers.pc |= (unsigned short)memory_read(NMI_VEC_UPPER) << 8;
  ppu_cycle();

  return 0;
}

int irq_implied() {
  intr_flags.irq = 0;

  if(registers.status & STATUS_I) {
    int i;

    for(i = 0; i < 7; i++) {
      ppu_cycle();
    }

    return 0;
  }

  registers.status &= ~STATUS_B;
  ppu_cycle();

  memory_write(registers.stack--, (unsigned char)(registers.pc >> 8));
  ppu_cycle();

  memory_write(registers.stack--, (unsigned char)(registers.pc));
  ppu_cycle();

  memory_write(registers.stack--, registers.status);
  ppu_cycle();

  registers.status |= STATUS_I;
  registers.pc = memory_read(IRQ_VEC_LOWER);
  ppu_cycle();
  registers.pc |= (unsigned short)memory_read(IRQ_VEC_UPPER) << 8;
  ppu_cycle();

  return 0;
}

int brk_implied() {
  unsigned short pushed_pc;
  
  if(registers.status & STATUS_I) {
    int i;

    for(i = 0; i < 7; i++) {
      ppu_cycle();
    }

    return 0;
  }

  pushed_pc = registers.pc + 2;
  registers.status |= STATUS_B;
  ppu_cycle();

  memory_write(registers.stack--, (unsigned char)(pushed_pc >> 8));
  ppu_cycle();

  memory_write(registers.stack--, (unsigned char)(pushed_pc));
  ppu_cycle();

  memory_write(registers.stack--, registers.status);
  ppu_cycle();

  registers.status |= STATUS_I;

  registers.pc = memory_read(BRK_VEC_LOWER);
  ppu_cycle();

  registers.pc = (unsigned short)memory_read(BRK_VEC_LOWER) << 8;
  ppu_cycle();

  registers.pc -= 1; // 1 is byte length of BRK instruction

  return 0;
}

int rti_implied() {
  unsigned char pc_lower, pc_upper, status;

  ppu_cycle();
  ppu_cycle();
  status = memory_read(++registers.stack);
  ppu_cycle();

  pc_lower = memory_read(++registers.stack);
  ppu_cycle();

  pc_upper = memory_read(++registers.stack);
  ppu_cycle();

  registers.status = status;
  registers.pc = ((unsigned short)pc_upper << 8) + pc_lower;
  registers.pc -= 1; // 1 is byte length of RTI instruction

  return 0;
}