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
