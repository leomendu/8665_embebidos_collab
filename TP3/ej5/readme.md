# Ejercicio 5

A partir del ejemplo `adc_dac` en `firmware_v3/examples/c/sapi/adc_dac/` se configuró el conversor AD de la placa, utilizando las siguientes funciones dentro del main del programa: 

```{c}   
adcConfig( ADC_ENABLE ); /* ADC */
```

En este programa se utilizó el <i> channel 1 </i>, cuyo valor se puede leer mediante la función `adcRead(CH1)` la cual se encuentra definida en `firmware_v3/libs/sapi/sapi_v0.6.2/soc/peripherals/src/sapi_adc.c`. 

Para conocer el valor del AD (el cual debe estar entre 0 y 1023 pues tiene 1024 valores) se utilizó la función `uartWriteString()` para escribir en la UART USB dicho valor, el cual se conviritió a una <i> string </i> mediante la función `itoa()`:

```{c}
muestra = adcRead( CH1 );

/* Envío la primer parte del mnesaje a la Uart */
uartWriteString( UART_USB, "ADC CH1 value: " );

/* Conversión de muestra entera a ascii con base decimal */
itoa( muestra, uartBuff, 10 ); /* 10 significa decimal */

/* Enviar muestra y Enter */
uartWriteString( UART_USB, uartBuff );
uartWriteString( UART_USB, ";\r\n" );
```

Se realizaron diferentes configuraciones de conexiones para verificar el correcto funcionamiento:

<tr> 
  <li> Conexión de un resistor de 2k2 entre 3.3 V (3V3) y el pin CH1. Al haber corriente nula la tensión en CH1 es 3.3 V lo cual se convierte en el valor 1023 al pasar por el ADC. </li>
  <li> Conexión de un resistor de 2k2 entre GND y el pin CH1. En este caso como la tensión en CH1 es 0 V, se convierte al valor 0 al pasar por el ADC. </li>
  <li> Conexión de un resistor de 2k2 entre 3v3 y CH1, y otro resistor del mismo valor entre CH1 y GND. En este caso la tensión en CH1 es de aproximadamente 3.3/2 V y el valor leído convertido por el ADC es de aproximadamente 1024/2. 
</tr>
