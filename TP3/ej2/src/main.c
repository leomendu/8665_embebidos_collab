/* 86.65 - Sistemas Embebidos
 * Grupo 2
 * Gayet - Mendoza - Massone
 * TP3 - Ejercicio 2 */

// Bibliotecas
  #include "sapi.h"       // <= sAPI header

// Definiciones y variables 
static int ESTADO = 0;                                    //Variable de estado control (Maquina de Estados)
bool_t valor; 													// Variable para almacenar el valor de tecla leido

enum{
	ESPERANDO,
	PULSADO_UNO,
	PULSADO_DOS,
	PULSADO_TRES,
	PULSADO_CUATRO,
 };

// FUNCIONES 
 void PULSADOR_TO_SERIAL(void){

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
       ESTADO =PULSADO_UNO;
       return;}

        if(!gpioRead(TEC2) == 1){
       ESTADO =PULSADO_DOS;
       return;}

       if(!gpioRead(TEC3) == 1){
       ESTADO =PULSADO_TRES;
       return;}

        if(!gpioRead(TEC4) == 1){
       ESTADO =PULSADO_CUATRO;
       return;
       }
       break;

       case(PULSADO_UNO):
       uartWriteByte( UART_USB, 'H' );  // Envia 'H'
       delay(250);
       ESTADO = ESPERANDO;
       break;

       case(PULSADO_DOS):
       uartWriteByte( UART_USB, 'o' );  // Envia 'o'
       delay(250);
       ESTADO = ESPERANDO;
       break;

       case(PULSADO_TRES):
	    uartWriteByte( UART_USB, 'l' );  // Envia 'l'
       delay(250);
       ESTADO = ESPERANDO;
       break;

       case(PULSADO_CUATRO):
	   uartWriteByte( UART_USB, 'a' );  // Envia 'a'
       delay(250);
       ESTADO = ESPERANDO;
       break;


      }
 }



// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. 
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   // Inicializar UART_USB a 115200 baudios
   uartConfig( UART_USB, 115200 );

   gpioConfig( GPIO0, GPIO_INPUT );
   gpioConfig( GPIO1, GPIO_OUTPUT );

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {
	  PULSADOR_TO_SERIAL();
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
	return 0 ;
}
