# buscador-aed

Practica de Algoritmos y Estructuras de Datos (Universidad de Murcia,
curso 2003/2004, 2 de ITI de Gestion): un buscador de paginas web con
indice invertido posicional, tabla hash propia y ranking por relevancia.

Restaurada y modernizada en 2026: fugas de memoria y UB eliminados
(ASan/LSan limpio), un bug de perdida de claves en la tabla hash que
llevaba oculto desde 2004, parser HTML, algoritmos de carga y consulta
optimizados, tests y benchmark. El diseño y las correcciones estan
detallados en [buscador/DISENO.md](buscador/DISENO.md).

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

## Benchmark: la tabla hash contra nueve alternativas

`buscador/bench_estructuras.cpp` compara la `close_hash` de la practica
con nueve estructuras de firma identica (`insert`/`search`/`erase` sobre
`string → int`), todas artesanales: 1M de claves, 3M de busquedas por
fase, verificacion por checksums (las diez devuelven exactamente los
mismos resultados). Las implementaciones estan en
[buscador/estructuras/](buscador/estructuras/), una por fichero.

```
./bench_estructuras ch|cl|oa|bl|tr|rx|av|btn|bp|bs
```

Minimos de 3 rondas alternadas en la misma tanda:

| Estructura | Memoria | insercion | acierto | fallo | recorrido en orden | borrado |
|---|---:|---:|---:|---:|---:|---:|
| **close_hash (la practica — referencia)** | 122 MB | 559 ms | 1.262 ms | 871 ms | — | 364 ms |
| Flat hash (sondeo lineal) | 61 MB | 375 ms | 672 ms | 176 ms | — | 235 ms |
| Hash clasico (encadenado) | 91 MB | 467 ms | 1.039 ms | 580 ms | — | 307 ms |
| Radix trie (Patricia) | 67 MB | 586 ms | 2.136 ms | 9 ms* | — | 614 ms |
| Trie plano | 240 MB | 1.790 ms | 3.578 ms | 9 ms* | — | 612 ms |
| AVL | 106 MB | 716 ms | 1.361 ms | 289 ms | 27 ms | 797 ms |
| Arbol B (G=32) | 104 MB | 1.194 ms | 3.173 ms | 366 ms | 6 ms | 1.320 ms |
| Arbol B+ | 109 MB | 1.291 ms | 3.837 ms | 365 ms | 5 ms | 1.628 ms |
| Arbol B* | 97 MB | 1.435 ms | 3.314 ms | 312 ms | 5 ms | 1.293 ms |

El borrado es completo en todas (rebalanceo en el AVL, prestamo y fusion
de nodos en los arboles B, poda de ramas en los tries) salvo en el flat
hash, que usa tumbas: lo canonico en direccionamiento abierto. `*` fallo
de trie/radix dependiente del corpus (las claves ausentes divergen
pronto).

### Conclusiones

- **Para busqueda exacta, hash** — la eleccion de la practica era la
  correcta. El resto de estructuras tarda entre 2 y 4 veces mas por
  busqueda a cambio de operaciones que el buscador no necesita.
- **El flat hash gana la categoria**: arrays planos sin punteros, la
  mejor memoria de la tabla, y fallos 3-4 veces mas rapidos que los
  encadenados (una celda vacia responde sin comparar strings). Es la
  razon de que los hashes planos (Swiss tables) dominen hoy.
- **La close_hash queda a un 10-20% del clasico** tras las correcciones
  (partia de un 50% de desventaja por firmas por valor, y de perder
  claves por el bug del caso 3). Es la unica de la tabla con factor de
  carga <= 1 garantizado por construccion.
- **La compresion de caminos transforma el trie**: de 240 MB a 67 MB y casi
  el doble de velocidad, conservando el fallo instantaneo y la busqueda
  por prefijo, que ningun hash puede ofrecer.
- **El Bloom filter no compite: complementa** (por eso no esta en la
  tabla). Un filtro de 2 MB delante del hash clasico baja su fallo de
  580 ms a 259 ms a cambio de un 50% mas en los aciertos: rentable
  cuando dominan los fallos (el truco de las LSM y las caches). Esta
  implementado y medible con `./bench_estructuras bl`.
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
