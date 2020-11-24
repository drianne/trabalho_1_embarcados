#include "../src/main.h"
#define ON 0
#define OFF 1

#define VENTI RPI_V2_GPIO_P1_18 // BCM 24
#define RESISTOR RPI_V2_GPIO_P1_16 // BCM 23

void *temperature_control_gpio(void *params);
void start_resistor(int);
void start_fan(int);