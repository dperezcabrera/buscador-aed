/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@@@@@@@@@   ESTRUCTURA: ARBOL B  PURO   @@@@@@@@@@@@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

// CLRS: particion preventiva al insertar, y en el borrado
// prestamo de hermanos o fusion antes de descender

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
    }
    p->hijo[i+1]  = z ;
    p->claves[i]  = std::move(y->claves[G-1]) ;
    p->val[i]     = y->val[G-1] ;
    p->n++ ;
  }

  virtual void lleno_gestionado(NodoB* x, int i) { // en B puro: partir siempre
    split(x,i) ;
  }

  /*/ auxiliares del borrado /*/

  void presta_izq(NodoB* x, int i) { // hijo i recibe del hermano i-1
    NodoB *c = x->hijo[i], *izq = x->hijo[i-1] ;
    for( int j = c->n-1 ; j >= 0 ; j-- ) {
      c->claves[j+1] = std::move(c->claves[j]) ;
      c->val[j+1]    = c->val[j] ;
    }
    if ( not c->hoja )
      for( int j = c->n ; j >= 0 ; j-- )
	c->hijo[j+1] = c->hijo[j] ;
    c->claves[0] = std::move(x->claves[i-1]) ;
    c->val[0]    = x->val[i-1] ;
    if ( not c->hoja )
      c->hijo[0] = izq->hijo[izq->n] ;
    x->claves[i-1] = std::move(izq->claves[izq->n-1]) ;
    x->val[i-1]    = izq->val[izq->n-1] ;
    c->n++ ;
    izq->n-- ;
  }

  void presta_der(NodoB* x, int i) { // hijo i recibe del hermano i+1
    NodoB *c = x->hijo[i], *der = x->hijo[i+1] ;
    c->claves[c->n] = std::move(x->claves[i]) ;
    c->val[c->n]    = x->val[i] ;
    if ( not c->hoja )
      c->hijo[c->n+1] = der->hijo[0] ;
    x->claves[i] = std::move(der->claves[0]) ;
    x->val[i]    = der->val[0] ;
    for( int j = 0 ; j < der->n-1 ; j++ ) {
      der->claves[j] = std::move(der->claves[j+1]) ;
      der->val[j]    = der->val[j+1] ;
    }
    if ( not der->hoja )
      for( int j = 0 ; j < der->n ; j++ )
	der->hijo[j] = der->hijo[j+1] ;
    c->n++ ;
    der->n-- ;
  }

  void une(NodoB* x, int i) { // hijo i absorbe el separador i y el hijo i+1
    NodoB *c = x->hijo[i], *der = x->hijo[i+1] ;
    c->claves[c->n] = std::move(x->claves[i]) ;
    c->val[c->n]    = x->val[i] ;
    for( int j = 0 ; j < der->n ; j++ ) {
      c->claves[c->n+1+j] = std::move(der->claves[j]) ;
      c->val[c->n+1+j]    = der->val[j] ;
    }
    if ( not c->hoja )
      for( int j = 0 ; j <= der->n ; j++ )
	c->hijo[c->n+1+j] = der->hijo[j] ;
    c->n += der->n + 1 ;
    for( int j = i ; j < x->n-1 ; j++ ) {
      x->claves[j] = std::move(x->claves[j+1]) ;
      x->val[j]    = x->val[j+1] ;
    }
    for( int j = i+1 ; j < x->n ; j++ )
      x->hijo[j] = x->hijo[j+1] ;
    x->n-- ;
    delete der ;
    nnodos-- ;
  }

  // invariante: x tiene al menos G claves (salvo la raiz)
  bool borra(NodoB* x, const string& k) {
    int i = pos(x,k) ;
    if ( ( i < x->n ) and ( x->claves[i] == k ) ) {
      if ( x->hoja ) {
	for( int j = i ; j < x->n-1 ; j++ ) {
	  x->claves[j] = std::move(x->claves[j+1]) ;
	  x->val[j]    = x->val[j+1] ;
	}
	x->n-- ;
	return true ;
      }
      if ( x->hijo[i]->n >= G ) {           // sustituir por el predecesor
	NodoB* m = x->hijo[i] ;
	while ( not m->hoja )
	  m = m->hijo[m->n] ;
	x->claves[i] = m->claves[m->n-1] ;
	x->val[i]    = m->val[m->n-1] ;
	return borra( x->hijo[i], x->claves[i] ) ;
      }
      if ( x->hijo[i+1]->n >= G ) {         // sustituir por el sucesor
	NodoB* m = x->hijo[i+1] ;
	while ( not m->hoja )
	  m = m->hijo[0] ;
	x->claves[i] = m->claves[0] ;
	x->val[i]    = m->val[0] ;
	return borra( x->hijo[i+1], x->claves[i] ) ;
      }
      une(x,i) ;                            // ambos al minimo: fusionar
      return borra( x->hijo[i], k ) ;
    }
    if ( x->hoja )
      return false ;
    // asegurar que el hijo por el que bajamos tiene >= G claves
    if ( x->hijo[i]->n == G-1 ) {
      if      ( ( i > 0 )    and ( x->hijo[i-1]->n >= G ) ) presta_izq(x,i) ;
      else if ( ( i < x->n ) and ( x->hijo[i+1]->n >= G ) ) presta_der(x,i) ;
      else {
	if ( i == x->n )
	  i-- ;
	une(x,i) ;
      }
    }
    return borra( x->hijo[i], k ) ;
  }

  void inorden(NodoB* x) {
    for( int i = 0 ; i < x->n ; i++ ) {
      if ( not x->hoja )
	inorden(x->hijo[i]) ;
      acc += x->val[i] ;
    }
    if ( not x->hoja )
      inorden(x->hijo[x->n]) ;
  }

  void libera(NodoB* x) {
    if ( not x->hoja )
      for( int i = 0 ; i <= x->n ; i++ )
	libera(x->hijo[i]) ;
    delete x ;
  }

 public:
  /*/ operaciones /*/
  BTree() : nnodos(1) {
    raiz = new NodoB() ;
  }

  virtual ~BTree() {
    libera(raiz) ;
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
	}
	x->claves[i] = k ;
	x->val[i]    = v ;
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
	return &x->val[i] ;
      if ( x->hoja )
	return NULL ;
      x = x->hijo[i] ;
    }
  }

  bool erase(const string& k) { // borrado completo, con fusion de nodos
    bool hecho = borra( raiz, k ) ;
    if ( ( raiz->n == 0 ) and ( not raiz->hoja ) ) {
      NodoB* vieja = raiz ;
      raiz = raiz->hijo[0] ;
      delete vieja ;
      nnodos-- ;
    }
    return hecho ;
  }

  long scan() {
    acc = 0 ;
    inorden(raiz) ;
    return acc ;
  }

  long Nodos() { return nnodos ; }
};

#endif
