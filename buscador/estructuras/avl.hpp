/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@@@@@@@@@@@@   ESTRUCTURA:  AVL   @@@@@@@@@@@@@@@@@@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#ifndef AVL_HPP
#define AVL_HPP

#include     <string>
#include  <algorithm>
#include    <cstddef>

using std::string;

/*/ nodo del arbol /*/
struct NodoA
{
  string  clave ;
  int     valor ;
  NodoA*    izq ;
  NodoA*    der ;
  int       alt ;
};

class AVL
{
 private:
  /*/ elementos /*/
  NodoA*      raiz ;
  bool   insertado ;
  long         acc ;

  /*/ operaciones /*/
  static int alt(NodoA* n) { return n ? n->alt : 0 ; }

  static void act(NodoA* n) {
    n->alt = 1 + std::max( alt(n->izq), alt(n->der) ) ;
  }

  static NodoA* rotd(NodoA* y) {
    NodoA* x = y->izq ;
    y->izq = x->der ;
    x->der = y ;
    act(y) ;
    act(x) ;
    return x ;
  }

  static NodoA* roti(NodoA* x) {
    NodoA* y = x->der ;
    x->der = y->izq ;
    y->izq = x ;
    act(x) ;
    act(y) ;
    return y ;
  }

  static NodoA* balancea(NodoA* n) {
    act(n) ;
    int b = alt(n->izq) - alt(n->der) ;
    if ( b > 1 ) {
      if ( alt(n->izq->izq) < alt(n->izq->der) )
	n->izq = roti(n->izq) ;
      return rotd(n) ;
    }
    if ( b < -1 ) {
      if ( alt(n->der->der) < alt(n->der->izq) )
	n->der = rotd(n->der) ;
      return roti(n) ;
    }
    return n ;
  }

  NodoA* inserta(NodoA* n, const string& k, int v) {
    if ( not n ) {
      insertado = true ;
      return new NodoA{k,v,NULL,NULL,1} ;
    }
    if      ( k < n->clave ) n->izq = inserta(n->izq,k,v) ;
    else if ( n->clave < k ) n->der = inserta(n->der,k,v) ;
    else {
      insertado = false ;
      return n ;
    }
    return balancea(n) ;
  }

  NodoA* borra(NodoA* n, const string& k, bool& hecho) {
    if ( not n )
      return NULL ;
    if      ( k < n->clave ) n->izq = borra(n->izq,k,hecho) ;
    else if ( n->clave < k ) n->der = borra(n->der,k,hecho) ;
    else {
      hecho = true ;
      if ( ( not n->izq ) or ( not n->der ) ) {
	NodoA* h = n->izq ? n->izq : n->der ;
	delete n ;
	return h ;
      }
      NodoA* s = n->der ; // sucesor: minimo del subarbol derecho
      while ( s->izq )
	s = s->izq ;
      n->clave = s->clave ;
      n->valor = s->valor ;
      bool aux = false ;
      n->der = borra( n->der, n->clave, aux ) ;
    }
    return balancea(n) ;
  }

  void inorden(NodoA* n) {
    if ( not n )
      return ;
    inorden(n->izq) ;
    acc += n->valor ;
    inorden(n->der) ;
  }

  void libera(NodoA* n) {
    if ( not n )
      return ;
    libera(n->izq) ;
    libera(n->der) ;
    delete n ;
  }

 public:
  /*/ operaciones /*/
  AVL() : raiz(NULL) {}

  ~AVL() {
    libera(raiz) ;
  }

  bool insert(const string& k,int v) {
    raiz = inserta(raiz,k,v) ;
    return insertado ;
  }

  int* search(const string& k) {
    for( NodoA* n = raiz ; n ; ) {
      if      ( k < n->clave ) n = n->izq ;
      else if ( n->clave < k ) n = n->der ;
      else return &n->valor ;
    }
    return NULL ;
  }

  bool erase(const string& k) { // con rebalanceo
    bool hecho = false ;
    raiz = borra( raiz, k, hecho ) ;
    return hecho ;
  }

  long scan() {
    acc = 0 ;
    inorden(raiz) ;
    return acc ;
  }
};

#endif
