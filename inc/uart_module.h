#include "../src/main.h"
#define RASP_UART_DEVICE  "/dev/serial0"
#define TEMP 161
#define POTEN 162
#define UART_PORT "/dev/serial0"

void *uart_main(void *params);
int open_uart_connection();
void escrever_dados_uart(unsigned char *, int);
void formata_mensagem(int, unsigned char *);
void ler_dados_uart(unsigned char *, int, int, struct Sensors *temps);