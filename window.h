#ifndef HEADER_WINDOW
#define HEADER_WINDOW

typedef enum power_on_type {
  WINDOW_POWER_ON,
  WINDOW_RESET
}power_on_type;

void init_window(power_on_type type);
void partical_drawing(unsigned char addr);
void display();

#endif