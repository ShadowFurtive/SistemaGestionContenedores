# PRÁCTICA FINAL - ESIN
## Creación de un sistema de gestión de contenedores
## Esta versión está desactualizada.
##### Faltan PRE/POST y acabar de comentar el código. La versión entregada, la final, lo tiene todo acabado.

[![N|Solid](https://www.upc.edu/comunicacio/ca/identitat/descarrega-arxius-grafics/fitxers-marca-principal/upc-positiu-p3005.png)](https://www.epsevg.upc.edu/ca/escola)

![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)

Proyecto realizado en parejas para la primera actividad de la asignatura ESIN de la EPSEVG-UPC. Como el proyecto no tuvo que ser documentado, no hay demasiada documentación aparte de los comentarios en los códigos . 

- Autores: Mario Kochan y [Abdullah Bashir](https://github.com/aawan2805)
- Fecha: 08/01/2021
- Profesores: Jordi Esteve y Bernardino Casas

## Características

- Programación en [C++](https://es.wikipedia.org/wiki/C%2B%2B)
- Uso de clases
- Uso de punteros 
- Uso de estructuras de datos

### Proyecto
El proyecto se basaba en crear un sistema de gestión de contenedores por terminales, cada terminal contiene contenedores que estos contenedores estarán en catalogos. Los contenedores estarán sitios llamado ubicaciones. 

Los contenedores deben caber en una terminal de NxMxH, no puede haber una altura de contenedores más alta que H ni más grande o larga que N o M respectivamente.

Para cada contenedor, solo se podrá ubicar encima de otro contenedor siempre que no queden huecos libres por debajo. Además de eso, solo se podrá colocar un contenedor en un hueco que quepa. 

Los contenedores pueden tener diferentes tamaños, yendo de contenedores de 10 de largaria a 30. 

### Estructura de datos utilizada
Para este proyecto hemos utilizado punteros que forman una lista con las ubicaciones vacias y una matriz tridimensioal que nos sirve como mapa para saber como están ubicados los contenedores.
En el catalogo hemos hecho uso de una estructura de datos basada en hash. 

### Make y ejecución del programa

En la propia raiz del documento, podemos hacer uso de un make:
```sh
make
```
Nos compilará los archivos y podremos ejecutar el programa.
```sh
./program.exe
```
También viene incluido archivos .in para poder hacer diff en caso que queramos checkear si el programa saca buenos resultados.

### NOTA:
- Nota final: 6.8








