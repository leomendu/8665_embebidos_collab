 #include "sapi.h"       // <= sAPI header
// Definiciones y variables
int ESTADO = 0;                                    //Variable de estado control (Maquina de Estados)
int ESTADO2 = 0;                                    //Variable de estado control (Maquina de Estados)
bool_t valor;                             // Variable para almacenar el valor de tecla leido
uint8_t dato  = 0;

enum{
   ESPERANDO,
   TRASMITIDO,
   PULSADO_UNO,
   PULSADO_DOS,
   PULSADO_TRES,
   PULSADO_CUATRO,
 };


// FUNCIONES ------------------------------------------------------------------------------------------------
void SELECTOR_DATO(uint8_t  dato){
   if(dato=='1'){ESTADO =PULSADO_UNO;return;}
   if(dato=='2'){ESTADO =PULSADO_DOS;return;}
   if(dato=='3'){ESTADO =PULSADO_TRES;return;}
   if(dato=='4'){ESTADO =PULSADO_CUATRO;return;}
   ESTADO=ESPERANDO;
}


void FUNCION_UART232(void){

 switch(ESTADO){

  case(ESPERANDO):
      valor = !gpioRead( TEC1 );
       gpioWrite( LEDB, valor );
      valor = !gpioRead( TEC2 );
      gpioWrite( LED1, valor );
      valor = !gpioRead( TEC3 );
      gpioWrite( LED2, valor );
      valor = !gpioRead( TEC4 );
      gpioWrite( LED3, valor );
      valor = !gpioRead( GPIO0 );
      gpioWrite( GPIO1, valor );

       if(!gpioRead(TEC1) == 1){
       uartWriteByte( UART_232, '1' );
         ESTADO=TRASMITIDO;
       return;}

       if(!gpioRead(TEC2) == 1){
       uartWriteByte( UART_232, '2' );
         ESTADO=TRASMITIDO;
       return;}

       if(!gpioRead(TEC3) == 1){
       uartWriteByte( UART_232, '3');
         ESTADO=TRASMITIDO;
       return;}

       if(!gpioRead(TEC4) == 1){
       uartWriteByte( UART_232, '4');
         ESTADO=TRASMITIDO;
       return;}

       break;



  case(TRASMITIDO):
        uartReadByte( UART_232, &dato);
        SELECTOR_DATO(dato);
        break;


   case(PULSADO_UNO):
        gpioWrite( LED1, HIGH );
        delay(50);
        ESTADO=ESPERANDO;
       break;

   case(PULSADO_DOS):
       gpioWrite( LED2, HIGH );
       delay(50);
       ESTADO=ESPERANDO;
       break;

   case(PULSADO_TRES):
       gpioWrite( LED3, HIGH );
       delay(50);
       ESTADO=ESPERANDO;
       break;

   case(PULSADO_CUATRO):
       gpioWrite( LEDB, HIGH );
       delay(50);
       ESTADO=ESPERANDO;
       break;
      }
}



//PROGRAMA -----------------------------------------------------------------------------------
// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET.
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   // Inicializar la placa */
   /* Inicializar la placa */
   boardConfig();
   // Inicializar UART_232 a 115200 baudios
   uartConfig( UART_232, 115200 );
   gpioConfig( GPIO0, GPIO_INPUT );
   gpioConfig( GPIO1, GPIO_OUTPUT );
   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {
      FUNCION_UART232();
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}