# buscador-aed

Practica de Algoritmos y Estructuras de Datos (Universidad de Murcia,
curso 2003/2004, 2 de ITI de Gestion): un buscador de paginas web con
indice invertido posicional, tabla hash propia y ranking por relevancia.

Restaurada y modernizada en 2026: correccion de bugs y fugas de memoria,
parser HTML, algoritmos de carga y consulta optimizados, tests y
benchmark. El diseño y el detalle de las correcciones estan en
[buscador/DISENO.md](buscador/DISENO.md).

## Uso

```
cd buscador
make            # compila el buscador
make test       # autocomprobacion con asserts
make bench      # compila el benchmark de estructuras

./Buscador      # REPL: LOAD maestro.txt, SEARCH OR|AND|NOT|COM palabras..., QUIT
```

El directorio `generador/` contiene el generador de conjuntos de prueba
proporcionado por los profesores (intacto): crea N paginas sinteticas y el
fichero maestro que consume `LOAD`.

Rendimiento del buscador (2000 paginas, 1,2M palabras): carga ~0,5 s,
consultas 0-9 ms.

## Benchmark: la tabla hash contra ocho alternativas

`buscador/bench_estructuras.cpp` compara la `close_hash` de la practica
con ocho estructuras de firma identica (`insert`/`search`/`erase` sobre
`string → int`), todas artesanales: 1M de claves, 3M de busquedas por
fase, verificacion por checksums (las nueve devuelven exactamente los
mismos resultados). Las implementaciones estan en
[buscador/estructuras/](buscador/estructuras/), una por fichero.

```
./bench_estructuras ch|cl|oa|tr|rx|av|btn|bp|bs
```

Minimos de 3 rondas alternadas en la misma tanda. Todas las estructuras
arrancan vacias y crecen dinamicamente — los hashes desde 1.024 celdas,
con 10 o mas duplicaciones por el camino:

| Estructura | Memoria | insercion | acierto | fallo | recorrido en orden | borrado |
|---|---:|---:|---:|---:|---:|---:|
| **close_hash (la practica — referencia)** | 146 MB | 699 ms | 1.047 ms | 783 ms | — | 321 ms |
| Flat hash (sondeo lineal) | 127 MB | 394 ms | 664 ms | 173 ms | — | 248 ms |
| Hash clasico (encadenado) | 99 MB | 529 ms | 696 ms | 659 ms | — | 244 ms |
| Radix trie (Patricia) | 67 MB | 602 ms | 2.280 ms | 10 ms* | — | 682 ms |
| Trie plano | 240 MB | 1.572 ms | 3.693 ms | 9 ms* | — | 641 ms |
| AVL | 106 MB | 880 ms | 1.417 ms | 241 ms | 34 ms | 800 ms |
| Arbol B (G=32) | 104 MB | 1.152 ms | 4.272 ms | 386 ms | 6 ms | 1.460 ms |
| Arbol B+ | 109 MB | 1.377 ms | 4.528 ms | 402 ms | 6 ms | 1.816 ms |
| Arbol B* | 97 MB | 1.535 ms | 3.855 ms | 371 ms | 6 ms | 1.525 ms |

El borrado es completo en todas (rebalanceo en el AVL, prestamo y fusion
de nodos en los arboles B, poda de ramas en los tries) salvo en el flat
hash, que marca la celda como vacia y la reutiliza o purga al crecer:
lo canonico en direccionamiento abierto. `*` fallo de trie/radix
dependiente del corpus (las claves ausentes divergen pronto).

Prealocando los hashes al tamaño final (`./bench_estructuras oa` sin
capacidad inicial) ahorran insercion — close_hash 482 ms, clasico
405 ms, flat sin cambio apreciable — y sobre todo memoria: flat 61 MB,
clasico 91 MB, close_hash 122 MB. La diferencia de memoria no es
estructura viva sino retencion del allocator tras los crecimientos.

### Conclusiones

- **Para busqueda exacta, hash** — la eleccion de la practica era la
  correcta. El resto de estructuras tarda entre 2 y 4 veces mas por
  busqueda a cambio de operaciones que el buscador no necesita.
- **El flat hash gana la categoria**: arrays planos sin punteros, la
  mejor memoria de la tabla, y fallos 3-4 veces mas rapidos que los
  encadenados (una celda vacia responde sin comparar strings). Es la
  razon de que los hashes planos (Swiss tables) dominen hoy.
- **La close_hash queda a un 30-50% del clasico creciendo, y a un
  10-20% prealocada** tras las correcciones. Su resize es el mas caro de
  los tres: re-inserta cada clave recalculando el hash.
- **La compresion de caminos transforma el trie**: de 240 MB a 67 MB y casi
  el doble de velocidad, conservando el fallo instantaneo y la busqueda
  por prefijo, que ningun hash puede ofrecer.
- **Los arboles B ganan en recorrido ordenado** (5-6 ms el recorrido
  completo, 5 veces mas rapido que el AVL) y el AVL en busqueda puntual
  ordenada; el B+ es lo que usan las bases de datos porque su ventaja
  real (hojas enlazadas = rangos secuenciales, internos sin valores =
  arbol bajo) se materializa con paginas de disco, no en RAM. El B*
  cumple su promesa historica: 11% menos nodos a cambio de inserciones
  mas caras.

## Estructura del repositorio

```
buscador/               codigo del buscador + DISENO.md + tests + benchmark
buscador/estructuras/   las estructuras del benchmark, una por fichero
generador/              generador de conjuntos de prueba (codigo de los profesores)
```
