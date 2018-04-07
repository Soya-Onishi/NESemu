#include "ppu.h"
#include "ppu_rendering.h"

unsigned char read_from_status();
unsigned char read_from_oamdata();
void write_to_oamdata(unsigned char data);
void write_to_ppuaddr(unsigned char data);
void write_to_scroll(unsigned char data);
void write_to_ppudata(unsigned char data);
unsigned char read_from_ppudata();

void address_increment();
void dma_exec(unsigned char std_addr);



unsigned char memory[0x10000];

unsigned short memory_convert(unsigned short addr) {
  if(addr >= 0x0000 && addr <= 0x1FFF) {
    addr &= 0x7FF;
  } else if(addr >= 0x2000 && addr <= 0x3FFF) {
    addr &= 0x2007;
  }

  return addr;
}

unsigned char memory_read(unsigned short addr) {
  unsigned char data;
  
  addr = memory_convert(addr);

  switch(addr) {
    case 2000:
    case 2001:
    case 2003:
    case 2005:
    case 2006:
    case 4014:
      return ppu_data_bus; 
    case 2002:
      return read_from_status();
    case 2004:
      return read_from_oamdata();
    case 2007:
      return read_from_ppudata();
    default:
      return memory[addr];
  }
}

void memory_write(unsigned short addr, unsigned char data) {
  addr = memory_convert(addr);

  if((addr >= 2000 && addr <= 2007) || addr == 4014) {
    ppu_data_bus = data;
  }

  switch(addr) {
    case 2000:
      //write to ctrl register
      ppu_reg.ctrl = data;
      break;
    case 2001:
      //write to mask register
      ppu_reg.mask = data;
      break;
    case 2003:
      //write to sprite memory addr
      ppu_reg.oamaddr = data;
      break;
    case 2004:
      //write sprite memory data
      oam[(ppu_reg.oamaddr >> 2) & 0x3F][ppu_reg.oamaddr & 3] = data;
      break;
    case 2005:
      //write scroll offset
      write_to_scroll(data);
      break;
    case 2006:
      //write accessing ppu memory address
      write_to_ppuaddr(data);
      break;
    case 2007:
      write_to_ppudata(data);
      break;
    case 4014:
      //DMA is occured
      dma_exec(data);
    default:
      memory[addr] = data;
      break;
  }
}

unsigned char read_from_status() {
  unsigned char status = ppu_reg.status;

  ppu_render_info.toggle = 0;
  ppu_reg.status &= (SPRITE_HIT | SPRITE_OVERFLOW);
  ppu_data_bus = status;

  return status;
}

unsigned char read_from_oamdata() {
  int scanline = get_scanline();
  int upper, lower;

  upper = (ppu_reg.oamaddr >> 2) & 0x3F;
  lower = ppu_reg.oamaddr & 3;

  ppu_data_bus = oam[upper][lower];

  if(!(scanline >= 240 && scanline <= 260) && !(ppu_reg.mask & (SPRITE_ENABLE | BG_ENABLE))) {
    ppu_reg.oamaddr++;
  }

  return oam[upper][lower];
}

void write_to_oamdata(unsigned char data) {
  int scanline = get_scanline();
  int upper, lower;

  if((ppu_reg.mask & (SPRITE_ENABLE | BG_ENABLE)) && ((scanline >= 0 && scanline <= 239) || scanline == -1 || scanline == 261)) {
    ppu_reg.oamaddr += 0x04;
  } else {
    upper = (ppu_reg.oamaddr >> 2) & 0x3F;
    lower = ppu_reg.oamaddr & 3;

    oam[upper][lower] = data;

    ppu_reg.oamaddr++;
  }
}

void write_to_ppuaddr(unsigned char data) {
  if(ppu_render_info.toggle) {
    //write to lower bits
    ppu_render_info.t = (ppu_render_info.t & 0xFF00) | data;
    ppu_render_info.v = ppu_render_info.t;
  } else {
    //write to higher bits
    ppu_render_info.t = ((unsigned short)data << 8) | (ppu_render_info.t & 0x00FF);
  }

  ppu_render_info.toggle ^= 1;
}

void write_to_scroll(unsigned char data) {
  if(ppu_render_info.toggle) {
    //write vertical scroll
    unsigned short fine_y, coarse_y;

    fine_y = ((unsigned short)data & 0x07) << 12;
    coarse_y = ((unsigned short)data & 0xF8) << 2;

    ppu_render_info.t = fine_y | (ppu_render_info.t & 0x0C1F) | coarse_y; 
  } else {
    //write horizontal scroll

    unsigned short fine_x, coarse_x;

    fine_x = ((unsigned short)data & 0x07);
    coarse_x = ((unsigned short)data & 0xF8) >> 3;

    ppu_render_info.t = (ppu_render_info.t & 0x001F) | coarse_x;
    ppu_render_info.fine_x = fine_x; 
  }

  ppu_render_info.toggle ^= 1;
}

void write_to_ppudata(unsigned char data) {
  vram_write(ppu_render_info.v, data);
  address_increment();
}

unsigned char read_from_ppudata() {
  static unsigned char data_buffer = 0;
  unsigned char return_data;

  return_data = data_buffer;
  data_buffer = vram_read(ppu_render_info.v);
  ppu_data_bus = return_data;

  address_increment();

  return return_data;
}

void address_increment() {
  if(ppu_reg.status & ADDR_INCREMENT) {
    ppu_render_info.v += 32;
  } else {
    ppu_render_info.v++;
  }
}

void dma_exec(unsigned char std_addr) {
  unsigned short addr = (unsigned short)std_addr << 8;
  unsigned char i;
  int upper, lower;

  //dummy cycle
  ppu_cycle();
  if(get_frame() % 2 == 1) {
    //dummy cycle
    ppu_cycle();
  }

  i = ppu_reg.oamaddr;
  do {
    unsigned char data;

    upper = i >> 2;
    lower = i & 3;

    data = oam[upper][lower];
    oam[upper][lower] = memory_read(addr);
    ppu_cycle();

    memory_write(addr, oam[upper][lower]);
    ppu_cycle();
    
    i++;
    addr++;
  }while(i == ppu_reg.oamaddr);
}