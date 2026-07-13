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

 public:
  /*/ operaciones /*/
  Radix() {
    raiz = new NodoR{"", false, 0, NULL, NULL} ;
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

  bool erase(const string& k) { // marcado, sin fusionar aristas
    NodoR* p = baja(k) ;
    if ( p and p->tiene ) {
      p->tiene = false ;
      return true ;
    }
    return false ;
  }

  long scan() { return -1 ; } // hermanos sin ordenar
};

#endif
