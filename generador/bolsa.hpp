//////////////////////////////////////////////////////////
// TIPO DE DATOS GENERICO: Bolsa<T>
//   DESCRIPCION:
//     Los objetos de tipo Bolsa<T> almacenan colecciones
//     no ordenadas y con repeticion de elementos de tipo T.
//     El tamaño de las bolsas no esta restringido.
//     La operacion cualquiera(), devuelve un elemento
//     elegido aleatoriamente de la bolsa.
//////////////////////////////////////////////////////////

#ifndef _LIB_BOLSAS
#define _LIB_BOLSAS

#include <assert.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

////////////////////
// DEFINICION     //
////////////////////

template <class T>
class Bolsa {
  private:
    T *datos;
    int tamano;
    int ultimo;
  public: 
    Bolsa (int tamInicial= 1000);    //Constructor
    ~Bolsa ();                       //Destructor
    void vaciar () { ultimo= 0; }
    void insertar (T valor);
    T cualquiera ();
    int numElems () {return ultimo;}
    void escribir ();
};


#define ALEATORIO(maximo) (rand()%(maximo))


////////////////////
// IMPLEMENTACION //
////////////////////


template <class T>
Bolsa<T>::Bolsa (int tamInicial)
{
  assert(tamInicial>0);     //Precondicion 
  datos= new T[tamInicial];
  tamano= tamInicial;
  vaciar();
  assert(datos!=NULL);     //Postcondicion
}

template <class T>
Bolsa<T>::~Bolsa ()
{
  delete[] datos;
}

template <class T>
void Bolsa<T>::insertar (T valor)
{
  if (ultimo>=tamano) {          // Si se sobrepasa el tamaño actual
    T *ndatos= new T[tamano*2];  // redimensionar el array datos
    assert(ndatos!=NULL);
    for (int i= 0; i<ultimo; i++)
      ndatos[i]= datos[i];
    tamano*= 2;
    delete[] datos;
    datos= ndatos;
  }
  datos[ultimo++]= valor;
}

template <class T>
T Bolsa<T>::cualquiera ()
{
  return datos[ALEATORIO(ultimo)];
}

template <class T>
void Bolsa<T>::escribir ()
{
  cout << "\nBolsa: ";
  for (int i= 0; i<ultimo; i++)
    cout << datos[i] <<" ";
  cout << "\n";
}


#endif
