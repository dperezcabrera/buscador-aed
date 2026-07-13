/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@@@@@@@@@   ESTRUCTURA: ARBOL B (STAR)  @@@@@@@@@@@@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

// redistribuye con los hermanos antes de partir (split 2->3
// canonico omitido: la redistribucion ya logra el relleno
// alto; con ambos hermanos llenos parte 1->2 como B)

#ifndef ARBOL_B_ESTRELLA_HPP
#define ARBOL_B_ESTRELLA_HPP

#include "arbol_b.hpp"

class BStar : public BTree
{
 private:
  /*/ operaciones /*/
  void rota_izq(NodoB* x, int i) { // pasa 1 clave del hijo i al hermano i-1
    NodoB *izq = x->hijo[i-1], *c = x->hijo[i] ;
    izq->claves[izq->n] = std::move(x->claves[i-1]) ;
    izq->val[izq->n]    = x->val[i-1] ;
    izq->muerto[izq->n] = x->muerto[i-1] ;
    if ( not izq->hoja )
      izq->hijo[izq->n+1] = c->hijo[0] ;
    izq->n++ ;
    x->claves[i-1] = std::move(c->claves[0]) ;
    x->val[i-1]    = c->val[0] ;
    x->muerto[i-1] = c->muerto[0] ;
    for( int j = 0 ; j < c->n-1 ; j++ ) {
      c->claves[j] = std::move(c->claves[j+1]) ;
      c->val[j]    = c->val[j+1] ;
      c->muerto[j] = c->muerto[j+1] ;
    }
    if ( not c->hoja )
      for( int j = 0 ; j < c->n ; j++ )
	c->hijo[j] = c->hijo[j+1] ;
    c->n-- ;
  }

  void rota_der(NodoB* x, int i) { // pasa 1 clave del hijo i al hermano i+1
    NodoB *der = x->hijo[i+1], *c = x->hijo[i] ;
    for( int j = der->n-1 ; j >= 0 ; j-- ) {
      der->claves[j+1] = std::move(der->claves[j]) ;
      der->val[j+1]    = der->val[j] ;
      der->muerto[j+1] = der->muerto[j] ;
    }
    if ( not der->hoja )
      for( int j = der->n ; j >= 0 ; j-- )
	der->hijo[j+1] = der->hijo[j] ;
    der->claves[0] = std::move(x->claves[i]) ;
    der->val[0]    = x->val[i] ;
    der->muerto[0] = x->muerto[i] ;
    if ( not der->hoja )
      der->hijo[0] = c->hijo[c->n] ;
    der->n++ ;
    x->claves[i] = std::move(c->claves[c->n-1]) ;
    x->val[i]    = c->val[c->n-1] ;
    x->muerto[i] = c->muerto[c->n-1] ;
    c->n-- ;
  }

 protected:
  /*/ operaciones /*/
  void lleno_gestionado(NodoB* x, int i) {
    if ( ( i > 0 ) and ( x->hijo[i-1]->n < 2*G-2 ) ) {
      rota_izq(x,i) ;
      return ;
    }
    if ( ( i < x->n ) and ( x->hijo[i+1]->n < 2*G-2 ) ) {
      rota_der(x,i) ;
      return ;
    }
    split(x,i) ;
  }
};

#endif
