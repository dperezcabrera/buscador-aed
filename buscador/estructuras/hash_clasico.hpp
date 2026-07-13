/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@   ESTRUCTURA: HASH CLASICO CON ENCADENAMIENTO   @@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#ifndef HASH_CLASICO_HPP
#define HASH_CLASICO_HPP

#include     <string>
#include    <cstddef>

using std::string;

int f_hash(const string&) ; // definida en estructura_datos.cpp

/*/ nodo de la cadena /*/
struct NodoC
{
  string clave ;
  int    valor ;
  NodoC*   sig ;
};

class ClassicHash
{
 private:
  /*/ elementos /*/
  NodoC** cubetas ;
  int        tama ;

  /*/ operaciones /*/
  int idx(const string& k) {
    int h = f_hash(k) % tama ;
    return h < 0 ? h + tama : h ;
  }

 public:
  /*/ operaciones /*/
  ClassicHash(int n) : tama(n) {
    cubetas = new NodoC*[n]() ;
  }

  bool insert(const string& k,int v) {
    int h = idx(k) ;
    for( NodoC* p = cubetas[h] ; p ; p = p->sig )
      if ( p->clave == k )
	return false ;
    cubetas[h] = new NodoC{k,v,cubetas[h]} ;
    return true ;
  }

  int* search(const string& k) {
    for( NodoC* p = cubetas[idx(k)] ; p ; p = p->sig )
      if ( p->clave == k )
	return &p->valor ;
    return NULL ;
  }

  bool erase(const string& k) {
    int h = idx(k) ;
    for( NodoC** pp = &cubetas[h] ; *pp ; pp = &(*pp)->sig )
      if ( (*pp)->clave == k ) {
	NodoC* m = *pp ;
	*pp = m->sig ;
	delete m ;
	return true ;
      }
    return false ;
  }

  long scan() { return -1 ; } // sin orden
};

#endif
