# buscador-aed

Practica de Algoritmos y Estructuras de Datos (Universidad de Murcia,
curso 2003/2004, 2 de ITI de Gestion): un buscador de paginas web con
indice invertido posicional, tabla hash propia y ranking por relevancia.
Calificacion original: sobresaliente.

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

| Estructura | Memoria | insert | hit | miss | scan orden | erase |
|---|---|---|---|---|---|---|
| Flat hash (sondeo lineal) | 61 MB | 287 | 696 | 184 | — | 217 |
| Hash clasico (encadenado) | 91 MB | 350 | 769 | 685 | — | 270 |
| close_hash (2004, arreglada) | 122 MB | 369 | 884 | 647 | — | 242 |
| Bloom (2 MB) + clasico | 93 MB | 445 | 1.074 | 208 | — | 272 |
| Radix trie (Patricia) | 67 MB | 437 | 1.747 | 7* | — | 606* |
| AVL | 106 MB | 687 | 1.129 | 210 | 29 ms | 373* |
| Arbol B (G=32) | 105 MB | 905 | 2.471 | 248 | 6 ms | 852* |
| Arbol B+ | 110 MB | 888 | 2.830 | 280 | 5 ms | 943* |
| Arbol B* | 98 MB | 1.244 | 2.828 | 231 | 5 ms | 1.035* |
| Trie plano | 240 MB | 1.447 | 2.877 | 8* | — | 1.062* |

Tiempos en ms. `*` erase por marcado (tombstone); miss de trie/radix y de
los arboles dependiente del corpus (las claves ausentes divergen pronto).

### Conclusiones

- **Para busqueda exacta, hash** — la eleccion de la practica era la
  correcta. Todo lo demas paga 2-4x el hit a cambio de operaciones que el
  buscador no necesita.
- **El flat hash gana la categoria**: arrays planos sin punteros, la
  mejor memoria de la tabla, y miss 3-4x mejor que los encadenados (una
  celda vacia responde sin comparar strings). Es la razon de que los
  hashes planos (Swiss tables) dominen hoy.
- **La close_hash queda a un 5-15% del clasico en insert y hit, y le
  empata o gana en miss y erase** tras las correcciones (partia de un 50%
  de desventaja por firmas por valor, y de perder claves por el bug del
  caso 3). Es la unica de la tabla con factor de carga <= 1 garantizado
  por construccion.
- **La compresion de caminos transforma el trie**: de 240 a 67 MB y casi
  el doble de velocidad, conservando el miss instantaneo y la busqueda
  por prefijo, que ningun hash puede ofrecer.
- **Un Bloom filter de 2 MB** baja el miss del clasico de 685 a 208 ms a
  cambio de +40% en los hits: rentable cuando dominan los fallos (el
  truco de las LSM y las caches).
- **Los arboles B ganan en recorrido ordenado** (5-6 ms el scan completo,
  5x mejor que el AVL) y el AVL en busqueda puntual ordenada; el B+ es lo
  que usan las bases de datos porque su ventaja real (hojas enlazadas =
  rangos secuenciales, internos sin valores = arbol bajo) se materializa
  con paginas de disco, no en RAM. El B* cumple su promesa historica: 11%
  menos nodos a cambio de inserciones mas caras.

## Estructura del repositorio

```
buscador/               codigo del buscador + DISENO.md + tests + benchmark
buscador/estructuras/   las estructuras del benchmark, una por fichero
generador/              generador de conjuntos de prueba (codigo de los profesores)
```
