#ifndef HEADER_PPU_RENDERING
#define HEADER_PPU_RENDERING

int get_dots();
int get_scanline();
int get_frame();

int get_true_scanline();

void ppu_rendering();
void start_rendering();
void init_rendering_funcs();

#endif