#ifndef HEADER_COMPARE_INST
#define HEADER_COMPARE_INST

int cmp_immediate();
int cpx_immediate();
int cpy_immediate();

int cpm_zeropage();
int cpx_zeropage();
int cpy_zeropage();

int cmp_absolute();
int cpx_absolute();
int cpy_absolute();

int cmp_zeropage_x();

int cmp_absolute_x();
int cmp_absolute_y();

int cmp_indirect_x();
int cmp_indirect_y();

#endif