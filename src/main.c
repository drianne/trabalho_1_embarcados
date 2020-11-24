#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

#include <pthread.h>

#include <time.h>
#include <bcm2835.h>

#include "../inc/t_module_i2c.h"
#include "../inc/uart_module.h"
#include "../inc/lcd_module.h"
#include "../inc/gpio_module.h"
#include "../inc/csv_module.h"
#include "../inc/user.h"

void trata_interrupcao(int sinal) {
    start_resistor(OFF);
    start_fan(OFF);
    bcm2835_close();
    endwin();
    exit(0);
}

int main(void) {
  pthread_t thread_I2c, thread_Uart, thread_LcdPrint, thread_Gpio, thread_Csv, thread_Menu;
  struct Sensors temps;

  signal(SIGINT, trata_interrupcao);

  // Inicializando
  temps.temp_read = 25;
  temps.hysteresis = 2;
  strcpy(temps.control_type, "t");   // Entrada do teclado  
  
  pthread_create(&thread_I2c, NULL, temperature_control, &temps); 
  pthread_create(&thread_Uart, NULL, uart_main, &temps);  
  pthread_create(&thread_LcdPrint, NULL, lcd_control, &temps);
  pthread_create(&thread_Gpio, NULL, temperature_control_gpio, &temps);
  pthread_create(&thread_Csv, NULL, registrar_csv, &temps);
  pthread_create(&thread_Menu, NULL, menu, &temps);

	pthread_join(thread_I2c, NULL);
	pthread_join(thread_Uart, NULL);
	pthread_join(thread_LcdPrint, NULL);
	pthread_join(thread_Gpio, NULL);
	pthread_join(thread_Menu, NULL);
  return 0;
}