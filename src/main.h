#ifndef TEMPERATURE_STRUCT // Só define se ainda não tiver sido definido
#define TEMPERATURE_STRUCT

struct Sensors {
   double temp_externa;
   double temp_interna;
   double hysteresis;
   char control_type[3];   
   double temp_read;   
};  
#endif