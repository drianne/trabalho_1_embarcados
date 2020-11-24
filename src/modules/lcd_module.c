#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../../inc/lcd_module.h"

void lcd_byte(int bits, int mode);
void lcd_toggle_enable(int bits);
void typeInt(int i);
void typeFloat(float myFloat);
void lcdLoc(int line); 
void ClrLcd(void); 
void typeln(const char *s);
void typeChar(char val);

int fd;  

void lcd_init();

void *lcd_control(void* params)   {
  struct Sensors *temps = params;

  if (wiringPiSetup () == -1) {
    exit (1);
  } 

  fd = wiringPiI2CSetup(I2C_ADDR);
  lcd_init(); // setup LCD
  while (1) {
    typeln(" TI");
    float temp_interna = temps->temp_interna;
    typeFloat(temp_interna);
    typeln("TE");
    float temp_externa = temps->temp_externa;
    typeFloat(temp_externa);    
    lcdLoc(LINE2);
    typeln("TR");
    float temp_referencia = temps->temp_read;
    typeFloat(temp_referencia);
    delay(1000);
    ClrLcd();
  }
}

// float to string
void typeFloat(float myFloat){
  char buffer[20];
  sprintf(buffer, "%4.2f",  myFloat);
  typeln(buffer);
}

// clr lcd go home loc 0x80
void ClrLcd(void){
  lcd_byte(0x01, LCD_CMD);
  lcd_byte(0x02, LCD_CMD);
}

// go to location on LCD
void lcdLoc(int line)   {
  lcd_byte(line, LCD_CMD);
}

// this allows use of any size string
void typeln(const char *s){
  while ( *s ) lcd_byte(*(s++), LCD_CHR);
}

void lcd_byte(int bits, int mode)   {
  int bits_high;
  int bits_low;
  
  bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
  bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

  // High bits
  wiringPiI2CReadReg8(fd, bits_high);
  lcd_toggle_enable(bits_high);

  // Low bits
  wiringPiI2CReadReg8(fd, bits_low);
  lcd_toggle_enable(bits_low);
}

void lcd_toggle_enable(int bits)   {
  // Toggle enable pin on LCD display
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits | ENABLE));
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
  delayMicroseconds(500);
}

void lcd_init()   {
  // Initialise display
  lcd_byte(0x33, LCD_CMD); // Initialise
  lcd_byte(0x32, LCD_CMD); // Initialise
  lcd_byte(0x06, LCD_CMD); // Cursor move direction
  lcd_byte(0x0C, LCD_CMD); // 0x0F On, Blink Off
  lcd_byte(0x28, LCD_CMD); // Data length, number of lines, font size
  lcd_byte(0x01, LCD_CMD); // Clear display
  delayMicroseconds(500);
}
