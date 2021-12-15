# Ejercicio 2

A partir de los programas de ejemplo switches_leds, fsm_debounce_non_blocking, tickHook, y uart , programar una aplicación que realice lo
siguiente:

<p align="center">
  PULSADOR -> LED -> UART (USB).TX -> TERMINAL (PC) -> TEXTO(PANTALLA)
</p>

Es decir, cuando se presione (o se suelte) uno de los pulsadores se deberá prender (o apagar) un led de la placa y enviar un carácter a
través de la terminal serie implementada con el puerto USB de debug. Dado que hay 4 pulsadores en la placa, asignar un led y un caracter
distinto a cada pulsador y mantener la consistencia entre ellos.

# Código

Inicializando la placa y la UART (USB) a una velocidad de 115200 baudios, se programa una función que sea capaz de interpretar la tecla presionada y prenda el led correspondiente y mande el mensaje correspondiente por puerto serie. La función principal tiene la siguiente forma:

```c
// inicializaciones
// ... Placa, UART, GPIOs ...
// fin inicializaciones

while(1) {
	  PULSADOR_TO_SERIAL();
   }

return 0 ;
```

donde la función `PULSADOR_TO_SERIAL` se encarga de traducir las teclas a LEDs y mensajes por puerto serie. Lo que realiza esta función es basicamente manipular estados (variable `ESTADO` de tipo enumerativo), y en base a esos estados ejecutar ciertas acciones.

```c
void PULSADOR_TO_SERIAL(void){

  switch(ESTADO){
    case(ESPERANDO):
      valor = !gpioRead( TEC1 );
      gpioWrite( LEDB, valor );

      valor = !gpioRead( TEC2 );
      gpioWrite( LED1, valor );
      
      // ...
      
      if(!gpioRead(TEC1) == 1) {
        ESTADO =PULSADO_UNO;
        return;
      }

      if(!gpioRead(TEC2) == 1) {
        ESTADO =PULSADO_DOS;
        return;
      }
      
      // ...
       
      case(PULSADO_UNO):
        uartWriteByte( UART_USB, 'H' );  // Envia 'H'
        delay(250);
        ESTADO = ESPERANDO;
        break;
        
      // ...
```

como se ve en la porción de código previo, se miden los estados de los botones y esto pone a la maquina en ese estado, prendiendo el LED correspondiente y enviando un mensaje por UART (USB).
