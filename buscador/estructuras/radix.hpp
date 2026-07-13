/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@   ESTRUCTURA: RADIX TRIE CON CAMINOS  COMPRIMIDOS    @@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#ifndef RADIX_HPP
#define RADIX_HPP

#include     <string>
#include    <cstddef>

using std::string;

/*/ nodo con arista comprimida /*/
struct NodoR
{
  string  trozo ;
  bool    tiene ;
  int     valor ;
  NodoR*   hijo ;
  NodoR* hermano;
};

class Radix
{
 private:
  /*/ elementos /*/
  NodoR* raiz ;

  /*/ operaciones /*/
  NodoR* baja(const string& k) {
    NodoR* p = raiz ;
    unsigned i = 0 ;
    while ( true ) {
      if ( i == k.size() )
	return p ;
      NodoR* q = p->hijo ;
      while ( q and ( q->trozo[0] != k[i] ) )
	q = q->hermano ;
      if ( ( not q ) or ( k.compare( i, q->trozo.size(), q->trozo ) != 0 ) )
	return NULL ;
      i += q->trozo.size() ;
      p = q ;
    }
  }

  void absorbe(NodoR* q) { // q sin valor y con hijo unico: comprimir arista
    NodoR* h = q->hijo ;
    q->trozo += h->trozo ;
    q->tiene  = h->tiene ;
    q->valor  = h->valor ;
    q->hijo   = h->hijo ;
    delete h ;
  }

  // borra k[i..] bajo p; hecho indica si la clave existia;
  // devuelve true si p ha quedado inutil y debe eliminarse
  bool borra(NodoR* p, const string& k, unsigned i, bool& hecho) {
    if ( i == k.size() ) {
      if ( not p->tiene )
	return false ;
      p->tiene = false ;
      hecho = true ;
    }
    else {
      NodoR** q = &p->hijo ;
      while ( *q and ( (*q)->trozo[0] != k[i] ) )
	q = &(*q)->hermano ;
      if ( ( not *q ) or ( k.compare( i, (*q)->trozo.size(), (*q)->trozo ) != 0 ) )
	return false ;
      if ( borra( *q, k, i + (*q)->trozo.size(), hecho ) ) {
	NodoR* m = *q ;
	*q = m->hermano ;
	delete m ;
      }
      else if ( hecho and ( not (*q)->tiene )
		and (*q)->hijo and ( not (*q)->hijo->hermano ) ) {
	absorbe(*q) ;
      }
    }
    return hecho and ( not p->tiene ) and ( p->hijo == NULL ) ;
  }

  void libera(NodoR* p) {
    if ( not p )
      return ;
    libera(p->hijo) ;
    libera(p->hermano) ;
    delete p ;
  }

 public:
  /*/ operaciones /*/
  Radix() {
    raiz = new NodoR{"", false, 0, NULL, NULL} ;
  }

  ~Radix() {
    libera(raiz) ;
  }

  bool insert(const string& k, int v) {
    NodoR* p = raiz ;
    unsigned i = 0 ;
    while ( true ) {
      if ( i == k.size() ) {
	if ( p->tiene )
	  return false ;
	p->tiene = true ;
	p->valor = v ;
	return true ;
      }
      NodoR* q = p->hijo ;
      while ( q and ( q->trozo[0] != k[i] ) )
	q = q->hermano ;
      if ( not q ) {
	p->hijo = new NodoR{k.substr(i), true, v, NULL, p->hijo} ;
	return true ;
      }
      unsigned j = 0 ;
      while ( ( j < q->trozo.size() ) and ( i+j < k.size() )
	      and ( q->trozo[j] == k[i+j] ) )
	j++ ;
      if ( j < q->trozo.size() ) { // divergencia a mitad de arista: partir
	q->hijo = new NodoR{q->trozo.substr(j), q->tiene, q->valor, q->hijo, NULL} ;
	q->trozo.resize(j) ;
	q->tiene = false ;
      }
      p = q ;
      i += j ;
    }
  }

  int* search(const string& k) {
    NodoR* p = baja(k) ;
    return ( p and p->tiene ) ? &p->valor : NULL ;
  }

  bool erase(const string& k) { // poda la rama y comprime aristas
    bool hecho = false ;
    borra( raiz, k, 0, hecho ) ; // la raiz no se elimina ni fusiona
    return hecho ;
  }

  long scan() { return -1 ; } // hermanos sin ordenar
};

#endif
