//////////////////////////////////////////////////////////
// PROGRAMA PRINCIPAL 
//////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

#include "generador.hpp"


////////////////////
// IMPLEMENTACION //
////////////////////

#define _TAM_BUFFER      300

int main (void)
{
  FILE *fm;
  char nEnt[_TAM_BUFFER], nMaes[_TAM_BUFFER], nSal[_TAM_BUFFER];
  cout << "   AED - Curso 2003/2004 - Gines Garcia Mateos - UM\n";
  cout << "       Generador de conjuntos de paginas web\n";
  cout << "            Version 0.0.  Diciembre 2003\n\n";
  cout << "Nombre del fichero de palabras (lectura): ";
  cin >> nEnt;
  cout << "Nombre del fichero maestro (escritura): ";
  cin >> nMaes;
  fm= fopen(nMaes, "w");
  assert(fm!=NULL);
  cout << "Directorio base de salida: ";
  cin >> nSal;
  mkdir(nSal, 0777);
  int nPags, npMin, npMax;
  cout << "Numero de paginas a generar: ";
  cin >> nPags;
  cout << "Numero minimo de palabras por pagina: ";
  cin >> npMin;
  cout << "Numero maximo de palabras por pagina: ";
  cin >> npMax;
  cout << "Leyendo fichero de palabras...\n";
  Generador *gen= new Generador(nEnt, nSal, npMin, npMax);
  gen->escribirInfo();
  fprintf(fm, "################################################\n");
  fprintf(fm, "# TOTAL\n# Paginas: %d      Palabras distintas: %d\n", nPags, gen->numPals());
  fprintf(fm, "# Palabras por pagina: %d - %d\n\n", npMin, npMax);
  for (int i= 0; i<nPags; i++) {
    cout << ".";
    gen->escribirDescrip(fm);
    gen->escribirPagina();
    gen->avanzar();
  }
  cout << "\nHecho.\n";
  delete gen;
  return 0;
}
