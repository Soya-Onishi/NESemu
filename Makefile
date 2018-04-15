all:
	ccgl -g main.c cpu.c cpu_circuit.c instruction.c memory.c ppu.c bg_render.c ppu_rendering.c rendering.c sprite_render.c status_flag_manager.c controller.c window.c instructions/*.c
