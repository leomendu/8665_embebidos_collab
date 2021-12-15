// 86.65 - Sistemas Embebidos 
// Grupo 2
// Gayet - Mendoza - Massone

#define TEST TP3_2

////////////////////////////////////////////////////////////////////////////////////////////////////
#if (TEST == TP3_2)
//Biblioteca
  #include "sapi.h"       // <= sAPI header
// Definiciones y variables 
int ESTADO = 0;                                    //Variable de estado control (Maquina de Estados)
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

   // Inicializar la placa */
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
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
#if (TEST == TP3_3a)
 #include "sapi.h"       // <= sAPI header

/* Definiciones y variables */
int ESTADO = 0;                                    //Variable de estado control (Maquina de Estados)
bool_t valor; 													// Variable para almacenar el valor de tecla leido
uint8_t dato  = 0;
enum{
	ESPERANDO,
	LED_UNO,
	LED_DOS,
	LED_TRES,
 };

/* FUNCIONES */
 void SERIAL_TO_PULSADOR(void){

 switch(ESTADO){

       case(ESPERANDO):
	   uartReadByte( UART_USB, &dato);

       if(dato == 'a' || dato == 'q'){
    	   ESTADO =LED_UNO;
    	   return;}

       if(dato == 's'  || dato == 'w'){
           ESTADO =LED_DOS;
           return;}

       if(dato == 'd'  || dato == 'e'){
    	   ESTADO =LED_TRES;
    	   return;}
       break;

       case(LED_UNO):
		if(dato == 'q'){
			valor = 1;
			gpioWrite( LED1, valor );
			}

       if(dato == 'a'){
       		valor = 0;
       		gpioWrite( LED1, valor );
       		}
       ESTADO = ESPERANDO;
       break;

       case(LED_DOS):
	   if(dato == 'w'){
	   valor = 1;
	   gpioWrite( LED2, valor );
	   }

	   if(dato == 's'){
	   valor = 0;
 	   gpioWrite( LED2, valor );
	   }
	   ESTADO = ESPERANDO;
       break;

       case(LED_TRES):
		if(dato == 'e'){
	    valor = 1;
	    gpioWrite( LED3, valor );
	    }

	    if(dato == 'd'){
		valor = 0;
		gpioWrite( LED3, valor );
	    }
	   ESTADO = ESPERANDO;
       break;
      }
 }

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   // Inicializar la placa */
   /* Inicializar la placa */
   boardConfig();
   // Inicializar UART_USB a 115200 baudios
   uartConfig( UART_USB, 115200 );
   gpioConfig( GPIO0, GPIO_INPUT );
   gpioConfig( GPIO1, GPIO_OUTPUT );

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {
	   SERIAL_TO_PULSADOR();
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
	return 0 ;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
#if (TEST == TP3_3b) //NO ME ANDUVO INTERRUPCION :(
 #include "sapi.h"       // <= sAPI header

/* Definiciones y variables */
int ESTADO = 0;                                                  //Variable de estado control (Maquina de Estados)
bool_t valor;                                      // Variable para almacenar el valor de tecla leido
char data =0;

enum{
   ESPERANDO,
   LED_UNO,
   LED_DOS,
   LED_TRES,
 };

/* FUNCIONES */

void onRx( void *noUsado)
{
    char c = uartRxRead( UART_USB );
    printf( "Recibimos <<%c>> por UART\r\n", c );
    if(c == "a" || c == "q"){
            ESTADO =LED_UNO;
            return;}

          if(c == "s"  || c == "w"){
              ESTADO =LED_DOS;
              return;}

          if(c == "d"  || c == "e"){
            ESTADO =LED_TRES;
            return;}
}


void SERIAL_TO_PULSADOR(void){

 switch(ESTADO){

       case(ESPERANDO):

       if(data == "a" || data == "q"){
         ESTADO =LED_UNO;
         return;}

       if(data == "s"  || data == "w"){
           ESTADO =LED_DOS;
           return;}

       if(data == "d"  || data == "e"){
         ESTADO =LED_TRES;
         return;}
       break;

       case(LED_UNO):
      if(data == "q"){
         valor = 1;
         gpioWrite( LED1, valor );
         }

       if(data == "a"){
            valor = 0;
            gpioWrite( LED1, valor );
            }
       ESTADO = ESPERANDO;
       break;

       case(LED_DOS):
      if(data == "w"){
      valor = 1;
      gpioWrite( LED2, valor );
      }

      if(data == "s"){
      valor = 0;
      gpioWrite( LED2, valor );
      }
      ESTADO = ESPERANDO;
       break;

       case(LED_TRES):
      if(data == "e"){
       valor = 1;
       gpioWrite( LED3, valor );
       }

       if(data == "d"){
      valor = 0;
      gpioWrite( LED3, valor );
       }
      ESTADO = ESPERANDO;
       break;
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
      SERIAL_TO_PULSADOR();
      gpioToggle(LEDB);
      delay(1000);
   }
   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

#endif


///////////////////////////////////////////////////////////////////
#if (TEST == TP3_3c)

   #include "sapi.h"       // <= sAPI header
// Definiciones y variables 
int ESTADO = 0;                                    //Variable de estado control (Maquina de Estados)
bool_t valor;                                      // Variable para almacenar el valor de tecla leido
uint8_t dato  = 0;
enum{
   ESPERANDO,
   LED_UNO,
   LED_DOS,
   LED_TRES,
 };
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

/* FUNCIONES */
 void SERIAL_TO_PULSADOR(void){

 switch(ESTADO){

       case(ESPERANDO):
      uartReadByte( UART_USB, &dato);

       if(dato == 'a' || dato == 'q'){
         ESTADO =LED_UNO;
         return;}

       if(dato == 's'  || dato == 'w'){
           ESTADO =LED_DOS;
           return;}

       if(dato == 'd'  || dato == 'e'){
         ESTADO =LED_TRES;
         return;}
       break;

       case(LED_UNO):
      if(dato == 'q'){
         valor = 1;
         gpioWrite( LED1, valor );
         }

       if(dato == 'a'){
            valor = 0;
            gpioWrite( LED1, valor );
            }
       ESTADO = ESPERANDO;
       break;

       case(LED_DOS):
      if(dato == 'w'){
      valor = 1;
      gpioWrite( LED2, valor );
      }

      if(dato == 's'){
      valor = 0;
      gpioWrite( LED2, valor );
      }
      ESTADO = ESPERANDO;
       break;

       case(LED_TRES):
      if(dato == 'e'){
       valor = 1;
       gpioWrite( LED3, valor );
       }

       if(dato == 'd'){
      valor = 0;
      gpioWrite( LED3, valor );
       }
      ESTADO = ESPERANDO;
       break;
      }
 }


// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. 
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   // Inicializar la placa */
   /* Inicializar la placa */
   boardConfig();
   // Inicializar UART_USB a 115200 baudios
   uartConfig( UART_USB, 115200 );
   gpioConfig( GPIO0, GPIO_INPUT );
   gpioConfig( GPIO1, GPIO_OUTPUT );

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {
     PULSADOR_TO_SERIAL();
     SERIAL_TO_PULSADOR();
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

#endif

///////////////////////////////////////////////////////////////////
#if (TEST == TP3_4) //No me anduvo el codigo

 /*  Aca va el codigo del inciso 
  *
  *
  */

#endif

///////////////////////////////////////////////////////////////////
#if (TEST == TP3_5)

 //a) Tiro por serial 1023
 //b) Tiro por serial 0
 //c) Tiro por serial 512
#endif


///////////////////////////////////////////////////////////////////
#if (TEST == TP3_6a)

#include "sapi.h"        // <= sAPI header
uint8_t serial_val  = 0;
uint16_t val = 0;

//Funciones
char* itoa(int value, char* result, int base) {
   // check that the base if valid
   if (base < 2 || base > 36) { *result = '\0'; return result; }

   char* ptr = result, *ptr1 = result, tmp_char;
   int tmp_value;

   do {
      tmp_value = value;
      value /= base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
   } while ( value );

   // Apply negative sign
   if (tmp_value < 0) *ptr++ = '-';
   *ptr-- = '\0';
   while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
   }
   return result;
}


void DAC_SERIAL_LEVEL(void){
   uartReadByte( UART_USB, &serial_val);
   if(serial_val == '0'){val =0;}      //0V
   if(serial_val == '1'){val =113;} //0.36V
   if(serial_val == '2'){val =226;} //0.72V
   if(serial_val == '3'){val =339;} //1.09V
   if(serial_val == '4'){val =452;} //1.46V
   if(serial_val == '5'){val =565;} //1.81V
   if(serial_val == '6'){val =678;} //2.18V
   if(serial_val == '7'){val =791;} //2.54V
   if(serial_val == '8'){val =904;} //2.90V
   if(serial_val == '9'){val =1023;}   //3.28V
   }



/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar UART_USB a 115200 baudios */
   uartConfig( UART_USB, 115200 );

   /* Inicializar AnalogIO */
   /* Posibles configuraciones:
    *    ADC_ENABLE,  ADC_DISABLE,
    *    ADC_ENABLE,  ADC_DISABLE,
    */
   adcConfig( ADC_ENABLE ); /* ADC */
   dacConfig( DAC_ENABLE ); /* DAC */

   /* ConfiguraciÃ³n de estado inicial del Led */
   bool_t ledState1 = OFF;

   /* Contador */
   uint32_t i = 0;

   /* Buffer */
   static char uartBuff[10];

   /* Variable para almacenar el valor leido del ADC CH1 */
   uint16_t muestra = 0;

   /* Variables de delays no bloqueantes */
   delay_t delay1;
   delay_t delay2;

   /* Inicializar Retardo no bloqueante con tiempo en ms */
   delayConfig( &delay1, 500 );
   delayConfig( &delay2, 200 );

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {

      /* delayRead retorna TRUE cuando se cumple el tiempo de retardo */
      if ( delayRead( &delay1 ) ){

         /* Leo la Entrada Analogica AI0 - ADC0 CH1 */
         //muestra = adcRead( CH4 );
        muestra = adcRead( CH1 );
         /* EnvÃ­o la primer parte del mnesaje a la Uart */
         uartWriteString( UART_USB, "ADC CH1 value: " );

         /* ConversiÃ³n de muestra entera a ascii con base decimal */
         itoa( muestra, uartBuff, 10 ); /* 10 significa decimal */

         /* Enviar muestra y Enter */
         uartWriteString( UART_USB, uartBuff );
         uartWriteString( UART_USB, ";\r\n" );


         //Escribo el valor acorde al serial
         DAC_SERIAL_LEVEL();
         /* Escribo la muestra en la Salida AnalogicaAO - DAC */
         dacWrite( DAC, val );
      }

      /* delayRead retorna TRUE cuando se cumple el tiempo de retardo */
      if ( delayRead( &delay2 ) ){
         ledState1 = !ledState1;
         gpioWrite( LED1, ledState1 );

         /* Si pasaron 20 delays le aumento el tiempo */
         i++;
         if( i == 20 )
            delayWrite( &delay2, 1000 );
      }

   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

#endif


///////////////////////////////////////////////////////////////////
#if (TEST == TP3_6b) 
#include "sapi.h"        // <= sAPI header
uint8_t serial_val  = 0;
uint16_t val = 0;

//Funciones
char* itoa(int value, char* result, int base) {
   // check that the base if valid
   if (base < 2 || base > 36) { *result = '\0'; return result; }

   char* ptr = result, *ptr1 = result, tmp_char;
   int tmp_value;

   do {
      tmp_value = value;
      value /= base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
   } while ( value );

   // Apply negative sign
   if (tmp_value < 0) *ptr++ = '-';
   *ptr-- = '\0';
   while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
   }
   return result;
}


void DAC_SERIAL_LEVEL(void){
   uartReadByte( UART_USB, &serial_val);
   if(serial_val == '0'){val =0;}      //0V
   if(serial_val == '1'){val =113;} //0.36V
   if(serial_val == '2'){val =226;} //0.72V
   if(serial_val == '3'){val =339;} //1.09V
   if(serial_val == '4'){val =452;} //1.46V
   if(serial_val == '5'){val =565;} //1.81V
   if(serial_val == '6'){val =678;} //2.18V
   if(serial_val == '7'){val =791;} //2.54V
   if(serial_val == '8'){val =904;} //2.90V
   if(serial_val == '9'){val =1023;}   //3.28V
   }



/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar UART_USB a 115200 baudios */
   uartConfig( UART_USB, 115200 );

   /* Inicializar AnalogIO */
   /* Posibles configuraciones:
    *    ADC_ENABLE,  ADC_DISABLE,
    *    ADC_ENABLE,  ADC_DISABLE,
    */
   adcConfig( ADC_ENABLE ); /* ADC */
   dacConfig( DAC_ENABLE ); /* DAC */

   /* ConfiguraciÃ³n de estado inicial del Led */
   bool_t ledState1 = OFF;

   /* Contador */
   uint32_t i = 0;

   /* Buffer */
   static char uartBuff[10];

   /* Variable para almacenar el valor leido del ADC CH1 */
   uint16_t muestra = 0;

   /* Variables de delays no bloqueantes */
   delay_t delay1;
   delay_t delay2;

   /* Inicializar Retardo no bloqueante con tiempo en ms */
   delayConfig( &delay1, 500 );
   delayConfig( &delay2, 200 );

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {

      /* delayRead retorna TRUE cuando se cumple el tiempo de retardo */
      if ( delayRead( &delay1 ) ){

         /* Leo la Entrada Analogica AI0 - ADC0 CH1 */
         //muestra = adcRead( CH4 );
        muestra = adcRead( CH1 );
         /* EnvÃ­o la primer parte del mnesaje a la Uart */
         uartWriteString( UART_USB, "ADC CH1 value: " );

         /* ConversiÃ³n de muestra entera a ascii con base decimal */
         itoa( muestra, uartBuff, 10 ); /* 10 significa decimal */

         /* Enviar muestra y Enter */
         uartWriteString( UART_USB, uartBuff );
         uartWriteString( UART_USB, ";\r\n" );


         //Escribo el valor acorde al serial
         DAC_SERIAL_LEVEL();
         /* Escribo la muestra en la Salida AnalogicaAO - DAC */
         dacWrite( DAC, val );
      }

      /* delayRead retorna TRUE cuando se cumple el tiempo de retardo */
      if ( delayRead( &delay2 ) ){
         ledState1 = !ledState1;
         gpioWrite( LED1, ledState1 );

         /* Si pasaron 20 delays le aumento el tiempo */
         i++;
         if( i == 20 )
            delayWrite( &delay2, 1000 );
      }

   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}  

#endif

///////////////////////////////////////////////////////////////////
#if (TEST == TP3_7) 

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
   if(dato=='1'){ESTADO =PULSADO_UNO;}
   if(dato=='2'){ESTADO =PULSADO_DOS;}
   if(dato=='3'){ESTADO =PULSADO_TRES;}
   if(dato=='4'){ESTADO =PULSADO_CUATRO;}
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
      gpioWrite( LED2, 1 );
         delay(500);
         ESTADO=ESPERANDO;
       break;

   case(PULSADO_DOS):
      gpioWrite( LED3, 1 );
      delay(500);
      ESTADO=ESPERANDO;
       break;

   case(PULSADO_TRES):
      gpioWrite( LED3, 1 );
       delay(500);
      ESTADO=ESPERANDO;
       break;

   case(PULSADO_CUATRO):
      gpioWrite( LEDB, 1 );
      delay(500);
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

#endif

///////////////////////////////////////////////////////////////////
#if (TEST == TP3_8) 

 /*  Aca va el codigo del inciso
  *
  *
  */

#endif
///////////////////////////////////////////////////////////////////