#ifndef HEADER_CALC_INST
#define HEADER_CALC_INST

int adc_immediate();
int sbc_immediate();
int and_immediate();
int ora_immediate();
int eor_immediate();

int adc_zeropage();
int sbc_zeropage();
int and_zeropage();
int ora_zeropage();
int eor_zeropage();

int adc_zeropage_x();
int sbc_zeropage_x();
int and_zeropage_x();
int ora_zeropage_x();
int eor_zeropage_x();

int adc_absolute();
int sbc_absolute();
int and_absolute();
int ora_absolute();
int eor_absolute();

int adc_absolute_x();
int adc_absolute_y();
int sbc_absolute_x();
int sbc_absolute_y();
int and_absolute_x();
int and_absolute_y();
int ora_absolute_x();
int ora_absolute_y();
int eor_absolute_x();
int eor_absolute_y();

int adc_indirect_x();
int sbc_indirect_x();
int and_indirect_x();
int ora_indirect_x();
int eor_indirect_x();

int adc_indirect_y();
int sbc_indirect_y();
int and_indirect_y();
int ora_indirect_y();
int eor_indirect_y();

#endif