#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include <errno.h>
#include <string.h>
#include <time.h> 

int return_cod(){
    char **ptr;
    return (int)strtol("0xA1", ptr, 16);
}

void add_id_number(unsigned char *msg){
    *msg++ = 7;
    *msg++ = 5;
    *msg++ = 9;
    *msg++ = 5;
    *msg++ = '\0';
}

int open_uart(){
    int uart0 = -1;
    uart0 = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY ); //Open in non blocking read/write mode
    
    if (uart0 == -1)
    {
        printf("Erro - Não foi possível iniciar a UART.\n");
        return -1;
    }
    else
    {
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

void write(int uart, unsigned char *tx_buffer){
    if (uart != -1){
        printf("Escrevendo caracteres na UART ...\n\n");
        int count = write(uart, &tx_buffer[0], strlen((char *)tx_buffer));
        if (count < 0) {
            printf("UART TX error\n");
        } else {
            printf("escrito.\n");
        }
    }
}


void read_return(int uart){
    float rx_buffer;
    int rx_length = read(uart, &rx_buffer, sizeof(rx_buffer)); 
    if (rx_length > 0){
        printf("Mensagem: %f\n", rx_buffer);
    } else if (rx_length < 0){
        printf("%s\n", strerror(errno));
    } else {
        printf("Nenhum dado disponível.\n");
    }
}

void le_string(int uart){
    unsigned char rx_buffer[256];    
    unsigned char *p_rx_buffer;

    int rx_length = read(uart, (void *)rx_buffer, 255);
  
    if (rx_length > 0)
    {
        p_rx_buffer = &rx_buffer[1];
        printf("Tamanho: %d\nMensagem: %s\n", rx_buffer[0], p_rx_buffer);
    } else if (rx_length < 0){
        printf("%s\n", strerror(errno));
    } else {
        printf("Nenhum dado disponível.\n");
    }
}

int main(int argc, const char * argv[]) {    
    int cod, dado_i, uart0_filestream;
    float dado_f;
    char dado_s[40];
    unsigned char tx_buffer[260];
    unsigned char *p_tx_buffer;
    p_tx_buffer = &tx_buffer[0];    

    uart0_filestream = open_uart();

    do {
        cod = return_cod();
        *p_tx_buffer++ = cod;
        add_id_number(p_tx_buffer);

        write(uart0_filestream, tx_buffer); 

        sleep(1); 
        read_return(uart0_filestream);
        sleep(0.5);
    }while(cod != 0);
    close(uart0_filestream);
   return 0;
}