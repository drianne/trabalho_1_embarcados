#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h> 
#include <errno.h>
#include <string.h>

// Temperatura interna 

int open_uart(){
    int uart0 = -1;
    uart0 = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY ); 
    
    if (uart0 == -1){
        printf("\nErro - Não foi possível iniciar a UART.\n");
        return -1;
    }
    else{
        printf("\nUART inicializada!\n");
    }    

    struct termios options;

    tcgetattr(uart0, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;     
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0, TCIFLUSH);
    tcsetattr(uart0, TCSANOW, &options);

    return uart0;
}

float read_uart(int uart){
    float rx_buffer;
    int rx_length = read(uart, &rx_buffer, sizeof(rx_buffer)); 

    return rx_buffer;
}

int get_uart_value(int cod) {    
    // int uart0_filestream;
    float dado_f;
    char dado_s[40];
    unsigned char tx_buffer[260], *p_tx_buffer;
    p_tx_buffer = &tx_buffer[0];       
    unsigned char cod_temp_interna = 0xA1; // 1
    unsigned char cod_potenciometro = 0xA2; // 2
    float value; 
    int uart0_filestream;

    char padrao[] ={cod, 7, 5, 9, 5};

    if(cod == 1){
       padrao[0] = cod_temp_interna;
    }else if (cod == 2){
       padrao[0] = cod_potenciometro;
    }
    uart0_filestream = open_uart();

    int count = write(uart0_filestream, &padrao[0], 5);

    if (count < 0) {
        printf("UART erro\n");
    } else {
        printf("escrito.\n");
    }

    sleep(1); 
    value = read_uart(uart0_filestream);
    close(uart0_filestream);

   return value;
}

