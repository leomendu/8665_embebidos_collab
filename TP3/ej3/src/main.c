/* 86.65 - Sistemas Embebidos
 * Grupo 2
 * Gayet - Mendoza - Massone
 * TP3 - Ejercicio 3 */

// Bibliotecas

#include "sapi.h"

static char data = 'a';

//Variable de estado control (Maquina de Estados)
bool_t valor; 													// Variable para almacenar el valor de tecla leido

typedef enum{
   ESPERANDO,
   LED_UNO,
   LED_DOS,
   LED_TRES,
 }estado_t;

static estado_t ESTADO = 0;


// Funcion de ejecucion de la interrupcion UART
void onRx( void *noUsado)
{
    volatile char c = uartRxRead( UART_USB );
    printf( "Recibimos <<%c>> por UART\r\n", c );
    if(c == 'a' || c == 'q'){
            ESTADO =LED_UNO;
            data = c;
            return;}

    if(c == 's'  || c == 'w'){
              ESTADO =LED_DOS;
              data = c;
              return;}

   if(c == 'd'  || c == 'e'){
            ESTADO =LED_TRES;
            data = c;
            return;}
   return;
}


void SERIAL_TO_PULSADOR(void){

 switch(ESTADO){

       case(ESPERANDO):
		//uartWriteString( UART_USB, "Esperando\n" );
    	break;

       case(LED_UNO):
      if(data == 'q'){
         valor = 1;
         gpioWrite( LED1, valor );
         }

       if(data == 'a'){
            valor = 0;
            gpioWrite( LED1, valor );
            }
       ESTADO = ESPERANDO;
       break;

       case(LED_DOS):
      if(data == 'w'){
      valor = 1;
      gpioWrite( LED2, valor );
      }

      if(data == 's'){
      valor = 0;
      gpioWrite( LED2, valor );
      }
      ESTADO = ESPERANDO;
       break;

       case(LED_TRES):
      if(data == 'e'){
       valor = 1;
       gpioWrite( LED3, valor );
       }

       if(data == 'd'){
      valor = 0;
      gpioWrite( LED3, valor );
       }
      ESTADO = ESPERANDO;
       break;
      }
 }

void PULSADOR_TO_SERIAL(void){
	static int val_tec_1_old = 0;
	static int val_tec_2_old = 0;
	static int val_tec_3_old = 0;
	static int val_tec_4_old = 0;


	int val_tec_1 = !gpioRead(TEC1);

	if (val_tec_1 == 1 && val_tec_1_old == 0){
		uartWriteByte( UART_USB, 'H' );
		gpioToggle( LEDR);
	}

	int val_tec_2 = !gpioRead(TEC2);
	if (val_tec_2 == 1 && val_tec_2_old == 0){
		uartWriteByte( UART_USB, 'o' );
		gpioToggle( LED1);
	}


	int val_tec_3 = !gpioRead(TEC3);
	if (val_tec_3 == 1 && val_tec_3_old == 0){
		uartWriteByte( UART_USB, 'l' );
		gpioToggle( LED2);
	}


	int val_tec_4 = !gpioRead(TEC4);
	if (val_tec_4 == 1 && val_tec_4_old == 0){
		uartWriteByte( UART_USB, 'a' );
		gpioToggle( LED3);
	}

 }


/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */
   /* Inicializar la placa */
   boardConfig();

    /* Inicializar la UART_USB junto con las interrupciones de Tx y Rx */
   uartConfig(UART_USB, 115200);
   // Seteo un callback al evento de recepcion y habilito su interrupcion
   uartCallbackSet(UART_USB, UART_RECEIVE, onRx, NULL);
    // Habilito todas las interrupciones de UART_USB
   uartInterrupt(UART_USB, true);
   gpioConfig( GPIO0, GPIO_INPUT );
   gpioConfig( GPIO1, GPIO_OUTPUT );

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(TRUE) {
	  //uartWriteByte(UART_USB, data);
      SERIAL_TO_PULSADOR();
      delay(1000);
      PULSADOR_TO_SERIAL();
      gpioToggle(LEDB);
   }
   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}
