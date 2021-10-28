# Ejercicio 5

> Implementar (editar, simular y generar el código) el modelo de control de Portón Levadizo automatizado (motor con movimiento en dos sentidos, control remoto de apertura/cierre, fines de carrera y señalización luminosa)

En este ejercicio se implementó el control del Portón Levadizo automatizado, el cual se puede accionar mediante control remoto (tanto para abrir como para cerrar) y frenar al sensar obstáculos.

A continuación se muestra una figura con el diagrama de estados del modelo

![Image](ej5sct_porton.png)
![Image](ej5sct_luz.png)

El diagrama cuenta con dos regiones: Portón y Luz. En la región de Portón se realiza el accionamiento del portón propiamente dicho, dicha región cuenta con 6 estados: ```CERRADO```, ```CERRANDO```, ```STOP1```, ```STOP2```, ```ABRIENDO``` y ```ABIERTO```. En los estados ```CERRANDO``` y ```ABRIENDO``` se produce el cierre y apertura del portón. Una vez que el portón esta cerrado/abierto se transiciona al estado ```CERRADO```/```ABIERTO```. A los estados ```STOP1``` y ```STOP2``` se transiciona cuando ocurre el sensado de un obstáculo mientras el portón se estaba cerrando o abriendo, respectivamente. Luego se puede resumir la apertura o el cierre

En la segunda región, Luz, se realiza la señalización, la cual corre paralelamente a la región portón. La señalización propuesta es un titilo de leds cuando el portón se está cerrando o abriendo, o cuando hay un obstáculo presente en el portón. 

Los eventos utilizados para el modelo son:

```evRemotoA```
```evRemoroC```
```evCerro``` se produce cuando se termina el cierre del portón
```evAbrio``` se produce cuando se termina la apertura del portón
```evEmergencia``` se produce cuando hay un obstáculo interponiendose


Las señales que se utilizan para comunicar ambas regiones son:

```siTitilar``` se levanta cuando se empieza a cerrar/abrir el portón
```siNoTitilar``` se levanta cuando se terminó de cerrar/abrir el portón
```siEmergencia``` se levanta cuando se interpone un obstáculo en el portón

