int menu()
{   
   int cod;

   printf("================= MENU =================\n");
   printf("1 - Mudar temperatura base via teclado\n");
   printf("2 - Mudar temperatura base via potenciômetro\n");
   printf("3 - Mudar histerese\n");
   printf("0 - Encerrar\n");
   scanf("%d", &cod);
   return cod;
}