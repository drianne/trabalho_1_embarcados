#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include "../../inc/uart_module.h"

int open_uart_connection();
void escrever_dados_uart(unsigned char *, int);
void formata_mensagem(int, unsigned char *);
void ler_dados_uart(unsigned char *, int, int, struct Sensors *temps);

void *uart_main(void *params)
{
    struct Sensors *temps = params;

    int escolha;
    int uart0_filestream = -1;

    uart0_filestream = open_uart_connection();

    unsigned char tx_buffer_temp[10];
    unsigned char tx_buffer_pot[10];

    while(1) {
        formata_mensagem(TEMP, &tx_buffer_temp[0]);
        formata_mensagem(POTEN, &tx_buffer_pot[0]);

        escrever_dados_uart(&tx_buffer_temp[0], uart0_filestream);
        ler_dados_uart(&tx_buffer_temp[0], uart0_filestream, TEMP, temps);
        
        if(!strcmp(temps->control_type, "p")) {
            escrever_dados_uart(&tx_buffer_pot[0], uart0_filestream);
            ler_dados_uart(&tx_buffer_pot[0], uart0_filestream, POTEN, temps);
        }
        usleep(500000);
    }
    close(uart0_filestream);
}

int open_uart_connection(){
    int uart0_filestream = -1;

    uart0_filestream = open(UART_PORT, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK); //Open in non blocking read/write mode

    if (uart0_filestream == -1){
        close(uart0_filestream);
    }

    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
    return uart0_filestream;
}

void ler_dados_uart(unsigned char *tx_buffer, int uart0_filestream, int codigo, struct Sensors *temps){
    float rx_buffer;
    int rx_length = read(uart0_filestream, &rx_buffer, sizeof(rx_buffer)); 
    if (codigo == TEMP){
        if(rx_buffer > 0){
            temps->temp_interna= rx_buffer;
        }         
    } else {
        if(rx_buffer > 0){
            temps->temp_externa = rx_buffer;
        }
    } 
}

void escrever_dados_uart(unsigned char *tx_buffer, int uart0_filestream){
    int count = write(uart0_filestream, tx_buffer, strlen((char *)tx_buffer));
}

void formata_mensagem( int code, unsigned char *tx_buffer){
    unsigned char *p_tx_buffer;
    p_tx_buffer = tx_buffer;

    *p_tx_buffer++ = code;
    *p_tx_buffer++ = 7;
    *p_tx_buffer++ = 5;
    *p_tx_buffer++ = 9;
    *p_tx_buffer++ = 5;
    *p_tx_buffer++ = '\0';
}
