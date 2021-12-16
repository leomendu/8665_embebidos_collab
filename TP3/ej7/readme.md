# Ejercicio 7

En este ejercicio se utilizó la UART 232 para comunicar que se produjo el accionado de un LED. De la misma forma que en el ejercicio 3, al oprimir (y soltar) los pulsadores se prenden (y apagan) los leds de la placa, pero además, en este ejercicio se incorporó la utilización de la UART_232, y al oprimir y soltar los pulsadores producen el encendido y apagado de los LEDS contiguos a los accionados por el pulsador. 

Para utilizar la UART_232 se utilizó la función `uartConfig()` de la siguiente manera:

```{c}
int main(void){
...

uartConfig(UART_232, 115200);

...
}
```

En el loop infinito del programa se corre la función `void FUNCION_UART232(void)`. Esta función se basa en un `switch` sobre la variable `ESTADO`, la cual toma los distintos valores del tipo enumerativo:
```{c}
enum{
  ESPERANDO,
  TRANSMITIDO,
  PULSADO_UNO,
  PULSADO_DOS,
  PULSADO_TRES,
  PULSADO_CUATRO,
  };
```

En el <i> case </i> `ESPERANDO` se reutilizó el codigo del ejercicio 2 para implementar el control de los leds mediante los pulsadores. A su vez, se escribe por la UART un caracter que puede ser '1', '2', '3', o '4', mediante la función: `uartWriteByte(UART_232, caracter)`, donde caracter puede ser cualquiera de los mencionados (cada uno asociado a un pulsador distinto):

```{c}
if(!gpioRead(TEC1) == 1){    //Tecla 1
       uartWriteByte( UART_232, '1' );
         ESTADO=TRASMITIDO;
       return;}

 if(!gpioRead(TEC2) == 1){    //Tecla 2
       uartWriteByte( UART_232, '2' );
         ESTADO=TRASMITIDO;
       return;}

 if(!gpioRead(TEC3) == 1){    //Tecla 3
       uartWriteByte( UART_232, '3');
         ESTADO=TRASMITIDO;
       return;}

 if(!gpioRead(TEC4) == 1){ //Tecla 4
       uartWriteByte( UART_232, '4');
         ESTADO=TRASMITIDO;
       return;}
```

En el <i> case </i> `TRANSMITIDO` se realiza la lectura de la UART y se setea un nuevo valor a la variable `ESTADO` mediante la función `SELECTOR_DATO()`

```{c}
case(TRASMITIDO):
      uartReadByte( UART_232, &dato);
      SELECTOR_DATO(dato);  //Identificador de dato enviado por tx
      break;
```

En la función `SELECTOR_DATO` se setea la variable `ESTADO` según cuál fue el dato transmitido por la UART: 

```{c}
void SELECTOR_DATO(uint8_t  dato){ //Selector del dato enviado
   if(dato=='1'){ESTADO =PULSADO_UNO;return;}
   if(dato=='2'){ESTADO =PULSADO_DOS;return;}
   if(dato=='3'){ESTADO =PULSADO_TRES;return;}
   if(dato=='4'){ESTADO =PULSADO_CUATRO;return;}
   ESTADO=ESPERANDO;
}
```

En los <i> cases </i> `PULSADO_UNO`, `PULSADO_DOS`, `PULSADO_TRES` y `PULSADO_CUATRO` se pone en alto el led contiguo al asociado al pulsador que se haya seleccionado. Por ejemplo: si se pulsó la tecla `TEC 1`, se enciende el `LEDB`, pero además, mediante la UART_232 se enciende el LED contiugo al `LEDB`,  es decir, `LED1`, como se muestra a continuación:

```{c}
 case(PULSADO_UNO):
        gpioWrite( LED1, HIGH );//Lectura x tecla 1
        delay(50);
        ESTADO=ESPERANDO;
       break;
```

Para las conexiones de la placa se utilizó un resistor de 2k2 entre el pin RX y TX de la UART_232. De no estar conectados estos pines, no se produce el encedido de los LEDS contiguos a los controlados por los pulsadores. 





