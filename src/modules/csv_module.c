#include <stdio.h>
#include <time.h>
#include <bcm2835.h>
#include <unistd.h>
#include "../main.h"

int arquivo_existe(const char *filename){
    FILE *file;
    if (file = fopen(filename, "r"))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void adiciona_registro(char *filename, struct Sensors *temps){
    FILE *fp;    
    int arquivo;

    arquivo = arquivo_existe(filename);

    fp = fopen(filename, "a");

    if (arquivo != 1){
      fprintf(fp, "Data, Hora, Temperatura Interna, Temperatura Externa, Temperatura de Referência");
    }
    
    while(1){
      time_t t = time(NULL);
      struct tm now = *localtime(&t);
      
      fprintf(fp, "\n%d/%02d/%02d , %02d:%02d:%02d,", now.tm_mday, now.tm_mon + 1, now.tm_year + 1900, now.tm_hour, now.tm_min, now.tm_sec);
      fprintf(fp, "%0.2lf , %0.2lf , %0.2lf ", temps->temp_interna, temps->temp_externa, temps->temp_read);
      sleep(2);
    }
    fclose(fp);
}

void *registrar_csv(void *params){
  struct Sensors *temps = params;
  adiciona_registro("temperaturas.csv", temps);
}