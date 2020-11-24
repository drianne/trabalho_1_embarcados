#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <unistd.h>
#include "../../inc/gpio_module.h"


void configura_pinos(){
    bcm2835_gpio_fsel(RESISTOR, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(VENTI, BCM2835_GPIO_FSEL_OUTP);
}

void start_resistor(int resistor_mask){
    bcm2835_gpio_write(RESISTOR, resistor_mask & 1 );
}

void start_fan(int fan_mask){
    bcm2835_gpio_write(VENTI, fan_mask & 1 );
}

void *temperature_control_gpio(void *params){
    struct Sensors *temps = params;

    if (!bcm2835_init())
      exit(1);
      
    configura_pinos();

    int counter;
    while(1){
        if(temps->temp_interna < (temps->temp_read - temps->hysteresis)){
            // Liga resistor
            start_resistor(ON);
            start_fan(OFF);
        }
        else if(temps->temp_interna > (temps->temp_read + temps->hysteresis)){
            // Liga Ventilador
            start_resistor(OFF);
            start_fan(ON);
        } 
        else {
            // Desliga os dois
            start_resistor(OFF);
            start_fan(OFF);
        }
        usleep(500000);
    }
}
