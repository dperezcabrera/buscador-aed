//////////////////////////////////////////////////////////
// TIPO DE DATOS: Generador
//   DESCRIPCION:
//     Los objetos de tipo Generador permiten generar
//     conjuntos de paginas de prueba para el problema del
//     buscador web, incluyendo la descripcion del fichero
//     maestro.
//     En la inicialización se debe indicar un fichero base
//     (del que se extraen las palabras a usar), el subdi-
//     rectorio donde colocar las paginas y el numero minimo
//     y maximo de palabras por pagina.
//     Las paginas se deben generar una a una, con las funciones
//     escribeDescripcion() y escribeFichero(). Despues de
//     escribir una pagina se debe aplicar la funcion
//     avanza().
//////////////////////////////////////////////////////////

#ifndef _LIB_GENERADOR
#define _LIB_GENERADOR

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <assert.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>

#include "bolsa.hpp"
#include "conjunto.hpp"
using namespace std;

////////////////////
// DEFINICION     //
////////////////////

class Generador
{
  private:
    Bolsa<char *> *palabras;      //Palabras usadas para los ejemplos
    Conjunto<char *> *distintas;  //Palabras distintas existentes
    char *letras;                 //Array con el contenido del fichero
    int numLetras;
    char const *subdir;
    int idPagina;
    char *nombreAct, *url, *titulo;
    int relevancia;
    int numPalMin, numPalMax;
    int numPalabras;
    char const* unaPalabra(unsigned maxLong, int maxIntentos= 10);
  public: 
    Generador (char *fichBase, char const *subdirec= ".", int npMin=10, int npMax=100);
    ~Generador ();
    void escribirDescrip (FILE *fmaestro);
    void escribirPagina ();
    void avanzar ();
    void escribirInfo ();
    int numPals () {return distintas->numElems();}
};

bool comparaStr(char *cad1, char *cad2);  //Funcion de comparacion de cadenas
                                          // independiente de caso y tildes

#endif
