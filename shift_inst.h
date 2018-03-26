#ifndef HEADER_SHIFT_INST
#define HEADER_SHIFT_INST

int asl_accumulator();
int lsr_accumulator();
int rol_accumulator();
int ror_accumulator();

int asl_zeropage();
int lsr_zeropage();
int rol_zeropage();
int ror_zeropage();

int asl_zeropage_x();
int lsr_zeropage_x();
int rol_zeropage_x();
int ror_zeropage_x();

int asl_absolute();
int lsr_absolute();
int rol_absolute();
int ror_absolute();

int asl_absolute_x();
int lsr_absolute_x();
int rol_absolute_x();
int ror_absolute_x();

#endif