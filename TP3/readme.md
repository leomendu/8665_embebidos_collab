# Ejercicio 2

A partir de los programas de ejemplo switches_leds, fsm_debounce_non_blocking, tickHook, y uart , programar una aplicación que realice lo
siguiente:

<p align="center">
  PULSADOR -> LED -> UART (USB).TX -> TERMINAL (PC) -> TEXTO(PANTALLA)
</p>

Es decir, cuando se presione (o se suelte) uno de los pulsadores se deberá prender (o apagar) un led de la placa y enviar un carácter a
través de la terminal serie implementada con el puerto USB de debug. Dado que hay 4 pulsadores en la placa, asignar un led y un caracter
distinto a cada pulsador y mantener la consistencia entre ellos.



# Ejercicio 3

Una vez hecho el punto anterior, se desea hacer una aplicación similar pero al revés:

<p align="center">
  TECLA (PC) -> TERMINAL (PC) -> UART (USB).RX -> LED
</p>

a. Por medio de la terminal serie implementada con el puerto USB de debug, y basándose en el ejemplo uart.c, asignar 3
caracteres para prender cada uno de los 3 led y 3 caracteres para apagar cada uno de los 3 led.

b. Implementar el ejercicio a. de nuevo, pero por interrupciones. Basarse en el ejemplo rx_interrupt.

c. Combinar las funcionalidades de los ejercicios a. y b. en un solo programa:

<p align="center">
  TEXTO(PANTALLA) <- TERMINAL (PC) <- UART (USB).TX <- PULSADOR
</p>

<p align="center">
  TECLA (PC) -> TERMINAL (PC) -> UART (USB).RX -> LED
</p>



# Ejercicio 4

A partir del programa de ejemplo rgb_led_uart:

a. Armar un PWM por software para disponer de distintos niveles de brillo en cada uno de los leds.

b. Por medio de la terminal serie implementada con el puerto USB de debug, asignar un caracter para incrementar y otro para
decrementar el brillo de 1 led.



# Ejercicio 5

A partir del programa de ejemplo adc_dac , vamos a probar el funcionamiento del conversor A/D. Con un resistor de valor igual o superior a
1k, y según lo que muestra la imagen adc_dac_pins.png del ejemplo:

a. Conectar a través del resistor la entrada de A/D a 3.3v y verificar por la terminal de debug que indique el fondo de escala.

b. Conectar a través del resistor la entrada de A/D a 0v y verificar por la terminal de debug que indique el valor nulo.

c. Conectar 2 resistores de igual valor de la siguiente manera:

<p align="center">
  3,3v -/\/\/\-- A/D -/\/\/\-- 0v
</p>

y verificar por la terminal de debug que indique un valor cercano al centro de escala.

<p align="center">
  (0v .. 3,3V) -> RESISTOR -> A/D -> UART (USB).TX -> TERMINAL (PC) -> TEXTO (PC)
</p>


# Ejercicio 6

A partir del programa de ejemplo adc_dac , vamos a probar el funcionamiento del conversor D/A y vamos a verificarlo con el ejercicio
anterior:

a. Modificar el ejemplo para que con las teclas numéricas (de 0 a 9) se pueda variar la tension de salida del DAC el valor de la
salida DAC (donde el “0” corresponde con 0v y el “9” con 3,3v).

<p align="center">
TECLA (0 - 9) -> TERMINAL (PC) -> UART (USB).RX -> DAC -> (0 - 3,3v)
</p>

b. Agregar al punto anterior la funcionalidad del ejercicio 5 para poder visualizar por la consola la variacion de tension que le
aplicamos al A/D. Para ello se debe armar un loopback a través de puerto serie:

<p align="center">
bornera 15 [DAC] --/\/\/\-- bornera 11 [ADC (CH1)]
</p>

con un resistor de valor igual o mayor a 1k.

<p align="center">
TECLA (0..9) -> TERMINAL (PC) -> UART (USB).RX -> DAC -> (0..3,3v) -> RESISTOR -> ( 0v .. 3,3V) -> RESISTOR -> A/D ->
UART (USB).TX -> TERMINAL (PC) -> TEXTO (PANTALLA)
</p>



# Ejercicio 7

En el conector CONN_20x2 de la edu-ciaa, armar un loopback a través de puerto serie Uart3:

<p align="center">
  Pin 88 {P2_4} bornera 23 [RX] --/\/\/\-- [TX] 25 bornera {P2₃} Pin 87
</p>

a través de un resistor de valor igual o superior a 1k.

a. Modificar el programa del ejercicio 2 para redireccionar el envió del carácter de la UART2 a la UART3.

b. Modificar el programa del ejercicio 3 para redireccionar la recepción del carácter de la UART2 a la UART3.

c. Testear que cuando esté conectado el resistor de loopback, se puedan prender o apagar 2 de los leds con los 4 pulsadores.

<p align="center">
  PULSADOR -> LED -> UART3.TX -> RESISTOR -> UART3.RX -> LED
</p>



# Ejercicio 8

En el conector CONN_20x2 de la edu-ciaa armar un loopback a través de puerto serie Uart3:

<p align="center">
  Pin 88 {P2_4} bornera 23 [RX] --/\/\/\-- [TX] 25 bornera {P2₃} Pin 87
</p>

a través de un resistor de valor igual o superior a 1k.

a. Modificar el programa 3 para forwardear el carácter que ingresa por RX de la uart USB de debug al RX de la UART3.

b. Agregar la funcionalidad de forwardear el carácter que ingresa por el pin 88 RX de la UART3 al TX de la uart USB de
debug.

c. Testear que cuando esté conectado el resistor de loopback se reciba por la consola un eco de lo escrito por la misma.

<p align="center">
  TECLA -> TERMINAL (PC) -> UART (USB).RX -> LED -> UART3.TX -> RESISTOR -> UART3.RX -> LED -> UART (USB).TX ->
  TERMINAL (PC) -> TEXTO(PANTALLA)
</p>
