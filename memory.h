#ifndef HEADER_MEMORY
#define HEADER_MEMORY

extern unsigned char memory[];

unsigned char memory_read(unsigned short addr);
void memory_write(unsigned short addr, unsigned char data);
void init_memory();

#endif