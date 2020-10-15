#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <unistd.h>

#define ON 0
#define OFF 1

/* Setting pins */
#define FAN RPI_V2_GPIO_P1_18 // BCM 24
#define RESISTOR RPI_V2_GPIO_P1_16 // BCM 23

void start_resistor(int resistor_mask) {
    bcm2835_gpio_write(RESISTOR, resistor_mask & 1 );
}

void start_fan(int fan_mask) {
    bcm2835_gpio_write(FAN, fan_mask & 1 );
}

void setting_pins(){
    // Setting pins as out
    bcm2835_gpio_fsel(RESISTOR, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(FAN, BCM2835_GPIO_FSEL_OUTP);
}

int test_module(){

    if (!bcm2835_init())
      exit(1);
      
    setting_pins();

    start_resistor(OFF);
    start_fan(OFF);
    sleep(2);
    printf("Liga Ventoinha\n");
    start_fan(ON);
    sleep(3);
    printf("Liga Resistor\n");
    start_resistor(ON);
    sleep(3);
    printf("Desligando Ventoinha\n");
    start_fan(OFF);
    sleep(3);
    printf("Desligando Resistor\n");
    start_resistor(OFF);
}
