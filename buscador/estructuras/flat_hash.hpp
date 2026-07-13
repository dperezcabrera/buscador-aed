/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@@@@@@@   ESTRUCTURA:  FLAT  HASH    @@@@@@@@@@@@@@@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

// direccionamiento abierto, tambien llamado hash cerrado:
// los elementos viven dentro del array (cero punteros) y la
// colision se resuelve sondeando la celda siguiente; el hash
// cacheado evita comparar strings en los sondeos fallidos

#ifndef FLAT_HASH_HPP
#define FLAT_HASH_HPP

#include     <string>
#include    <cstddef>

using std::string;

int f_hash(const string&) ; // definida en estructura_datos.cpp

class FlatHash
{
 private:
  /*/ elementos /*/
  int          tama ; // potencia de 2
  string*    claves ;
  int*          val ;
  unsigned*  hcache ;
  char*      estado ; // 0 vacio, 1 ocupado, 2 tumba

 public:
  /*/ operaciones /*/
  FlatHash(int t) : tama(t) {
    claves = new string[t]   ;
    val    = new int[t]      ;
    hcache = new unsigned[t] ;
    estado = new char[t]()   ;
  }

  bool insert(const string& k,int v) {
    unsigned hh = (unsigned) f_hash(k) ;
    int tumba = -1 ;
    for( unsigned i = hh & (tama-1) ; ; i = (i+1) & (tama-1) ) {
      if ( estado[i] == 0 ) {
	int d = tumba >= 0 ? tumba : (int) i ;
	claves[d] = k  ;
	val[d]    = v  ;
	hcache[d] = hh ;
	estado[d] = 1  ;
	return true ;
      }
      if ( estado[i] == 2 ) {
	if ( tumba < 0 )
	  tumba = (int) i ;
      }
      else if ( ( hcache[i] == hh ) and ( claves[i] == k ) )
	return false ;
    }
  }

  int* search(const string& k) {
    unsigned hh = (unsigned) f_hash(k) ;
    for( unsigned i = hh & (tama-1) ; ; i = (i+1) & (tama-1) ) {
      if ( estado[i] == 0 )
	return NULL ;
      if ( ( estado[i] == 1 ) and ( hcache[i] == hh ) and ( claves[i] == k ) )
	return &val[i] ;
    }
  }

  bool erase(const string& k) { // tumba: el borrado canonico en abierto
    unsigned hh = (unsigned) f_hash(k) ;
    for( unsigned i = hh & (tama-1) ; ; i = (i+1) & (tama-1) ) {
      if ( estado[i] == 0 )
	return false ;
      if ( ( estado[i] == 1 ) and ( hcache[i] == hh ) and ( claves[i] == k ) ) {
	estado[i] = 2 ;
	claves[i] = string() ;
	return true ;
      }
    }
  }

  long scan() { return -1 ; } // sin orden
};

#endif
