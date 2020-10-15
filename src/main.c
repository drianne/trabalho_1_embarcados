#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#include <pthread.h>

#include "./modules/i2c_module.c"
#include "./modules/uart_module.c"
#include "./modules/lcd_module.c"
#include "./modules/gpio_module.c"
#include "./modules/user.c"


int executar_temp, executa_sistema, executa_poten, executa_show_temps, executa_salva_csv, executa_sistema;
int contador_temperatura, contador_sistema, contador_show_temps, codigo, contador_salva_csv;
float temperatura_interna, temperatura_externa, temp, temp_histerese, histerese, temperatura_recebida;

contador_temperatura = 0;
contador_sistema = 0;
contador_show_temps = 0;
contador_salva_csv = 0;

void controla_sistema(){
    if (temperatura_interna < (temperatura_recebida - histerese)){
        start_fan(OFF);
        start_resistor(ON);
    }else if (temperatura_interna > (temperatura_recebida + histerese)){
        start_resistor(OFF);
        start_fan(ON);
    }
}

// histerese = 0;

// void apresenta_menu(){
//     while(1){
//         codigo = menu();
//         switch (codigo)
//         {
//         case 1:
//             scanf("%f", &temp);
//             temperatura_recebida = temp; 
//             break;
//         case 2: 
//             executa_poten = 1;
//             break;
//         case 3: 
//             scanf("%f", &temp_histerese);
//             histerese = temp_histerese;
//             break;
//         default:
//             exit(1);
//             break;
//         }
//     }
// }

// void salva_csv(){
//     while(1){
//         if(executa_salva_csv == 1){
//             save_into_csv(temperatura_interna, temperatura_externa, temperatura_recebida);
//             executa_salva_csv = 0;
//         }
//         contador_salva_csv ++; 
//         usleep(100000);
//     }    
// }

// void mostra_temps(){
//     while(1){
//         if (executa_show_temps == 1){
//             imprime_led(temperatura_interna, temperatura_externa, temperatura_recebida); 
//             executa_show_temps = 0;
//         }
//         contador_show_temps ++;
//         usleep(100000);
//     }
    
// }

// void get_potenciometro(){
//     while(1){
//         if(executa_poten == 1){
//             temperatura_recebida = get_uart_value(2);
//             executa_poten = 0;
//         }
//         usleep(100000);
//     }
// }

// void get_temperature_value(){
//     while(1){
//         if(executar_temp == 1){
//             temperatura_interna = get_uart_value(1);
//             temperatura_externa = get_external_temperature();
//             executar_temp = 0;
//         }
//         contador_temperatura ++;
//         usleep(100000);
//     }
// }

// void aciona_sistema(){
//     while(1){
//         if(executa_sistema == 1){
//             if (temperatura_interna < (temperatura_recebida - histerese)){
//                 start_fan(OFF);
//                 start_resistor(ON);
//             }else if (temperatura_interna > (temperatura_recebida + histerese)){
//                 start_resistor(OFF);
//                 start_fan(ON);
//             }

//             executa_sistema = 0;
//         }
        
//         contador_sistema ++;
//         usleep(100000);
//     }
// }

// void signal_controller(int sig) { 
//     if(contador_temperatura == 5){
//         contador_temperatura = 0;
//         executar_temp = 1;        
//     }

//     if (!executa_sistema && contador_sistema >= 10){
//         executa_sistema = 1;
//         contador_sistema = 0;
//     }

//     if (!executa_show_temps == 5){
//         executa_show_temps = 1;
//         contador_sistema = 0;
//     }

//     if (contador_salva_csv == 20){
//         executa_salva_csv = 1;
//         contador_salva_csv = 0;
//     }
//     alarm(1000);
// } 

int main(int argc, char* argv[]){
    // pthread_t thread_temperatura, thread_aciona_sistema, thread_potenciometro;
    // pthread_t thread_mostra_temperaturas, thread_mostra_temperaturas;
    // pthread_t thread_armazena_csv, thread_menu;
    
    
    // pthread_create (&thread_temperatura, NULL, &get_temperature_value, NULL); //Threads temperature
    // pthread_create (&thread_aciona_sistema, NULL, &aciona_sistema, NULL);
    // pthread_create (&thread_potenciometro, NULL, &get_potenciometro, NULL);
    // pthread_create (&thread_mostra_temperaturas, NULL, &mostra_temps, NULL);
    // pthread_create (&thread_armazena_csv, NULL, &salva_csv, NULL);
    // pthread_create (&thread_menu, NULL, &apresenta_menu, NULL);

    // pthread_join(thread_temperatura, NULL);
    // pthread_join(thread_aciona_sistema, NULL);
    // pthread_join(thread_potenciometro, NULL);
    // pthread_join(thread_mostra_temperaturas, NULL);
    // pthread_join(thread_armazena_csv, NULL);
    // pthread_join(thread_menu, NULL);

    // signal(SIGALRM, signal_controller);
    // alarm(1000);
    
    int uart0_filestream;
    int cod; 

    temperatura_interna = 40.00;
    temperatura_recebida = 25.0;
    histerese = 2.0;
    int get_potenciometro = 1;
    float temperatura_r, histerese_temp;

    uart0_filestream = open_uart();
    while(1){     
        cod = menu();

        switch (cod){
        case 1:
            scanf("%f", &temperatura_r);
            temperatura_recebida = temperatura_r; 
            printf("\nTemperatura usuario: %f", temperatura_recebida);
            get_potenciometro = 0;
            break;
        case 2: 
            temperatura_recebida = get_uart_value(2);
            printf("\nTemperatura Potenciometro: %f", temperatura_recebida);
            get_potenciometro = 1;
            break;
        case 3: 
            scanf("%f", &histerese_temp);
            histerese = histerese_temp;
            break;
        default:
            exit(1);
            break;
        } 

        printf("\nAjustando Sistema\n");
        controla_sistema(); 

        printf("\nImprimindo temperaturas\n");
        imprime_led(temperatura_interna, temperatura_externa, temperatura_recebida);
        
        // temperatura_interna = get_uart_value(1);
        // printf("Temperatura Interna: %f", temperatura_interna);
        // sleep(1);
        // temperatura_externa = get_external_temperature(argc, argv);
        // printf("Temperatura Externa: %f", temperatura_externa);
        // sleep(1);

        // if(get_potenciometro){
        //     temperatura_recebida = get_uart_value(2);
        //     printf("Temperatura Recebida: %f", temperatura_recebida);
        //     sleep(1);
        // }
        
        // save_into_csv(temperatura_interna, temperatura_externa, temperatura_recebida);
        
    }
    close(uart0_filestream);

    return  0;
}