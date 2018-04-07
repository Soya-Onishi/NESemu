#include"../cpu_circuit.h"
#include"../memory.h"
#include"../status_flag_manager.h"

void exec_lda(unsigned short addr);
void exec_ldx(unsigned short addr);
void exec_ldy(unsigned short addr);
void exec_sta(unsigned short addr);
void exec_stx(unsigned short addr);
void exec_sty(unsigned short addr);

void transfer_immediate(void (*exec_trans)(unsigned short));
void transfer_zeropage(void (*exec_trans)(unsigned short));
void transfer_zeropage_index(void (*exec_trans)(unsigned short), unsigned char index);
void transfer_absolute(void (*exec_trans)(unsigned short));
int transfer_absolute_index(void (*exec_trans)(unsigned short), unsigned char index);
void transfer_indirect_x(void (*exec_trans)(unsigned short));
int transfer_indirect_y(void (*exec_trans)(unsigned short));

void exec_lda(unsigned short addr) {
  registers.accumulator = memory_read(addr);
  set_z_flag(registers.accumulator);
  set_n_flag(registers.accumulator);
}

void exec_ldx(unsigned short addr) {
  registers.index_x = memory_read(addr);
  set_z_flag(registers.index_x);
  set_n_flag(registers.index_x);
}

void exec_ldy(unsigned short addr) {
  registers.index_y = memory_read(addr);
  set_z_flag(registers.index_y);
  set_n_flag(registers.index_y);
}

void exec_sta(unsigned short addr) {
  memory_write(addr, registers.accumulator);
}

void exec_stx(unsigned short addr) {
  memory_write(addr, registers.index_x);
}

void exec_sty(unsigned short addr) {
  memory_write(addr, registers.index_y);
}

void transfer_immediate(void (*exec_trans)(unsigned short)) {
  exec_trans((unsigned short)registers.pc + 1);
}

int lda_immediate() {
  transfer_immediate(exec_lda);
  return 0;
}

int ldx_immediate() {
  transfer_immediate(exec_ldx);
  return 0;
}

int ldy_immediate() {
  transfer_immediate(exec_ldy);
  return 0;
}

void transfer_zeropage(void (*exec_trans)(unsigned short)) {
  exec_trans((unsigned short)memory_read(registers.pc + 1));
}

int lda_zeropage() {
  transfer_zeropage(exec_lda);
  return 0;
}

int ldx_zeropage() {
  transfer_zeropage(exec_ldx);
  return 0;
}

int ldy_zeropage() {
  transfer_zeropage(exec_ldy);
  return 0;
}

int sta_zeropage() {
  transfer_zeropage(exec_sta);
  return 0;
}

int stx_zeropage() {
  transfer_zeropage(exec_stx);
  return 0;
}

int sty_zeropage() {
  transfer_zeropage(exec_sty);
  return 0;
}

void transfer_zeropage_index(void (*exec_trans)(unsigned short), unsigned char index) {
  unsigned char addr;

  addr = memory_read(registers.pc + 1) + index;
  exec_trans((unsigned short)addr);
}

int lda_zeropage_index() {
  transfer_zeropage_index(exec_lda, registers.index_x);
  return 0;
}

int ldx_zeropage_index() {
  transfer_zeropage_index(exec_ldx, registers.index_y);
  return 0;
}

int ldy_zeropage_index() {
  transfer_zeropage_index(exec_ldy, registers.index_x);
  return 0;
}

int sta_zeropage_index() {
  transfer_zeropage_index(exec_sta, registers.index_x);
  return 0;
}

int stx_zeropage_index() {
  transfer_zeropage_index(exec_stx, registers.index_y);
  return 0;
}

int sty_zeropage_index() {
  transfer_zeropage_index(exec_sty, registers.index_x);
  return 0;
}

void transfer_absolute(void (*exec_trans)(unsigned short)) {
  unsigned short addr;

  addr = memory_read(registers.pc + 1);
  addr |= (unsigned short)memory_read(registers.pc + 2) << 8;

  exec_trans(addr);
}

int lda_absolute() {
  transfer_absolute(exec_lda);
  return 0;
}

int ldx_absolute() {
  transfer_absolute(exec_ldx);
  return 0;
}

int ldy_absolute() {
  transfer_absolute(exec_ldy);
  return 0;
}

int sta_absolute() {
  transfer_absolute(exec_sta);
  return 0;
}

int stx_absolute() {
  transfer_absolute(exec_stx);
  return 0;
}

int sty_absolute() {
  transfer_absolute(exec_sty);
  return 0;
}

int transfer_absolute_index(void (*exec_trans)(unsigned short), unsigned char index) {
  unsigned short addr, before;
  int additional_cycle = 0;

  addr = memory_read(registers.pc + 1);
  addr |= (unsigned short)memory_read(registers.pc + 2) << 8;
  
  before = addr;
  addr += index;
  if((before & 0xf0) != (addr & 0xf0)) {
    additional_cycle++;
  }

  exec_trans(addr);

  return additional_cycle;
}

int lda_absolute_index_x() {
  return transfer_absolute_index(exec_lda, registers.index_x);
}

int lda_absolute_index_y() {
  return transfer_absolute_index(exec_lda, registers.index_y);
}

int ldx_absolute_index_y() {
  return transfer_absolute_index(exec_ldx, registers.index_y);
}

int ldy_absolute_index_x() {
  return transfer_absolute_index(exec_ldy, registers.index_x);
}

int sta_absolute_index_x() {
  return transfer_absolute_index(exec_sta, registers.index_x);
}

int sta_absolute_index_y() {
  return transfer_absolute_index(exec_sta, registers.index_y);
}

void transfer_indirect_x(void (*exec_trans)(unsigned short)) {
  unsigned char addr;
  unsigned short exec_addr;

  addr = memory_read(registers.pc + 1) + registers.index_x;
  exec_addr = (unsigned short)memory_read(addr) << 8;
  addr++;
  exec_addr |= memory_read(addr);
  exec_trans(exec_addr);
}

int lda_indirect_x() {
  transfer_indirect_x(exec_lda);
  return 0;
}

int sta_indirect_x() {
  transfer_indirect_x(exec_sta);
  return 0;
}

int transfer_indirect_y(void (*exec_trans)(unsigned short)) {
  unsigned char addr;
  unsigned short exec_addr, before;
  int addtional_cycle = 0;

  addr = memory_read(registers.pc + 1);
  exec_addr = (unsigned short)memory_read(addr) << 8;
  addr++;
  exec_addr |= memory_read(addr);

  before = exec_addr;
  exec_addr += registers.index_y;

  if((before & 0xf0) != (exec_addr & 0xf0)) {
    addtional_cycle++;
  }

  exec_trans(exec_addr);

  return addtional_cycle;
}

int lda_indirect_y() {
  return transfer_indirect_y(exec_lda);
}

int sta_indirect_y() {
  return transfer_indirect_y(exec_sta);
}

int tax_implied() {
  registers.index_x = registers.accumulator;
  set_z_flag(registers.index_x);
  set_n_flag(registers.index_x);
  return 0;
}

int txa_implied() {
  registers.accumulator = registers.index_x;
  set_z_flag(registers.accumulator);
  set_n_flag(registers.accumulator);
  return 0;
}

int tay_implied() {
  registers.index_y = registers.accumulator;
  set_z_flag(registers.index_y);
  set_n_flag(registers.index_y);
  return 0;
}

int tya_implied() {
  registers.accumulator = registers.index_y;
  set_z_flag(registers.accumulator);
  set_n_flag(registers.accumulator);
  return 0;
}

int tsx_implied() {
  registers.index_x = registers.stack;
  set_z_flag(registers.index_x);
  set_n_flag(registers.index_x);
  return 0;
}

int txs_implied() {
  registers.stack = registers.index_x;
  return 0;
}
