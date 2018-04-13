#include "instruction.h"
#include "cpu_circuit.h"
#include "memory.h"
#include "instructions/bit_inst.h"
#include "instructions/branch_inst.h"
#include "instructions/calc_inst.h"
#include "instructions/compare_inst.h"
#include "instructions/flag_inst.h"
#include "instructions/increment_inst.h"
#include "instructions/interrupt_inst.h"
#include "instructions/jump_inst.h"
#include "instructions/nop_inst.h"
#include "instructions/shift_inst.h"
#include "instructions/stack_inst.h"
#include "instructions/transfer_inst.h"

instruction instruction_set[0x100] = {
  //0
  {brk_implied, 7, 1}, {ora_indirect_x, 6, 2}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0}, 
  {unknown_opcode, 0, 0}, {ora_zeropage, 3, 2},{asl_zeropage, 5, 2}, {unknown_opcode, 0, 0}, 
  {php_implied, 3, 1}, {ora_immediate, 2, 2}, {asl_accumulator, 2, 1}, {unknown_opcode, 0, 0}, 
  {unknown_opcode, 0, 0}, {ora_absolute, 4, 3}, {asl_absolute, 6, 3}, {unknown_opcode, 0, 0},
  //1
  {bpl_relative, 2, 2}, {ora_indirect_y, 5, 2}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0}, 
  {unknown_opcode, 0, 0}, {ora_zeropage_x, 4, 2}, {asl_zeropage_x, 6, 2}, {unknown_opcode, 0, 0}, 
  {clc_implied, 2, 1}, {ora_absolute_y, 4, 3}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {unknown_opcode, 0, 0}, {ora_absolute_x, 4, 3}, {asl_absolute_x, 6, 3}, {unknown_opcode, 0, 0},
  //2
  {jsr_absolute, 6, 3}, {and_indirect_x, 6, 2}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {bit_zeropage, 3, 2}, {and_zeropage, 3, 2}, {rol_zeropage, 5, 2}, {unknown_opcode, 0, 0},
  {plp_implied, 4, 1}, {and_immediate, 2, 2}, {rol_accumulator, 2, 1}, {unknown_opcode, 0, 0},
  {bit_absolute, 4, 3}, {and_absolute, 4, 3}, {rol_absolute, 6, 3}, {unknown_opcode, 0, 0},
  //3
  {bmi_relative, 2, 2}, {and_indirect_y, 5, 2}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {unknown_opcode, 0, 0}, {and_zeropage_x, 4, 2}, {rol_zeropage_x, 6, 2}, {unknown_opcode, 0, 0},
  {sec_implied, 2, 1}, {and_absolute_y, 4, 3}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {unknown_opcode, 0, 0}, {and_absolute_x, 4, 3}, {rol_absolute_x, 6, 3}, {unknown_opcode, 0, 0},
  //4
  {rti_implied, 6, 1}, {eor_indirect_x, 6, 2}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {unknown_opcode, 0, 0}, {eor_zeropage, 3, 2}, {lsr_zeropage, 5, 2}, {unknown_opcode, 0, 0},
  {pha_implied, 4, 1}, {eor_immediate, 2, 2}, {lsr_accumulator, 2, 1}, {unknown_opcode, 0, 0},
  {jmp_absolute, 3, 3}, {eor_absolute, 4, 3}, {lsr_absolute, 6, 3}, {unknown_opcode, 0, 0},
  //5
  {bvc_relative, 2, 2}, {eor_indirect_y, 5, 2}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {unknown_opcode, 0, 0}, {eor_zeropage_x, 4, 2}, {lsr_zeropage_x, 6, 2}, {unknown_opcode, 0, 0},
  {cli_implied, 2, 1}, {eor_absolute_y, 4, 3}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {unknown_opcode, 0, 0}, {eor_absolute_x, 4, 3}, {lsr_absolute_x, 6, 3}, {unknown_opcode, 0, 0},
  //6
  {rts_implied, 6, 1}, {adc_indirect_x, 6, 2}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {unknown_opcode, 0, 0}, {adc_zeropage, 3, 2}, {ror_zeropage, 5, 2}, {unknown_opcode, 0, 0},
  {pla_implied, 4, 1}, {adc_immediate, 2, 2}, {ror_accumulator, 2, 1}, {unknown_opcode, 0, 0},
  {jmp_indirect, 5, 3}, {adc_absolute, 4, 3}, {ror_absolute, 6, 3}, {unknown_opcode, 0, 0},
  //7
  {bvs_relative, 2, 2}, {adc_indirect_y, 5, 2}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {unknown_opcode, 0, 0}, {adc_zeropage_x, 4, 2}, {ror_zeropage_x, 6, 2}, {unknown_opcode, 0, 0},
  {sei_implied, 2, 1}, {adc_absolute_y, 4, 3}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {unknown_opcode, 0, 0}, {adc_absolute_x, 4, 3}, {ror_absolute_x, 6, 3}, {unknown_opcode, 0, 0},
  //8
  {unknown_opcode, 0, 0}, {sta_indirect_x, 6, 2}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {sty_zeropage, 3, 2}, {sta_zeropage, 3, 2}, {stx_zeropage, 3, 2}, {unknown_opcode, 0, 0},
  {dey_implied, 2, 1}, {unknown_opcode, 0, 0}, {txa_implied, 2, 1}, {unknown_opcode, 0, 0},
  {sty_absolute, 4, 3}, {sta_absolute, 4, 3}, {stx_absolute, 4, 3}, {unknown_opcode, 0, 0},
  //9
  {bcc_relative, 2, 2}, {sta_indirect_y, 5, 2}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {sty_zeropage_index, 4, 2}, {sta_zeropage_index, 4, 2}, {stx_zeropage_index, 4, 2}, {unknown_opcode, 0, 0},
  {tya_implied, 2, 1}, {sta_absolute_index_y, 4, 3}, {txs_implied, 2, 1}, {unknown_opcode, 0, 0},
  {unknown_opcode, 0, 0}, {sta_absolute_index_x, 4, 3}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  //A
  {ldy_immediate, 2, 2}, {lda_indirect_x, 6, 2}, {ldx_immediate, 2, 2}, {unknown_opcode, 0, 0},
  {ldy_zeropage, 3, 2}, {lda_zeropage, 3, 2}, {ldx_zeropage, 3, 2}, {unknown_opcode, 0, 0},
  {tay_implied, 2, 1}, {lda_immediate, 2, 2}, {tax_implied, 2, 1}, {unknown_opcode, 0, 0},
  {ldy_absolute, 4, 3}, {lda_absolute, 4, 3}, {ldx_absolute, 4, 3}, {unknown_opcode, 0, 0},
  //B
  {bcs_relative, 2, 2}, {lda_indirect_y, 5, 2}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {ldy_zeropage_index, 4, 2}, {lda_zeropage_index, 4, 2}, {ldx_zeropage_index, 4, 2}, {unknown_opcode, 0, 0},
  {clv_implied, 2, 1}, {lda_absolute_index_y, 4, 3}, {tsx_implied, 2, 1}, {unknown_opcode, 0, 0},
  {ldy_absolute_index_x, 4, 3}, {lda_absolute_index_x, 4, 3}, {ldx_absolute_index_y, 4, 3}, {unknown_opcode, 0, 0},
  //C
  {cpy_immediate, 2, 2}, {cmp_indirect_x, 6, 2}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {cpy_zeropage, 3, 2}, {cmp_zeropage, 3, 2}, {dec_zeropage, 5, 2}, {unknown_opcode, 0, 0},
  {iny_implied, 2, 1}, {cmp_immediate, 2, 2}, {dex_implied, 2, 1}, {unknown_opcode, 0, 0},
  {cpy_absolute, 4, 3}, {cmp_absolute, 4, 3}, {dec_absolute, 6, 3}, {unknown_opcode, 0, 0},
  //D
  {bne_relative, 2, 2}, {cmp_indirect_y, 5, 2}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {unknown_opcode, 0, 0}, {cmp_zeropage_x, 4, 2}, {dec_zeropage_x, 6, 2}, {unknown_opcode, 0, 0},
  {cld_implied, 2, 1}, {cmp_absolute_y, 4, 3}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {unknown_opcode, 0, 0}, {cmp_absolute_x, 4, 3}, {dec_absolute_x, 6, 3}, {unknown_opcode, 0, 0},
  //E
  {cpx_immediate, 2, 2}, {sbc_indirect_x, 6, 2}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {cpx_zeropage, 3, 2}, {sbc_zeropage, 3, 2}, {inc_zeropage, 5, 2}, {unknown_opcode, 0, 0},
  {inx_implied, 2, 1}, {sbc_immediate, 2, 2}, {nop_implied, 2, 1}, {unknown_opcode, 0, 0},
  {cpx_absolute, 4, 3}, {sbc_absolute, 4, 3}, {inc_absolute, 6, 3}, {unknown_opcode, 0, 0},
  //F
  {beq_relative, 2, 2}, {sbc_indirect_y, 5, 2}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {unknown_opcode, 0, 0}, {sbc_zeropage_x, 4, 2}, {inc_zeropage_x, 6, 2}, {unknown_opcode, 0, 0},
  {sed_implied, 2, 1}, {sbc_absolute_y, 4, 3}, {unknown_opcode, 0, 0}, {unknown_opcode, 0, 0},
  {unknown_opcode, 0, 0}, {sbc_absolute_x, 4, 3}, {inc_absolute_x, 6, 3}, {unknown_opcode, 0, 0}
};