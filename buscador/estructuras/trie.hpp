/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@   ESTRUCTURA: TRIE PRIMER-HIJO / SIGUIENTE-HERMANO   @@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#ifndef TRIE_HPP
#define TRIE_HPP

#include     <string>
#include    <cstddef>

using std::string;

/*/ nodo del trie /*/
struct NodoT
{
  char        c ;
  bool    tiene ;
  int     valor ;
  NodoT*   hijo ;
  NodoT* hermano;
};

class Trie
{
 private:
  /*/ elementos /*/
  NodoT* raiz ;

  /*/ operaciones /*/
  NodoT* baja(NodoT* p, char c, bool crear) {
    NodoT** q = &p->hijo ;
    for( ; *q ; q = &(*q)->hermano )
      if ( (*q)->c == c )
	return *q ;
    if ( not crear )
      return NULL ;
    return *q = new NodoT{c,false,0,NULL,NULL} ;
  }

  // borra k[i..] bajo p; hecho indica si la clave existia;
  // devuelve true si p ha quedado inutil y debe eliminarse
  bool borra(NodoT* p, const string& k, unsigned i, bool& hecho) {
    if ( i == k.size() ) {
      if ( not p->tiene )
	return false ;
      p->tiene = false ;
      hecho = true ;
    }
    else {
      NodoT** q = &p->hijo ;
      while ( *q and ( (*q)->c != k[i] ) )
	q = &(*q)->hermano ;
      if ( not *q )
	return false ;
      if ( borra(*q, k, i+1, hecho) ) {
	NodoT* m = *q ;
	*q = m->hermano ;
	delete m ;
      }
    }
    return hecho and ( p->hijo == NULL ) and ( not p->tiene ) ;
  }

 public:
  /*/ operaciones /*/
  Trie() {
    raiz = new NodoT{0,false,0,NULL,NULL} ;
  }

  bool insert(const string& k, int v) {
    NodoT* p = raiz ;
    for( unsigned i = 0 ; i < k.size() ; i++ )
      p = baja( p, k[i], true ) ;
    if ( p->tiene )
      return false ;
    p->tiene = true ;
    p->valor = v ;
    return true ;
  }

  int* search(const string& k) {
    NodoT* p = raiz ;
    for( unsigned i = 0 ; ( i < k.size() ) and p ; i++ )
      p = baja( p, k[i], false ) ;
    return ( p and p->tiene ) ? &p->valor : NULL ;
  }

  bool erase(const string& k) { // con poda de la rama muerta
    bool hecho = false ;
    borra( raiz, k, 0, hecho ) ; // la raiz no se elimina nunca
    return hecho ;
  }

  long scan() { return -1 ; } // hermanos sin ordenar
};

#endif
