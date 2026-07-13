# Buscador AED (2003/2004) — ideas de implementacion

Practica de AED, 2 de ITI de Gestion, UM. Buscador booleano sobre paginas
generadas, con indice invertido posicional y ranking por relevancia.
Revisado y corregido en 2026 (leaks, UB, rendimiento, parser HTML).

## 1. El problema

El generador (codigo de los profesores) produce N paginas sinteticas y un
fichero maestro con una linea por pagina: `URL  fichero  relevancia  titulo`.
El buscador carga el maestro (`LOAD`), indexa el contenido de cada pagina y
responde consultas `SEARCH AND|OR|NOT|COM palabras...` ordenadas por
relevancia, ademas de `INSERT`, `REMOVE`, `SAVE`, `HELP`, `CLEAR`, `QUIT`.

## 2. Arquitectura de modulos

| Modulo | Responsabilidad |
|---|---|
| `principal.cpp` | main y bucle REPL (stdin/stdout o ficheros por argv) |
| `buffer.cpp` | tokenizacion de la linea de comandos |
| `runcom.cpp` | despacho de comandos |
| `estructura_datos.*` | el motor: Contenedor, indice, operadores, lector |
| `close_hash.*` | tabla hash generica reutilizable |
| `funciones_auxiliares.*` | normalizacion de texto y cronometro |
| `screen.*` | escapes ANSI |

Diseño de compilacion: una sola unidad de traduccion — los `.cpp` se
incluyen unos a otros (con include guards) y el makefile compila solo
`principal.cpp`. Decision de 2004; el Makefile actual declara todas las
fuentes como dependencias para que `make` recompile cuando toca.

## 3. Estructuras de datos

### close_hash<T,key>

Tabla hash de capacidad fija con encadenamiento sobre un pool compartido:
cuatro arrays paralelos (`indices` por cubeta, `siguientes` como enlaces,
`claves`, `elementos`) y una free-list (`gestor_vacios`) que entrega huecos.
Generica por plantilla y con la funcion hash inyectada por puntero a
funcion — inversion de dependencias antes de conocer el nombre. Las cadenas
se mantienen ordenadas por clave, lo que permite cortar la busqueda al
pasarse. Sin rehash: si se llena, el insert falla (techo asumido).

Dos instancias:
- `tabla_palabras`: palabra → `Lista_datos` (el indice invertido).
- `tabla_pag`: `Pagina*` → lista de sus palabras distintas (solo la usa
  REMOVE para des-indexar una pagina).

### Indice invertido posicional

- `Pagina`: URL, fichero, relevancia, `aparicion` (id unico de carga).
- `Nodo`: puntero a `Pagina` + `l_aparic`, la lista de posiciones (1a
  palabra, 2a, ...) donde la palabra aparece en esa pagina.
- `Lista_datos`: lista de nodos (una entrada por pagina que contiene la
  palabra) + flags `error` y `permitir_com`.

Las posiciones por palabra son lo que convierte un indice booleano en uno
posicional: habilitan la busqueda de frase exacta.

### Ranking precalculado (la idea mas fina)

`Pagina::operator<` ordena por relevancia descendente con desempate por id
(orden total). Tras la carga, cada posting list se ordena UNA vez
(`Sizeof_and_sort` aprovecha el recorrido de medicion). Como los merges de
consulta preservan el orden, los resultados salen rankeados gratis: cero
ordenaciones en tiempo de consulta. Es la misma idea que los postings
ordenados por rank estatico de los primeros Google.

## 4. Algoritmos

### Carga: O(palabras del corpus)

1. `limpiar_html()` (añadido 2026): pasada previa que elimina etiquetas
   (`<script>` y `<style>` con su contenido) y decodifica entidades basicas
   a latin-1 (`&oacute;` → o con tilde, `&#NNN;` numericas).
2. `lee()`: automata de dos estados que extrae secuencias de letras,
   plegando al vuelo mayusculas y tildes (`trasforma_letra`, tabla latin-1).
3. `inserta_palabra`: lookup hash O(1). Si el nodo frontal de la lista ya es
   la pagina actual, solo apunta la posicion — funciona porque las
   apariciones de una pagina llegan consecutivas y el nodo nuevo entra por
   delante (`push_front`). Palabra nueva en la pagina → nodo nuevo + alta en
   `tabla_pag`.

### Consulta: merges de listas ordenadas, O(|p|+|q|)

- `AND`/`OR`/`XOR`/`OPN`: dos iteradores avanzando por listas ordenadas;
  igualdad por id de pagina, avance por orden de relevancia. El algoritmo
  canonico de interseccion de posting lists.
- `COM` (frase exacta): merge por pagina y, en cada pagina comun,
  `f_aux_com` mergea las dos listas de posiciones buscando adyacencia
  (`pos2 - pos1 == 1`). El resultado conserva las posiciones de la segunda
  palabra, de modo que encadenar COM da frases de N palabras por induccion.
  Direccional: `COM que se` ≠ `COM se que`.
- `permitir_com`: invariante en runtime — solo listas con posiciones
  coherentes (indice puro o resultado de COM) pueden entrar a un COM; los
  resultados de AND/OR/NOT no.
- Cortes tempranos: OR se detiene al cubrir todas las paginas; AND y COM al
  quedarse vacios. NOT = XOR con la lista total, y OPN para los siguientes
  terminos.
- Desde 2026 los operadores trabajan con punteros al indice (`Search`
  devuelve la lista sin copiarla) y un acumulador unico: cero copias de
  posting lists por consulta.

### REPL

`Lee_buffer` + `limpiar_linea` + `crear_lista`: separa caracteres especiales,
troceo por espacios, y reescribe comillas a `( COM ... )` — azucar sintactico
que en 2004 quedo a medio cablear (el parser de SEARCH no consume el `(`;
la via operativa es `SEARCH COM`). `runcom` despacha con un switch por
primera letra para no comparar contra todas las palabras reservadas.

## 5. Instrumentacion

- `cronometro` sobre `gettimeofday` (resolucion de milisegundos), usado en
  carga y en cada consulta.
- `Sizeof_and_sort`: recorre las estructuras midiendo su tamaño real en
  bytes (y de paso ordena las listas). Se muestra tras cada LOAD/INSERT.
  La practica obligaba a saber lo que cuesta cada estructura.

## 6. Complejidades

| Operacion | Coste |
|---|---|
| Indexar una palabra | O(1) esperado |
| Carga completa | O(W) palabras + O(D log D) ordenacion de listas |
| AND / OR / NOT de k terminos | O(k · L) con L = longitud de posting list |
| COM de k palabras | O(k · (L + P)) con P = posiciones |
| Resultado rankeado | gratis (orden precalculado) |
| REMOVE | O(paginas) localizar + O(palabras de la pagina) |

Medido (2000 paginas, 1,2M palabras, 2026): carga ~0,5 s; consultas 0-9 ms.

## 7. Correcciones de 2026

- Bug de 2004 en `close_hash::inserta` (caso 3): con cadena de un solo
  nodo y clave nueva mayor, la anteponia rompiendo el orden y la busqueda
  (que corta por orden) perdia el nodo antiguo. Con colisiones a escala
  perdia el 12% de las claves; en el buscador real perdia unas pocas
  palabras en silencio desde 2004.
- Fugas: `minusculas`/`mayusculas`/`limpiar_linea` devuelven `string`;
  `Search` sin `new`; las `Pagina` se liberan en `Contenedor::Clear`;
  `delete[]` en los arrays de `close_hash`; `resize` liberaba los arrays
  viejos y restaura `num_elementos`; `insert` con clave duplicada libera
  las copias. ASan/LSan limpio.
- UB: `return` que faltaban en `erase`/`asign` (con `-O2` producian
  instruccion ilegal); `isalpha`/`toupper` con cast a `unsigned char`.
- Errores: los 23 `exit(1)` eliminados — hash negativo normalizado por
  construccion, invariantes internos con `assert`, contratos con `assert`
  mas valor de error; la biblioteca ya no mata el proceso del cliente.
- Rendimiento: `inserta_pagina` de O(w2) a O(w); consultas sin copias
  (`Search` devuelve puntero al indice); firmas `const key&` en la tabla
  y en las funciones hash (~2x carga, 3-8x consulta en total).
- Robustez: EOF sin QUIT ya no deja el proceso girando al 100% de CPU;
  fuentes convertidas a UTF-8 (los datos siguen en latin-1, con escapes
  `\x` documentados); Makefile con dependencias reales, `make test` y
  `make bench`; parser HTML (etiquetas y entidades) — el corpus original
  arrastraba tokens `<font>`/`<html>` que se indexaban como palabras
  fantasma.

## 8. Benchmark de estructuras

`bench_estructuras.cpp` (`make bench`) compara la `close_hash` con nueve
estructuras alternativas de firma identica (`insert`/`search`/`erase`
sobre `string → int`): hash encadenado clasico, direccionamiento abierto,
Bloom+hash, trie, radix trie, AVL, y arboles B, B+ y B*. Resultados y
conclusiones en el README del repo.
