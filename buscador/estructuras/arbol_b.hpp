/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@@@@@@@@@   ESTRUCTURA: ARBOL B  PURO   @@@@@@@@@@@@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

// CLRS: particion preventiva de nodos llenos en el descenso

#ifndef ARBOL_B_HPP
#define ARBOL_B_HPP

#include     <string>
#include  <algorithm>
#include    <utility>
#include    <cstddef>

using std::string;

const int G = 32 ; // grado minimo: nodos con [G-1, 2G-1] claves

/*/ nodo del arbol /*/
struct NodoB
{
  int                  n ;
  bool              hoja ;
  string claves[2*G-1]   ;
  int    val   [2*G-1]   ;
  bool   muerto[2*G-1]   ;
  NodoB* hijo  [2*G]     ;

  NodoB() : n(0), hoja(true) {
    for( int i = 0 ; i < 2*G ; i++ )
      hijo[i] = NULL ;
  }
};

class BTree
{
 protected:
  /*/ elementos /*/
  NodoB*   raiz ;
  long   nnodos ;
  long      acc ;

  /*/ operaciones /*/
  int pos(NodoB* x, const string& k) {
    return std::lower_bound( x->claves, x->claves + x->n, k ) - x->claves ;
  }

  void split(NodoB* p, int i) {
    NodoB *y = p->hijo[i], *z = new NodoB() ;
    nnodos++ ;
    z->hoja = y->hoja ;
    z->n = G-1 ;
    for( int j = 0 ; j < G-1 ; j++ ) {
      z->claves[j] = std::move(y->claves[j+G]) ;
      z->val[j]    = y->val[j+G] ;
      z->muerto[j] = y->muerto[j+G] ;
    }
    if ( not y->hoja )
      for( int j = 0 ; j < G ; j++ )
	z->hijo[j] = y->hijo[j+G] ;
    y->n = G-1 ;
    for( int j = p->n ; j > i ; j-- )
      p->hijo[j+1] = p->hijo[j] ;
    for( int j = p->n-1 ; j >= i ; j-- ) {
      p->claves[j+1] = std::move(p->claves[j]) ;
      p->val[j+1]    = p->val[j] ;
      p->muerto[j+1] = p->muerto[j] ;
    }
    p->hijo[i+1]  = z ;
    p->claves[i]  = std::move(y->claves[G-1]) ;
    p->val[i]     = y->val[G-1] ;
    p->muerto[i]  = y->muerto[G-1] ;
    p->n++ ;
  }

  virtual void lleno_gestionado(NodoB* x, int i) { // en B puro: partir siempre
    split(x,i) ;
  }

  void inorden(NodoB* x) {
    for( int i = 0 ; i < x->n ; i++ ) {
      if ( not x->hoja )
	inorden(x->hijo[i]) ;
      if ( not x->muerto[i] )
	acc += x->val[i] ;
    }
    if ( not x->hoja )
      inorden(x->hijo[x->n]) ;
  }

 public:
  /*/ operaciones /*/
  BTree() : nnodos(1) {
    raiz = new NodoB() ;
  }

  bool insert(const string& k, int v) {
    if ( raiz->n == 2*G-1 ) {
      NodoB* s = new NodoB() ;
      nnodos++ ;
      s->hoja = false ;
      s->hijo[0] = raiz ;
      raiz = s ;
      split(s,0) ;
    }
    NodoB* x = raiz ;
    while ( true ) {
      int i = pos(x,k) ;
      if ( ( i < x->n ) and ( x->claves[i] == k ) )
	return false ;
      if ( x->hoja ) {
	for( int j = x->n-1 ; j >= i ; j-- ) {
	  x->claves[j+1] = std::move(x->claves[j]) ;
	  x->val[j+1]    = x->val[j] ;
	  x->muerto[j+1] = x->muerto[j] ;
	}
	x->claves[i] = k ;
	x->val[i]    = v ;
	x->muerto[i] = false ;
	x->n++ ;
	return true ;
      }
      if ( x->hijo[i]->n == 2*G-1 ) {
	lleno_gestionado(x,i) ;
	continue ;
      }
      x = x->hijo[i] ;
    }
  }

  int* search(const string& k) {
    NodoB* x = raiz ;
    while ( true ) {
      int i = pos(x,k) ;
      if ( ( i < x->n ) and ( x->claves[i] == k ) )
	return x->muerto[i] ? NULL : &x->val[i] ;
      if ( x->hoja )
	return NULL ;
      x = x->hijo[i] ;
    }
  }

  bool erase(const string& k) { // marcado
    NodoB* x = raiz ;
    while ( true ) {
      int i = pos(x,k) ;
      if ( ( i < x->n ) and ( x->claves[i] == k ) ) {
	if ( x->muerto[i] )
	  return false ;
	x->muerto[i] = true ;
	return true ;
      }
      if ( x->hoja )
	return false ;
      x = x->hijo[i] ;
    }
  }

  long scan() {
    acc = 0 ;
    inorden(raiz) ;
    return acc ;
  }

  long Nodos() { return nnodos ; }
};

#endif
