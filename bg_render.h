#ifndef HEADER_BG_RENDER
#define HEADER_BG_RENDER

void bg_render();

void vblank_set();
void clear_some_flags();

void assign_horizontal_loopy_v();
void assign_vertical_loopy_v();

void nametable_fetch();
void attribute_fetch();
void low_byte_tile_fetch();
void high_byte_tile_fetch();

void bg_tile_fetch();

#endif