# Ejercicio 6

A partir del ejemplo `adc_dac` en `firmware_v3/examples/c/sapi/adc_dac/` se configuró el conversor DA de la placa, utilizando las siguientes funciones dentro del main del programa: 

```{c}   
adcConfig( DAC_ENABLE ); /* DAC */
```

El valor del pin DAC se puede setear mediante la función `dacWrite(DAC, val)` la cual se encuentra definida en `firmware_v3/libs/sapi/sapi_v0.6.2/soc/peripherals/src/sapi_dac.c`. 

El valor del DAC que se quiere setear se informa mediante consola, a través de la UART USB, utilizando interrrupciones como fue detallado en el ejercicio 3. La función a ejecutar al saltar la interrupción es la siguiente:

```{c}
void DAC_SERIAL_LEVEL(void){
	serial_val = uartRxRead( UART_USB );
   uartWriteString( UART_USB, "ADC CH1 value to be set: " );
   uartWriteByte(UART_USB, serial_val);
   uartWriteString( UART_USB, "\n" );
   if(serial_val == '0'){val =0;}      //0V
   if(serial_val == '1'){val =113;} //0.36V
   if(serial_val == '2'){val =226;} //0.72V
   if(serial_val == '3'){val =339;} //1.09V
   if(serial_val == '4'){val =452;} //1.46V
   if(serial_val == '5'){val =565;} //1.81V
   if(serial_val == '6'){val =678;} //2.18V
   if(serial_val == '7'){val =791;} //2.54V
   if(serial_val == '8'){val =904;} //2.90V
   if(serial_val == '9'){val =1023;}   //3.28V
   }
```
En la terminal se espera recibir un valor entre 0 y 9, los cuales se corresponden con 0 V y 3.3 V respectivamente, o 0 y 1023 en términos digitales. Haciendo esta conversión, tal como se detalla en la función `DAC_SERIAL_LEVEL()` se utiliza la variable global `val` para setear el valor del pin DAC mediante la función `dacWrite`.

Para poder verificar el funcionamiento del conversor DA, se conectaron los pines DAC y CH1 con un resistor de 2k2, ya que mediante CH1 se puede leer el valor seteado en el pin DAC, reutilizando el programa implementado en el ejercicio 5. 
