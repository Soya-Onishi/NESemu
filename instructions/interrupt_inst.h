#ifndef HEADER_INTERRUPT_INST
#define HEADER_INTERRUPT_INST

int reset_implied();
int nmi_implied();
int irq_implied();
int brk_implied();

#endif