# Prueba Recuperativa de Computación Paralela: Dijkstra con OpenMP

Este repositorio contiene una implementación paralela (OpenMP) del algoritmo de Dijkstra en C++ para sistemas Ubuntu/Linux.

## Requisitos

- Ubuntu 18.04 o superior
- g++ (GCC) con soporte para OpenMP

## Instalación de dependencias

```sh
sudo apt update
sudo apt install g++ build-essential
```

## Compilación

```sh
g++ -fopenmp -O2 -std=c++11 dijkstra_omp.cpp -o dijkstra_omp
```

## Ejecución

El programa se ejecuta con:

```sh
./dijkstra_omp "[[matriz_ady]]" vertice_origen ruta_salida.txt
```

- `[[matriz_ady]]`: Matriz de adyacencia en formato tipo Python, por ejemplo: `[[0,10,0],[0,0,5],[0,0,0]]`
- `vertice_origen`: Índice del vértice de inicio (entero, base 0)
- `ruta_salida.txt`: Ruta y nombre del archivo donde se guardarán las distancias mínimas

**Ejemplo:**

```sh
./dijkstra_omp "[[0,10,0,0,5],[0,0,1,0,2],[0,0,0,4,0],[7,0,6,0,0],[0,3,9,2,0]]" 0 /tmp/resultados.txt
cat /tmp/resultados.txt
```

## Formato de salida

El archivo de salida tendrá una línea por vértice:

```
0	0
1	8
2	9
3	7
4	5
```

Donde la primera columna es el vértice y la segunda la distancia mínima desde el origen. Si un vértice es inalcanzable, la distancia será `-1`.

**Autor:** Javier Fernandez
