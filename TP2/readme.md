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

En este caso se "desplazaron" dos posiciones los LEDs definidos, y una posición los pulsadores, dentro de `sapi_peripheral_map.h`, el cual se encuentra dentro de `firmware_v3/libs/sapi/sapi_v0.6.2/board/inc/`. Esto fue necesario en lugar de desplazar pulsadores y LEDs dos posiciones, porque al haber 4 LEDs fisicos y 4 pulsadores, el cambio en el código no reflejaba ningún cambio en el comportamiento de la placa, ya que desplazar dos posiciones a la derecha los LEDs y dos posiciones a la derecha los pulsadores TECx se obtenía el mismo comportamiento que si no se hubiese modificado nada.

Este cambio de comportamiento se logro mediante código condicional en el preprocesador, dentro de la definición del tipo enumerativo `gpioMap_t` el cual mapea los pines del GPIO a los nombres utilizados en el código para referenciarlos:

```c
typedef enum {

   // Configure GPIO pins for each board

   #if (BOARD == ciaa_nxp)
      DOUT0_C = -3, DOUT1_C = -3, DOUT2_C = -3, DOUT3_C = -3, DIN_COM = -3,
      VCC = -2, VIN_24V = -2, VOUT_24V = -2,
      GND = -1, GNDA0 = -1, GNDA1 = -1, GND0 = -1, GND1 = -1, GND2 = -1, GND3 = -1, 
      // Born digital inputs
      I0   = 0, I1   = 1, I2   = 2, I3   = 3, I4   = 4, I5   = 5, I6   = 6, I7   = 7,
      DI0  = 0, DI1  = 1, DI2  = 2, DI3  = 3, DI4  = 4, DI5  = 5, DI6  = 6, DI7  = 7,
      DIN0 = 0, DIN1 = 1, DIN2 = 2, DIN3 = 3, DIN4 = 4, DIN5 = 5, DIN6 = 6, DIN7 = 7,
      // Born digital outputs
      Q0    = 8, Q1    = 9, Q2    = 10, Q3    = 11, Q4    = 12, Q5    = 13, Q6    = 14, Q7    = 15,
      DO0   = 8, DO1   = 9, DO2   = 10, DO3   = 11, DO4   = 12, DO5   = 13, DO6   = 14, DO7   = 15,
      DOUT0 = 8, DOUT1 = 9, DOUT2 = 10, DOUT3 = 11, DOUT4 = 12, DOUT5 = 13, DOUT6 = 14, DOUT7 = 15,
      // P12 header
      GPIO0 = 16, GPIO1 = 17, GPIO2 = 18, GPIO3 = 19, GPIO7 = 20, GPIO8 = 21, 
      // P14 header
      SPI_MISO = 22, SPI_MOSI = 23, SPI_CS = 23,
      //#error CIAA-NXP
   #elif (BOARD == edu_ciaa_nxp)
      VCC = -2, GND = -1,
      // P1 header
      T_FIL1,    T_COL2,    T_COL0,    T_FIL2,      T_FIL3,  T_FIL0,     T_COL1,
      CAN_TD,    CAN_RD,    RS232_TXD, RS232_RXD,
      // P2 header
      GPIO8,     GPIO7,     GPIO5,     GPIO3,       GPIO1,
      LCD1,      LCD2,      LCD3,      LCDRS,       LCD4,
      SPI_MISO,
      ENET_TXD1, ENET_TXD0, ENET_MDIO, ENET_CRS_DV, ENET_MDC, ENET_TXEN, ENET_RXD1,
      GPIO6,     GPIO4,     GPIO2,     GPIO0,
      LCDEN,
      SPI_MOSI,
      ENET_RXD0,
	  #ifndef GRUPO
      // Switches
      // 36   37     38     39
	    TEC1,  TEC2,  TEC3,  TEC4,
      // Leds
      // 40   41     42     43     44     45
        LEDR,  LEDG,  LEDB,  LED1,  LED2,  LED3,
	  #elif (GRUPO == 2)
      // Switches
      // 36   37     38     39
	TEC3,  TEC4, TEC1,  TEC2,
      // Leds
      // 40   41     42     43     44     45
	 LEDR,  LEDG,  LED1, LED2,  LED3,  LEDB,
	  #endif
	  //#error EDU-CIAA-NXP
   #else
      #error BOARD not supported yet!
   #endif
} gpioMap_t;
```

De esta forma si no se define la constante `GRUPO`, no se cambia el comportamiento de los periféricos definidos por sAPI.

La implementación de este ejercicio está basada en el ejemplo `switches_led.c` que se encuentra en: `firmware_v3/examples/c/sapi/gpio/switches_leds/src/switches_leds.c/`. De este archivo se pueden obtener las funciones de librería sAPI utiles para el sensado de un pulsador y para el accionado de un led. 

La función `gpioRead(gpioMap_t pin)` se encuentra en el archivo `sapi_gpio.c` en el directorio `firmware_v3/libs/sapi/sapi_v0.6.2/soc/peripherals/src/sapi_gpio.c` y se utiliza para leer un pin del GPIO que ha sido seteado como INPUT mediante la función `gpioConfig(gpioMap_t pin, gpioIinit_t config)`. Leyendo el valor de un pin que está asociado a pulsador (que se obtiene como el retorno de la función `gpioRead`), se puede accionar un led según si está presionado o no. 

Con la función `gpioWrite(gpioMap_t pin, bool_t value)`, que se encuentra en el mismo archivo `sapi_gpio.c`, se puede cambiar el valor de un pin que ha sido seteado como OTPUT mediante la función `gpioConfig`. Si un pin está conectado a un led, se puede prender o apagar cambiando el valor de `value` (1 o 0 respectivamente). A su vez se puede utilizar el valor de la lectura del pulsador para controlar el led, como se indica a continación:

```c
while(1) {

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

   }
```
Al haber definido la constante `GRUPO` los leds y los pulsadores en la placa no van a coincidir con los nombres que se utilizan en el código, con lo cual, por ejemplo, al presionar la tecla TEC1, que en la placa es el pulsador numero 3, se encenderá el LEDB, que no es el led azul del RGB, sino, el led número 4.


# Ejercicio 3/4: ticks_tickHook y constantes para manipular tiempos

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

Donde `myTickHook` es la función a ser ejecutada y `(void*)LEDR` son los argumentos con los cuales se ejecuta la función. La función `tickCallbackSet(callBackFuncPtr_t tickCallback, void* tickCallbackParams)` se encuentra en `firmware_v3/libs/sapi/sapi_v0.6.2/soc/peripherals/src/sapi_tick.c`.

En este caso la función que se definió para ser ejecutada cuando se dispara la interrupción es un toggle de algun LED, como se especifica a continuación:


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
En esta función se definió una variable `static bool_t ledState` que es la encargada de guardar el valor del pin, al ser `static` su valor no se pierde al salir de la función y se puede reutilizar en cada llamado. 

Luego, en el programa principal se tienen las siguientes sentencias:

```c
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
```

Donde la constante `LED_TOGGLE_MS` indica el tiempo en el cual se vuelve a definir una función cada vez que ocurre una interrupción. En este caso se utiliza la misma función pero se le cambian los argumentos, variando el LED que se hace parpadear cada `LED_TOGGLE_MS` milisegundos.

# Ejercicio 5: UART

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
Donde se define la vía por la cual se establecerá la comunicación. En este caso, se elige la opción `` UART_USB  = 3``, es decir que la comunicación será a través del puerto `USB DEBUG.` También recibe como parámetro el baud rate, que marcará la tasa con que se transmita la información y está tipificado como un entero de 32 bits.

Luego esta función llama a su vez a otra función denominada `` uartConfig(uart, baudRate) ``, donde se especifican los números de puerto y de pin para transmisión y recepción para cada UART. 

La función `printString` recibe entre sus parámetros un puntero al inicio de una cadena de caracteres y llama a la función `` uartWriteString( printer, string )``. Se ve que la función recibe la cadena de caracteres y la vía de comunicación e invoca a la función `` uartWriteByte`` hasta que se termine de escribir la cadena entera. La función `` uartWriteByte`` está definida en el mismo lugar y lo que hace es recibir de a un byte por vez y escribirlo.

se agrega una sentencia condicional para inicializar las variables a utilizar, como se muestra a continuación:

```c
#if EJ == EJ_UART
DEBUG_PRINT_ENABLE
#endif
```
De esta forma se inicializan las variables que manipulan los mensajes por la UART, en este caso es una variable del tipo `print_t` llamada `debugPrint`.

En el codigo principal del programa comienza configurando el baudrate de la UART y cual usar:

```c
debugPrintConfigUart(UART_USB, 115200);
```

Se decide utilizar la UART por USB, con un baudrate de 115200. Luego se imprime una string inicial informando la correcta configuración del puerto serie.

```c
debugPrintlnString("DEBUG: UART_USB configurada.");
```

Luego se leen los pulsadores y se prenden o apagan los respectivos LEDs de distintas formas:

```c
// Prender LEDR si presiono TEC1
// Leer valor del pulsador 1 e invertir
tec1Value = !gpioRead(TEC1);
// Escribir el valor leido en el LED correspondiente.
gpioWrite(LEDR, tec1Value);

// Prender LED1 si presiono TEC2 en un solo movimiento
gpioWrite(LED1, !gpioRead(TEC2));

// Prender LED2 si se presiona TEC3
gpioWrite(LED2, !gpioRead(TEC3));

// Prender LED3 si se presiona TEC4
gpioWrite(LED3, !gpioRead(TEC4));

// Intercambio el valor del LEDB
gpioToggle(LEDB);
```

Se imprime por puerto serie el estado del LED azul (LEDB):

```c
// Chequear valor
if(ledbValue == ON) {
  // Si esta encendido mostrar por UART_USB "LEDB encendido."
  debugPrintlnString( "DEBUG: LEDB encendido." );
} else {
  // Si esta apagado mostrar por UART_USB "LEDB apagado."
  debugPrintlnString( "DEBUG: LEDB apagado." );
}
```

# Ejercicio 6: modificación del código para sensado de pulsadores

En este ejercicio se desea sensar pulsadores para ir encendiendo o apagando LEDs en secuencia. Se imprimirá por puerto serie el led que se encienda o se apague. Para poder apagar o prender los LEDs de forma secuencial se escribe el siguiente código:

```c
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
```

Donde cada vez que se prenda o apague un led, se pasa a seleccionar el siguiente sumando el iterador `i` hasta que se llegue al ultimo LEDx.
