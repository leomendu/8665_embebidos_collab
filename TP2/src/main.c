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

//#define GRUPO 2 				// grupo dos altera el orden de los LEDX y TECX

#include "sapi.h"              // <= sAPI header

/*==================[macros and definitions]=================================*/

#define TICKRATE_MS 50
#define LED_TOGGLE_MS 500

typedef enum {
	EJ_SWITCHES_LEDS,
	EJ_TICK_HOOK,
	EJ_UART,
} EJ;

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

EJ ejercicio = EJ_UART;

#if EJ == EJ_UART
DEBUG_PRINT_ENABLE
#endif

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

/* FUNCION que se ejecuta cada vez que ocurre un Tick. */
void myTickHook( void *ptr )
{
   static bool_t ledState = OFF;

   gpioMap_t led = (gpioMap_t)ptr;

   if( ledState ) {
      ledState = OFF;
   } else {
      ledState = ON;
   }
   gpioWrite( led, ledState );
}

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   switch (ejercicio) {
   case EJ_SWITCHES_LEDS:

	    /* GPIO config */
		gpioConfig( GPIO0, GPIO_INPUT );
		gpioConfig( GPIO1, GPIO_OUTPUT );

		/* Variable para almacenar el valor de tecla leido */
		bool_t valor;

		/* ------------- REPETIR POR SIEMPRE ------------- */
		while(1) {

			valor = !gpioRead(TEC1);
			gpioWrite(LEDB, valor);

			valor = !gpioRead(TEC2);
			gpioWrite(LED1, valor);

			valor = !gpioRead(TEC3);
			gpioWrite(LED2, valor);

			valor = !gpioRead(TEC4);
			gpioWrite(LED3, valor);

			valor = !gpioRead(GPIO0);
			gpioWrite(GPIO1, valor);

			}
		break;

   case EJ_TICK_HOOK:
		/* Inicializar el conteo de Ticks con resolucion de 50ms (se ejecuta
		periodicamente una interrupcion cada 50ms que incrementa un contador de
		Ticks obteniendose una base de tiempos). */

		tickConfig(TICKRATE_MS);

		/* Se agrega ademas un "tick hook" nombrado myTickHook. El tick hook es
		simplemente una funcion que se ejecutara peri�odicamente con cada
		interrupcion de Tick, este nombre se refiere a una funcion "enganchada"
		a una interrupcion.
		El segundo parametro es el parametro que recibe la funcion myTickHook
		al ejecutarse. En este ejemplo se utiliza para pasarle el led a titilar.
		*/

		tickCallbackSet(myTickHook, (void*)LEDR);
		delay(LED_TOGGLE_MS);

		/* ------------- REPETIR POR SIEMPRE ------------- */
		while(1) {
			tickCallbackSet(myTickHook, (void*)LEDG);
			delay(LED_TOGGLE_MS);
			tickCallbackSet(myTickHook, (void*)LEDB);
			delay(LED_TOGGLE_MS);
			tickCallbackSet(myTickHook, (void*)LED1);
			delay(LED_TOGGLE_MS);
			tickCallbackSet(myTickHook, (void*)LED2);
			delay(LED_TOGGLE_MS);
			tickCallbackSet(myTickHook, (void*)LED3);
			delay(LED_TOGGLE_MS);
			tickCallbackSet(myTickHook, (void*)LEDR);
			delay(LED_TOGGLE_MS);
		}
		break;

   case EJ_UART:

	   // Inicializar UART_USB como salida Serial de debug
		debugPrintConfigUart(UART_USB, 115200);
		debugPrintlnString("DEBUG: UART_USB configurada.");

		// Crear varias variables del tipo booleano
		bool_t oldStatusTEC3 = OFF;
		bool_t StatusTEC3 = OFF;
		bool_t oldStatusTEC4 = OFF;
		bool_t StatusTEC4 = OFF;
		bool_t ledbValue = OFF;
		char string[13];
		// Variable de seguimiento
		int i = 0;

		// ---------- REPETIR POR SIEMPRE --------------------------
		while(TRUE) {
			// Guardar estado anterior de pulsadores
			oldStatusTEC3 = StatusTEC3;
			oldStatusTEC4 = StatusTEC4;
			// Leer valor del pulsador e invertir
			StatusTEC3 = !gpioRead(TEC3);
			StatusTEC4 = !gpioRead(TEC4);
			// Reseteo la variable de seguimiento
			if(i >= 6) i = 0;

			if(StatusTEC3 && !oldStatusTEC3) {
			// Prender siguiente LED si se presiona TEC3
			gpioWrite(LEDR+i++, !gpioRead(TEC3));
			debugPrintlnString("LED encendido");
			}
			else if(StatusTEC4 && !oldStatusTEC4) {
			// Apagar siguiente LED si se presiona TEC4
			gpioWrite(LEDR+i++, gpioRead(TEC4));
			debugPrintlnString("LED apagado");
			}

			// Retardo bloqueante durante 100ms
			delay(100);
		}

	    break;

   default:

	    break;
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
	return 0;
}

/*==================[end of file]============================================*/
