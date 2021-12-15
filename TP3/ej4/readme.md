# Ejercicio 4

A partir del programa de ejemplo rgb_led_uart:

a. Armar un PWM por software para disponer de distintos niveles de brillo en cada uno de los leds.

b. Por medio de la terminal serie implementada con el puerto USB de debug, asignar un caracter para incrementar y otro para
decrementar el brillo de 1 led.

# Código

Se armó un programa principal que ejecuta un PWM personalizado, codificado por software. Los parámetros de este PWM son modificados por la UART, generando interrupciones al recibir datos por RX. El programa principal tiene las siguientes partes:

Inicializaciones: placa, uart, función handler de interrupciones por uart, interrupciones por uart habilitadas, mensaje inicial.

```c
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
```

Se puede ver que se inicializa la UART USB, a 115200 baudios, con una función handler de interrupciones llamada `onRx`. Esta función se ejecutará cada vez que se tenga un dato en el buffer de Rx de la UART, como lo indica la inicialización de interrupciones `... UART_RECIEVE, onRx ...`

Luego se entra a la región cíclica de codigo en la cual se muestrean los botones de la placa (para saber sobre qué LED se quiere cambiar el brillo) utilizando un puntero para almacenar la dirección en la que se encuentra el nivel de cada LED.

```c
buttonStatus = Buttons_GetStatus_();

if (buttonStatus == 1) {
  whichLED = LEDB;
  whichLEV = &levB;
}
```

donde `Buttons_GetStatus_()` es una función que guarda en 4 bits los estados de los botones (el estado de cada botón en cada bit).

Luego de esto se ejecuta la función de PWM utilizando el nivel del LED escogido.

```c
custom_PWM_cycle(*whichLEV, PERIOD_MS, whichLED);
```
donde `whichLEV` es la variable que almacena la dirección del NIVEL de brillo del LED escogido, de forma que `*whichLEV` es el nivel actual de brillo. `PERIOD_MS` es una constante que tiene que ver con la frecuencia a la que se quiere que funcione el PWM. `whichLED` indica que LED fue escogido.

El código de esta función es el siguiente:

```c
void custom_PWM_cycle(uint8_t level, int T_cycle_MS, gpioMap_t gpio_n) {

	gpioWrite(gpio_n, true);
	delay((int)(level*T_cycle_MS)/255);

	gpioWrite(gpio_n, false);
	delay((int)((256-level)*T_cycle_MS)/255);

	return;
}
```
que mantiene el LED en cuestión encendido durante una porción del período, y apagado durante la otra porción. En este caso se decidió usar arbitrariamente un entero de 8 bits, que podría tranquilamente ser reemplazado para lograr mayor precisión.

Por último para modificar el nivel de brillo actual se interrumpe el programa principal para aumentar o disminuir la variable `level` mediante interrupciones de recepción por UART (UART_RECIEVE). Esto se maneja dentro de la función handler `onRx`.

```c
void onRx(void *noUsado)
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
```

notar que el nivel de brillo modificado es aquel apuntado por `whichLEV` que depende del boton oprimido (LED seleccionado). Se aumenta en una cantidad fija, `LEVEL_SENSITIVITY` que por default es 32.
