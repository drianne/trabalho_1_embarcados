
#include <ncurses.h>			
#include <string.h> 
#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include "../main.h"
#include "../../inc/user.h"

#define MARGIN 10 // Define margem do menu

void bordas();
void menu_usuario(struct Sensors *temps);
void informacoes_sistemas(struct Sensors *temps);

void bordas() {
    char borda[200] = {"==================================================================================================================="};
    mvprintw (0+MARGIN, 1+MARGIN, borda);
    mvprintw (4+MARGIN, 1+MARGIN, borda);
    mvprintw (14+MARGIN, 1+MARGIN, borda);
}

void informacoes_sistemas(struct Sensors *temps) {
    mvprintw (2+MARGIN, 65+MARGIN, "Monitoramento do sistema");
    mvprintw (10+MARGIN, 60+MARGIN, "Controle atual: %s", strcmp("p", temps->control_type) ? "Potenciômetro" : "Usuário");
    mvprintw (0+MARGIN, 1+MARGIN, "=============================");
    mvprintw (6+MARGIN, 60+MARGIN, "Temperatura Interior: %0.2lf ºC", temps->temp_interna);
    mvprintw (7+MARGIN, 60+MARGIN, "Temperatura Exterior: %0.2lf ºC", temps->temp_externa);
    mvprintw (8+MARGIN, 60+MARGIN, "Temperatura Referência: %0.2lf ºC", temps->temp_read);
    mvprintw (9+MARGIN, 60+MARGIN, "Histerese: +- %0.2lf ºC", temps->hysteresis);
}

void menu_usuario(struct Sensors *temps) {
    struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };

    char controlType;
    char code[10] = {'$'};    
    
    mvprintw (2+MARGIN, 20+MARGIN, "Menu Usuário");
    mvprintw(5+MARGIN,2+MARGIN,"1. Definir temperatura esperada");
    mvprintw(6+MARGIN,2+MARGIN,"2. Alterar histerese");
    mvprintw(7+MARGIN,2+MARGIN,"3. Alterar tipo de controle");
    mvprintw(8+MARGIN,2+MARGIN,"CTRL + C sai do sistema");

    mvprintw(9+MARGIN,2+MARGIN,"Opção escolhida:");
    if( poll(&mypoll, 1, 0) ){
      getstr(code);
    }

    if(!strcmp(code, "1")) {
      mvprintw(11+MARGIN,2+MARGIN,"Digite a temperatura: ");
      scanw(" %4lf", &temps->temp_read);
      strcpy(temps->control_type, "t");
    }
    else if(!strcmp(code, "2")) {
      mvprintw(11+MARGIN,2+MARGIN,"Digite o valor da histerese: ");
      scanw(" %4lf", &temps->hysteresis) ;
    }
    else if(!strcmp(code, "3")) {
      mvprintw(11+MARGIN,2+MARGIN,"Digite o controle desejado");
      mvprintw(12+MARGIN,2+MARGIN,"(P - Potenciômetro | T - Teclado):");
      char controle[10];
      getstr(controle);
      if (!strcmp(controle, "P") || !strcmp(controle, "T")) {
        strcpy(temps->control_type, controle);
        mvprintw(13+MARGIN,2+MARGIN,"Controle alterado!");
      }
      else {
        mvprintw(13+MARGIN,2+MARGIN,"Escolha inválida");
      }
    }
    else if(!strcmp(code, "$")) {
      mvprintw(10+MARGIN,2+MARGIN,"");
    }
}

void *menu(void *params) 
{
    struct Sensors *temps = params;
    int row,col;
    initscr ();
    curs_set (0);

    while (1) {
        bordas();
        menu_usuario(temps);
        informacoes_sistemas(temps);        
        usleep(500000);
        refresh ();
        clear();
    }
    endwin();
    return 0;
}
