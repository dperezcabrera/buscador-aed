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
    bool   m[2*G]   ; // solo hojas
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
	x->m[j+1] = x->m[j] ;
      }
      x->k[i] = clave ;
      x->v[i] = valor ;
      x->m[i] = false ;
      x->n++ ;
      if ( x->n < 2*G )
	return NULL ;
      N* d = new N(true) ;
      nnodos++ ;
      for( int j = 0 ; j < G ; j++ ) {
	d->k[j] = std::move(x->k[j+G]) ;
	d->v[j] = x->v[j+G] ;
	d->m[j] = x->m[j+G] ;
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

 public:
  /*/ operaciones /*/
  BPlus() : nnodos(1), dup(false) {
    raiz = new N(true) ;
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
    return ( ( i < x->n ) and ( x->k[i] == clave ) and ( not x->m[i] ) )
	   ? &x->v[i] : NULL ;
  }

  bool erase(const string& clave) { // marcado
    N* x = hoja_de(clave) ;
    int i = std::lower_bound( x->k, x->k + x->n, clave ) - x->k ;
    if ( ( i < x->n ) and ( x->k[i] == clave ) and ( not x->m[i] ) ) {
      x->m[i] = true ;
      return true ;
    }
    return false ;
  }

  long scan() { // recorrido secuencial por las hojas enlazadas
    long acc = 0 ;
    N* x = raiz ;
    while ( not x->hoja )
      x = x->h[0] ;
    for( ; x ; x = x->sig )
      for( int i = 0 ; i < x->n ; i++ )
	if ( not x->m[i] )
	  acc += x->v[i] ;
    return acc ;
  }

  long Nodos() { return nnodos ; }
};

#endif
