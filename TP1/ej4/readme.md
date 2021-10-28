# Ejercicio 4

> Implementar (editar, simular y generar el código) el modelo de control de Puerta Corrediza automatizada (motor con movimiento en dos sentidos, sensor de presencia y fines de carrera)

En este ejercicio se implemento el control de la puerta corrediza automatizada. A continuación se muestra una figura con el diagrama de estados del modelo

![Image](Puerta Corrediza.png)

El diagrama cuenta con dos regiones: TECX y main region. En la región de main region se realiza el accionamiento de la puerta corrediza propiamente dicho, dicha región cuenta con 4 estados: CERRADO, CERRANDO, ABRIENDO y ABIERTO. En los estados CERRANDO y ABRIENDO se produce el cierre y apertura de la puerta corrediza.
