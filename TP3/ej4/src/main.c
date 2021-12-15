/* 86.65 - Sistemas Embebidos
 * Grupo 2
 * Gayet - Mendoza - Massone
 * TP3 - Ejercicio 4 */

// Bibliotecas

#define SAPI_USE_INTERRUPTS

#include "sapi.h"       // <= sAPI header

// Definiciones y variables

#define LEVEL_SENSITIVITY 32 // 255/8 donde 8 son los estados de brillo
#define PERIOD_MS 20

static uint8_t levB = 0;
static uint8_t lev1 = 0;
static uint8_t lev2 = 0;
static uint8_t lev3 = 0;
static uint8_t *whichLEV = &levB;

void custom_PWM_cycle(uint8_t level, int T_cycle_MS, gpioMap_t gpio_n) {


	gpioWrite(gpio_n, true);
	delay((int)(level*T_cycle_MS)/255);

	gpioWrite(gpio_n, false);
	delay((int)((256-level)*T_cycle_MS)/255);

	return;
}

// Handler de interrupción por Rx
void onRx( void *noUsado)
{
    char c = uartRxRead( UART_USB );

    if(c == 'a') {
		*whichLEV -= LEVEL_SENSITIVITY;
		printf( "Recibimos <<%c>> por UART: nivel es %i\r\n", c, *whichLEV );
		return;
    }

    if(c == 'd') {
		*whichLEV += LEVEL_SENSITIVITY;
		printf( "Recibimos <<%c>> por UART: nivel es %i\r\n", c, *whichLEV );
		return;
    }

    return;
}

// Función que devuelve el estado de los botones en un BYTE
uint32_t Buttons_GetStatus_(void) { // @suppress("Type cannot be resolved")
	uint8_t ret = false; // @suppress("Type cannot be resolved")
	uint32_t idx; // @suppress("Type cannot be resolved")

	for (idx = 0; idx < 4; ++idx) {
		if (gpioRead( TEC1 + idx ) == 0) // @suppress("Symbol is not resolved")
			ret |= 1 << idx;
	}
	return ret;
}

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. 
int main(void){

   /* ------------- INICIALIZACIONES ------------- */

	gpioMap_t whichLED = LEDB;

	uint32_t buttonStatus;

   // Inicializar la placa
   boardConfig();

   // Inicializar UART_USB a 115200 baudios
   uartConfig( UART_USB, 115200 );

   // Inicializo el callback
   uartCallbackSet(UART_USB, UART_RECEIVE, onRx, NULL);

   // Habilito todas las interrupciones de UART_USB
   uartInterrupt(UART_USB, true);

   // Imprimo como se usa el programa
   uartWriteString(UART_USB, "Configuración realizada. Con \"d\" se aumenta el brillo, con \"a\" se disminuye.\n");

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {

	buttonStatus = Buttons_GetStatus_();

	if (buttonStatus == 1) {
		whichLED = LEDB;
		whichLEV = &levB;
	}

	if (buttonStatus == 2) {
		whichLED = LED1;
		whichLEV = &lev1;
	}

	if (buttonStatus == 4) {
		whichLED = LED2;
		whichLEV = &lev2;
	}

	if (buttonStatus == 8) {
		whichLED = LED3;
		whichLEV = &lev3;
	}

	custom_PWM_cycle(*whichLEV, PERIOD_MS, whichLED);
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
	return 0 ;
}
