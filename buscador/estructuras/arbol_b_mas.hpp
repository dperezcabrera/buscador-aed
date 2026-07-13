/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@@@@@@@@@@   ESTRUCTURA: ARBOL B (MAS)  @@@@@@@@@@@@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

// valores solo en las hojas, hojas enlazadas entre si: el
// scan/rango es un recorrido secuencial sin subir al arbol

#ifndef ARBOL_B_MAS_HPP
#define ARBOL_B_MAS_HPP

#include "arbol_b.hpp" // por el grado G

class BPlus
{
 private:
  /*/ nodo: hoja o interno /*/
  struct N
  {
    bool  hoja ;
    int      n ;
    string k[2*G]   ;
    int    v[2*G]   ; // solo hojas
    N*     sig      ; // solo hojas
    N*     h[2*G+1] ; // solo internos

    N(bool ho) : hoja(ho), n(0), sig(NULL) {
      for( int i = 0 ; i < 2*G+1 ; i++ )
	h[i] = NULL ;
    }
  };

  /*/ elementos /*/
  N*       raiz ;
  long   nnodos ;
  bool      dup ;
  bool  quitado ;

  /*/ operaciones /*/
  // inserta descendiendo; si el nodo se parte devuelve el
  // nuevo hermano derecho y deja el separador en sep
  N* inserta(N* x, const string& clave, int valor, string& sep) {
    if ( x->hoja ) {
      int i = std::lower_bound( x->k, x->k + x->n, clave ) - x->k ;
      if ( ( i < x->n ) and ( x->k[i] == clave ) ) {
	dup = true ;
	return NULL ;
      }
      for( int j = x->n-1 ; j >= i ; j-- ) {
	x->k[j+1] = std::move(x->k[j]) ;
	x->v[j+1] = x->v[j] ;
      }
      x->k[i] = clave ;
      x->v[i] = valor ;
      x->n++ ;
      if ( x->n < 2*G )
	return NULL ;
      N* d = new N(true) ;
      nnodos++ ;
      for( int j = 0 ; j < G ; j++ ) {
	d->k[j] = std::move(x->k[j+G]) ;
	d->v[j] = x->v[j+G] ;
      }
      d->n = G ;
      x->n = G ;
      d->sig = x->sig ;
      x->sig = d ;
      sep = d->k[0] ; // el separador se COPIA arriba
      return d ;
    }
    int i = std::upper_bound( x->k, x->k + x->n, clave ) - x->k ;
    string s ;
    N* nuevo = inserta( x->h[i], clave, valor, s ) ;
    if ( not nuevo )
      return NULL ;
    for( int j = x->n-1 ; j >= i ; j-- )
      x->k[j+1] = std::move(x->k[j]) ;
    for( int j = x->n ; j > i ; j-- )
      x->h[j+1] = x->h[j] ;
    x->k[i]   = std::move(s) ;
    x->h[i+1] = nuevo ;
    x->n++ ;
    if ( x->n < 2*G )
      return NULL ;
    N* d = new N(false) ;
    nnodos++ ;
    for( int j = 0 ; j < G-1 ; j++ )
      d->k[j] = std::move(x->k[j+G+1]) ;
    for( int j = 0 ; j < G ; j++ )
      d->h[j] = x->h[j+G+1] ;
    d->n = G-1 ;
    sep  = std::move(x->k[G]) ; // el separador interno SUBE
    x->n = G ;
    return d ;
  }

  N* hoja_de(const string& clave) {
    N* x = raiz ;
    while ( not x->hoja ) {
      int i = std::upper_bound( x->k, x->k + x->n, clave ) - x->k ;
      x = x->h[i] ;
    }
    return x ;
  }

  /*/ auxiliares del borrado /*/
  // minimos: G claves en hojas, G-1 separadores en internos

  bool sobra(N* y) { return y->hoja ? ( y->n > G ) : ( y->n > G-1 ) ; }

  void presta_izq(N* x, int i) { // hijo i recibe del hermano i-1
    N *c = x->h[i], *izq = x->h[i-1] ;
    if ( c->hoja ) {
      for( int j = c->n-1 ; j >= 0 ; j-- ) {
	c->k[j+1] = std::move(c->k[j]) ;
	c->v[j+1] = c->v[j] ;
      }
      c->k[0] = std::move(izq->k[izq->n-1]) ;
      c->v[0] = izq->v[izq->n-1] ;
      c->n++ ;
      izq->n-- ;
      x->k[i-1] = c->k[0] ; // el separador se copia de la nueva primera
    }
    else {
      for( int j = c->n-1 ; j >= 0 ; j-- )
	c->k[j+1] = std::move(c->k[j]) ;
      for( int j = c->n ; j >= 0 ; j-- )
	c->h[j+1] = c->h[j] ;
      c->k[0] = std::move(x->k[i-1]) ;
      c->h[0] = izq->h[izq->n] ;
      x->k[i-1] = std::move(izq->k[izq->n-1]) ;
      c->n++ ;
      izq->n-- ;
    }
  }

  void presta_der(N* x, int i) { // hijo i recibe del hermano i+1
    N *c = x->h[i], *der = x->h[i+1] ;
    if ( c->hoja ) {
      c->k[c->n] = std::move(der->k[0]) ;
      c->v[c->n] = der->v[0] ;
      c->n++ ;
      for( int j = 0 ; j < der->n-1 ; j++ ) {
	der->k[j] = std::move(der->k[j+1]) ;
	der->v[j] = der->v[j+1] ;
      }
      der->n-- ;
      x->k[i] = der->k[0] ; // el separador se copia del nuevo primero
    }
    else {
      c->k[c->n]   = std::move(x->k[i]) ;
      c->h[c->n+1] = der->h[0] ;
      c->n++ ;
      x->k[i] = std::move(der->k[0]) ;
      for( int j = 0 ; j < der->n-1 ; j++ )
	der->k[j] = std::move(der->k[j+1]) ;
      for( int j = 0 ; j < der->n ; j++ )
	der->h[j] = der->h[j+1] ;
      der->n-- ;
    }
  }

  void une(N* x, int i) { // hijo i absorbe al hermano i+1
    N *c = x->h[i], *der = x->h[i+1] ;
    if ( c->hoja ) {
      for( int j = 0 ; j < der->n ; j++ ) {
	c->k[c->n+j] = std::move(der->k[j]) ;
	c->v[c->n+j] = der->v[j] ;
      }
      c->n += der->n ;
      c->sig = der->sig ;
    }
    else {
      c->k[c->n] = std::move(x->k[i]) ; // el separador baja
      for( int j = 0 ; j < der->n ; j++ )
	c->k[c->n+1+j] = std::move(der->k[j]) ;
      for( int j = 0 ; j <= der->n ; j++ )
	c->h[c->n+1+j] = der->h[j] ;
      c->n += der->n + 1 ;
    }
    for( int j = i ; j < x->n-1 ; j++ )
      x->k[j] = std::move(x->k[j+1]) ;
    for( int j = i+1 ; j < x->n ; j++ )
      x->h[j] = x->h[j+1] ;
    x->n-- ;
    delete der ;
    nnodos-- ;
  }

  // borra descendiendo; devuelve true si x quedo bajo minimos
  bool borra(N* x, const string& clave) {
    if ( x->hoja ) {
      int i = std::lower_bound( x->k, x->k + x->n, clave ) - x->k ;
      if ( ( i >= x->n ) or ( x->k[i] != clave ) )
	return false ;
      quitado = true ;
      for( int j = i ; j < x->n-1 ; j++ ) {
	x->k[j] = std::move(x->k[j+1]) ;
	x->v[j] = x->v[j+1] ;
      }
      x->n-- ;
      return x->n < G ;
    }
    int i = std::upper_bound( x->k, x->k + x->n, clave ) - x->k ;
    if ( not borra( x->h[i], clave ) )
      return false ;
    // el hijo i quedo bajo minimos: prestar o fusionar
    if ( ( i > 0 ) and sobra(x->h[i-1]) ) {
      presta_izq(x,i) ;
      return false ;
    }
    if ( ( i < x->n ) and sobra(x->h[i+1]) ) {
      presta_der(x,i) ;
      return false ;
    }
    if ( i == x->n )
      i-- ;
    une(x,i) ;
    return x->n < G-1 ;
  }

  void libera(N* x) {
    if ( not x->hoja )
      for( int i = 0 ; i <= x->n ; i++ )
	libera(x->h[i]) ;
    delete x ;
  }

 public:
  /*/ operaciones /*/
  BPlus() : nnodos(1), dup(false), quitado(false) {
    raiz = new N(true) ;
  }

  ~BPlus() {
    libera(raiz) ;
  }

  bool insert(const string& clave,int valor) {
    dup = false ;
    string s ;
    N* nuevo = inserta( raiz, clave, valor, s ) ;
    if ( nuevo ) {
      N* r = new N(false) ;
      nnodos++ ;
      r->h[0] = raiz ;
      r->h[1] = nuevo ;
      r->k[0] = std::move(s) ;
      r->n = 1 ;
      raiz = r ;
    }
    return not dup ;
  }

  int* search(const string& clave) {
    N* x = hoja_de(clave) ;
    int i = std::lower_bound( x->k, x->k + x->n, clave ) - x->k ;
    return ( ( i < x->n ) and ( x->k[i] == clave ) ) ? &x->v[i] : NULL ;
  }

  bool erase(const string& clave) { // borrado completo, con fusion de hojas
    quitado = false ;
    borra( raiz, clave ) ;
    if ( ( not raiz->hoja ) and ( raiz->n == 0 ) ) {
      N* vieja = raiz ;
      raiz = raiz->h[0] ;
      delete vieja ;
      nnodos-- ;
    }
    return quitado ;
  }

  long scan() { // recorrido secuencial por las hojas enlazadas
    long acc = 0 ;
    N* x = raiz ;
    while ( not x->hoja )
      x = x->h[0] ;
    for( ; x ; x = x->sig )
      for( int i = 0 ; i < x->n ; i++ )
	acc += x->v[i] ;
    return acc ;
  }

  long Nodos() { return nnodos ; }
};

#endif
