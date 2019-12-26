//////////////////////////////////////////////////////////
// TIPO DE DATOS GENERICO: Conjunto<T>
//   DESCRIPCION:
//     Los objetos de tipo Conjunto<T> almacenan colecciones
//     no ordenadas y sin repeticion de elementos de tipo T.
//     Para crear un conjunto nuevo es necesario indicar
//     la funcion de comparacion entre elementos.
//     El tamaño de los conjuntos no esta restringido.
//////////////////////////////////////////////////////////

#ifndef _LIB_CONJUNTOS
#define _LIB_CONJUNTOS

#include <ctype.h>
#include <iostream>

using namespace std;

////////////////////
// DEFINICION     //
////////////////////

template <class T>
class Conjunto {          
  private:
    T *datos;
    int tamano;
    int ultimo;
    bool (*fcompara)(T v1, T v2);  //Puntero a una funcion de comparacion en T
  public:
    Conjunto (bool (*fc)(T v1, T v2), int tamInicial= 1000);  //Constructor
    ~Conjunto ();                       //Destructor
    void vaciar () { ultimo= 0; }
    void insertar (T valor);
    bool miembro (T valor);
    int numElems () { return ultimo; }
    void escribir ();
};


////////////////////
// IMPLEMENTACION //
////////////////////


template <class T>
Conjunto<T>::Conjunto (bool (*fc)(T v1, T v2), int tamInicial)
{
  assert(tamInicial>0);     //Precondicion
  datos= new T[tamInicial];
  tamano= tamInicial;
  fcompara= fc;
  vaciar();
  assert(datos!=NULL);      //Postcondicion
}

template <class T>
Conjunto<T>::~Conjunto ()
{
  delete[] datos;
}

template <class T>
void Conjunto<T>::insertar (T valor)
{
  if (!miembro(valor)) {   //Los elementos no pueden estar repetidos
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
}

template <class T>
bool Conjunto<T>::miembro (T valor)
{
   for (int i= 0; i<ultimo; i++)
     if ((*fcompara)(datos[i], valor))
       return true;
   return false;
}

template <class T>
void Conjunto<T>::escribir ()
{
  cout << "\nConjunto: ";
  for (int i= 0; i<ultimo; i++)
    cout << datos[i] <<" ";
  cout << "\n";
}


#endif
