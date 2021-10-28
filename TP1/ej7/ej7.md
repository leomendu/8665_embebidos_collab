En este ejercicio se diseñó el modelo de control de Horno Microondas con 3 modos seleccionables por botón de modo, implementando un botón para Comenzar/Terminar, y botón de sensor de apertura de puerta.

A continuación se muestra el diagarama de estados del modelo de control:

![Image](ej7sct.png)

Los eventos que se definieron en base a los botones son los siguientes:

```evComenzarTerminar``` Presión del botón para Comenzar/Terminar la cocción
```evModoC``` Presión del botón de cambio de modo de cocción
```evPuertaAbierta``` Sensado de puerta abierta
```evPuertaCerrada``` Sensado de puerta cerrada
```evTECXoff``` Evento para validar el presionado de los botones

