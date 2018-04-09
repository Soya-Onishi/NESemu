#ifndef HEADER_CONTROLLER
#define HEADER_CONTROLLER

#define CONTROLLER_A (1 << 0)
#define CONTROLLER_B (1 << 1)
#define CONTROLLER_SELECT (1 << 2)
#define CONTROLLER_START (1 << 3)
#define CONTROLLER_UP (1 << 4)
#define CONTROLLER_DOWN (1 << 5)
#define CONTROLLER_LEFT (1 << 6)
#define CONTROLLER_RIGHT (1 << 7)

void set_controller(unsigned char data);
unsigned char read_controller(unsigned short addr);

void key_down(unsigned char key, int x, int y);
void key_up(unsigned char key, int x, int y);
void special_key_down(int key, int x, int y);
void special_key_up(int key, int x, int y);

#endif