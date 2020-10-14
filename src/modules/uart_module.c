#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h> 
#include <errno.h>
#include <string.h>

int open_uart(){
    int uart0 = -1;
    uart0 = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY ); 
    
    if (uart0 == -1){
        printf("Erro - Não foi possível iniciar a UART.\n");
        return -1;
    }
    else{
        printf("UART inicializada!\n");
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

void write_uart(int uart, unsigned char *tx_buffer){
    if (uart != -1){
        printf("Escrevendo na UART ...\n\n");
        int count = write(uart, &tx_buffer[0], strlen((char *)tx_buffer));
        if (count < 0) {
            printf("UART erro\n");
        } else {
            printf("escrito.\n");
        }
    }
}

void read_uart(int uart){
    float rx_buffer;
    int rx_length = read(uart, &rx_buffer, sizeof(rx_buffer)); //Filestream, buffer to store in, number of bytes to read (max)
    if (rx_length > 0){
        printf("%fºC\n", rx_buffer);
    } else if (rx_length < 0){
        printf("%s\n", strerror(errno));
    } else {
        printf("Nenhum dado disponível.\n");
    }
}

int main(int argc, const char * argv[]) {    
    int uart0_filestream;
    float dado_f;
    char dado_s[40];
    unsigned char tx_buffer[260], *p_tx_buffer, cod_temp_interna, cod_potenciometro;
    p_tx_buffer = &tx_buffer[0];    

    cod_temp_interna = 0xA1;
    cod_potenciometro = 0xA2;

    uart0_filestream = open_uart();

    do {
        *p_tx_buffer++ = cod_potenciometro;
        *p_tx_buffer++ = 7;
        *p_tx_buffer++ = 5;
        *p_tx_buffer++ = 9;
        *p_tx_buffer++ = 5;

        add_identity(p_tx_buffer);
        write_uart(uart0_filestream, tx_buffer); 
        sleep(1); 
        read_uart(uart0_filestream);

    }while(1);

    close(uart0_filestream);
   return 0;
}