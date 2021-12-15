# Ejercicio
En este ejercicio se implementó el control de los leds de la placa mediante teclas de la PC. A cada led se le asoció 2 teclas: una para prenderlo y otra para apagarlo. Como primera versión, se realizó mediante <i> polling</i>, de la siguiente manera: Una vez configurada la UART, como se explicó en el ejemplo anterior, dentro la función `void SERIAL_TO_PULSADOR(void)` se realiza la lectura de un byte de la UART mediante la función `uartReadByte()` y según el caracter leído se setea una variable que define qué led se quiere accionar. Luego, dependiendo cuál de los dos carácteres asociados a ese led se leyó, se prende o apaga el mismo. 

La función `SERIAL_TO_PULSADOR()` se ejecuta en el `main` del programa, dentro de un `while` infinito:

```{c}
int main(void){
  boardConfig();
  uartConfig(UART_USB, 115200);
  
  while(1) {
	   SERIAL_TO_PULSADOR();
   }
}
```

Como la lectura de la UART se realiza en cada iteración del `while`, esta implementación es por encuesta.

Luego se implementó nuevamente el mismo ejercicio, pero mediante interrpuciones. Para esto se utilizó como base el ejemplo `rx_interrupt`. 
En primer lugar se debió habilitar la utilización de las funciones asociadas a las interrupciones de la UART, mediante la definición de una macro en los archivos `sapi_uart.c` y `sapi_uart.h`, los cuales se encuentran en `firmware_v3/libs/sapi/sapi_v0.6.2/soc/peripherals/src/` y `firmware_v3/libs/sapi/sapi_v0.6.2/soc/peripherals/inc/` respectivamente. En ambos archivos se incluyó la siguiente línea de código:

```{c}
#define SAPI_USE_INTERRUPTS
```

Esta misma habilitación se puede realizar mediante la edición del archivo `config.mk`.

Para configurar la interrupción se utilizaron las siguientes funciones:

```{c}
uartCallbackSet(UART_USB, UART_RECEIVE, onRx, NULL);
uartInterrupt(UART_USB, true);
```
las cuales se encuentran definidas en `sapi_uart.c`. La función `uartCallbackSet()` se utilizó para asociar a la interrupción producida por el evento `UART_RECIEVE` en la UART USB, la función `onRx`, es decir, al saltar dicha interrupción, se debe ejecutar dicha función. El parámetro `NULL` indica que la función `onRx` no recibe ningún parámetro.

Para habilitar la interrupción propiamente dicha se utilizó la función `uartInterrupt()`, seteando el valor de la variable `enable` como `true`. 

En la función `onRx()` se raliza la lectura de la uart y según el carácter leído se cambia el valor de una variable <i> global </i> que selecciona el led a accionar:

```{c}
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
```

Se modificó la función `SERIAL_TO_PULSADOR()` para que únicamente prenda/apague los leds, en vez de además leer de la UART USB. 
Dentro de este programa también se incluyó el ejercicio 2, el cual prende o apaga un led y envía un carácter por la UART al pulsarse un botón, pero se optó por modificarlo, para que al pulsar (presionar+soltar) un botón se realice el <i> toggle </i> de un led. 

Se modificó entonces la función `PULSADOR_TO_SERIAL` en cuanto a cómo acciona los leds, pero manteniendo la escritura a la UART medinte `uartWriteByte()`:

```{c}
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
```

De esta forma se combiaron ambas funcionalidades en el mismo programa, evitando que una intervenga en la fucionalidad de la otra.
