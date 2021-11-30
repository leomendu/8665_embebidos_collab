# Introducción

Todos los ejercicios fueron integrados en un mismo archivo fuente `main.c`. Para poder elegir que programa se corre, se definió un nuevo tipo enumerativo que selecciona el ejercicio que se quiera correr. Esto se definió de la siguiente forma:

```c
typedef enum {
  EJ_SWITCHES_LEDS,
  EJ_TICK_HOOK,
  EJ_UART,
} EJ;
```

De forma que la siguiente sentencia...

```c
EJ ejercicio = EJ_SWITCHES_LEDS;
```
elija correr el código correspondiente a ese ejercicio. Esto se logra haciendo un `switch` de la variable `ejercicio` en el main. 

# Ejercicio 2: gpio_switches_leds

Para correr este ejercicio es necesario inicializar la variable `ejercicio` de la siguiente forma:

```c
EJ ejercicio = EJ_SWITCHES_LEDS;
```

Mediante el uso de compilación condicional, se puede cambiar rápidamente el funcionamiento del código. En este caso, se define la constante `GRUPO` de la siguiente manera:

```c
#define GRUPO 2
```

Luego mediante un `#ifndef` se puede alterar el funcionamiento de distintas macros como las posiciones de los puertos y pines de LEDS, o pines de los pulsadores TECx.

En este caso se "desplazaron" dos posiciones los LEDs definidos, y una posición los pulsadores, dentro de sapi_peripheral_map.h. Esto fue necesario en lugar de desplazar pulsadores y LEDs dos posiciones, porque al haber 4 LEDs fisicos y 4 pulsadores, el cambio en el código no reflejaba ningún cambio en el comportamiento de la placa, ya que desplazar dos posiciones a la derecha los LEDs y dos posiciones a la derecha los pulsadores TECx se obtenía el mismo comportamiento que si no se hubiese modificado nada.

Este cambio de comportamiento se logro mediante código condicional en el preprocesador:

```c
#ifndef GRUPO
  ... realizar lo de siempre ...
#elif (GRUPO == 2)
  ... cambiar orden de pulsadores y LEDs ...
#endif
```

De esta forma si no se define la constante `GRUPO`, no se cambia el comportamiento de los periféricos definidos por sAPI.

# Ejercicio 3: ticks_tickHook

En este ejercicio se busca configurar y controlar rutinas de interrupción utilizando el timer por defecto de la placa. En este caso llamado `SysTick`. Para correr este ejercicio es necesario inicializar la variable `ejercicio` de la siguiente forma:

```c
EJ ejercicio = EJ_TICK_HOOK;
```

Se comienza configurando cada cuanto tiempo se dispara una interrupción por timer:

```c
tickConfig(TICKRATE_MS);
```

Donde `TICKRATE_MS` es el valor en milisegundos que tarda en disparar una interrupción por timer. Luego de esa configuración se debe especificar que función es ejecutada cada vez que surge una interrupción, eso se hace de la siguiente manera:


```c
tickCallbackSet(myTickHook, (void*)LEDR);
```

Donde `myTickHook` es la función a ser ejecutada y `(void*)LEDR` son los argumentos con los cuales se ejecuta la función. En este caso la función que se definió es un toggle de algun LED. Como se especifica a continuación:


```c
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
```
# Ejercicio 4: uart

En este ejercicio se busca configurar y controlar la comunicacion serie (`uart`) por medio de la `uart_USB`. Las funciones utilizadas para el envío de mensajes de depuración por puerto serie son `debugPrintConfigUart(…)` y `debugPrintString(…)`. Ambas pertenecen a la librería sapi_debugPrint.h, ubicada en `\firmware_v3\libs\sapi\sapi_v0.6.2\abstract_modules\inc\sapi_debugPrint.h`

Donde se encuentran las siguientes declaraciones:
```c 
#define debugPrintConfigUart(uart,baudRate)     
printConfigUart(&(debugPrint),(uart),(baudRate))
```
y

```c
#define debugPrintString(string)                
printString((debugPrint),(string))
```
Donde vemos que en realidad se están llamando implícitamente a las funciones ``printConfigUart(…)`` y ``printString(…)`` que se encuentran definidas en `\firmware_v3\libs\sapi\sapi_v0.6.2\abstract_modules\src\sapi_print.c`

Dentro de los parámetros que recibe `printConfigUart` se encuentra el tipo de dato `uartMap_t`, que está definido de la siguiente forma:

```c
typedef enum{
  UART_GPIO = 0, // Hardware UART0 via GPIO1(TX), GPIO2(RX) pins on header P0
	UART_485  = 1, // Hardware UART0 via RS_485 A, B and GND Borns
                          // Hardware UART1 not routed
	UART_USB  = 3, // Hardware UART2 via USB DEBUG port
	UART_ENET = 4, // Hardware UART2 via ENET_RXD0(TX), ENET_CRS_DV(RX) pins on header P0
	UART_232  = 5, // Hardware UART3 via 232_RX and 232_tx pins on header P1
} uartMap_t;
```
Donde se define la vía por la cual se establecerá la comunicación. En este caso, se elige la opción ``c UART_USB  = 3``, es decir que la comunicación será a través del puerto `USB DEBUG.` También recibe como parámetro el baud rate, que marcará la tasa con que se transmita la información y está tipificado como un entero de 32 bits.

Luego esta función llama a su vez a otra función denominada ``c uartConfig(uart, baudRate) ``, donde se especifican los números de puerto y de pin para transmisión y recepción para cada UART. 

La función `printString` recibe entre sus parámetros un puntero al inicio de una cadena de caracteres y llama a la función ``c uartWriteString( printer, string )``. Se ve que la función recibe la cadena de caracteres y la vía de comunicación e invoca a la función ``c uartWriteByte`` hasta que se termine de escribir la cadena entera. La función ``c uartWriteByte`` está definida en el mismo lugar y lo que hace es recibir de a un byte por vez y escribirlo.



