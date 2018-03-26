#ifndef HEADER_TRANSFER_INST
#define HEADER_TRANSFER_INST

int lda_immediate();
int ldx_immediate();
int ldy_immediate();

int lda_zeropage();
int ldx_zeropage();
int ldy_zeropage();
int sta_zeropage();
int stx_zeropage();
int sty_zeropage();

int lda_zeropage_index();
int ldx_zeropage_index();
int ldy_zeropage_index();
int sta_zeropage_index();
int stx_zeropage_index();
int sty_zeropage_index();

int lda_absolute();
int ldx_absolute();
int ldy_absolute();
int sta_absolute();
int stx_absolute();
int sty_absolute();

int lda_absolute_index_x();
int lda_absolute_index_y();
int ldx_absolute_index_y();
int ldy_absolute_index_x();
int sta_absolute_index_x();
int sta_absolute_index_y();

int lda_indirect_x();
int sta_indirect_x();

int lda_indirect_y();
int sta_indirect_y();

int tax_implied();
int txa_implied();
int tay_implied();
int tya_implied();
int tsx_implied();
int txs_implied();

#endif