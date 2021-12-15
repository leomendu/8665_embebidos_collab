/* Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Date: 2016-04-26
 */

/*==================[inclusions]=============================================*/

#include "sapi.h"        // <= sAPI header

static char serial_val  = 0;
static uint16_t val = 0;

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/


/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.

 */
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
	serial_val = uartRxRead( UART_USB );
   uartWriteString( UART_USB, "ADC CH1 value to be set: " );
   uartWriteByte(UART_USB, serial_val);
   uartWriteString( UART_USB, "\n" );
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
   uartCallbackSet(UART_USB, UART_RECEIVE, DAC_SERIAL_LEVEL, NULL);
    // Habilito todas las interrupciones de UART_USB
    uartInterrupt(UART_USB, true);

   /* Inicializar AnalogIO */
   /* Posibles configuraciones:
    *    ADC_ENABLE,  ADC_DISABLE,
    *    ADC_ENABLE,  ADC_DISABLE,
    */
   adcConfig( ADC_ENABLE ); /* ADC */
   dacConfig( DAC_ENABLE ); /* DAC */

   /* Configuración de estado inicial del Led */
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
         muestra = adcRead( CH1 );

         /* Envío la primer parte del mnesaje a la Uart */
         uartWriteString( UART_USB, "ADC CH1 value: " );

         /* Conversión de muestra entera a ascii con base decimal */
         itoa( muestra, uartBuff, 10 ); /* 10 significa decimal */

         /* Enviar muestra y Enter */
         uartWriteString( UART_USB, uartBuff );
         uartWriteString( UART_USB, ";\r\n" );

         /* Escribo la muestra en la Salida AnalogicaAO - DAC */
         DAC_SERIAL_LEVEL();
         dacWrite( DAC, val);
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

/*==================[end of file]============================================*/
