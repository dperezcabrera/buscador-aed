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

  bool erase(const string& k) { // marcado, sin poda
    NodoT* p = raiz ;
    for( unsigned i = 0 ; ( i < k.size() ) and p ; i++ )
      p = baja( p, k[i], false ) ;
    if ( p and p->tiene ) {
      p->tiene = false ;
      return true ;
    }
    return false ;
  }

  long scan() { return -1 ; } // hermanos sin ordenar
};

#endif
