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
#include    <utility>
#include    <cstddef>

using std::string;

int f_hash(const string&) ; // definida en estructura_datos.cpp

class FlatHash
{
 private:
  /*/ elementos /*/
  int          tama ; // potencia de 2
  long        vivos ;
  long       vacios ;
  string*    claves ;
  int*          val ;
  unsigned*  hcache ;
  char*      estado ; // 0 libre, 1 ocupado, 2 vacio (borrado)

  /*/ operaciones /*/
  void reserva(int t) {
    tama   = t ;
    claves = new string[t]   ;
    val    = new int[t]      ;
    hcache = new unsigned[t] ;
    estado = new char[t]()   ;
  }

  void crece() { // duplica y recoloca los ocupados: purga los vacios
    int       viejo_tama = tama   ;
    string*   vc = claves ;
    int*      vv = val    ;
    unsigned* vh = hcache ;
    char*     ve = estado ;
    reserva( tama * 2 ) ;
    vacios = 0 ;
    for( int i = 0 ; i < viejo_tama ; i++ ) {
      if ( ve[i] != 1 )
	continue ;
      for( unsigned j = vh[i] & (tama-1) ; ; j = (j+1) & (tama-1) ) {
	if ( estado[j] == 0 ) {
	  claves[j] = std::move(vc[i]) ;
	  val[j]    = vv[i] ;
	  hcache[j] = vh[i] ;
	  estado[j] = 1 ;
	  break ;
	}
      }
    }
    delete[] vc ;
    delete[] vv ;
    delete[] vh ;
    delete[] ve ;
  }

 public:
  /*/ operaciones /*/
  FlatHash(int t) : vivos(0), vacios(0) {
    reserva(t) ;
  }

  ~FlatHash() {
    delete[] claves ;
    delete[] val    ;
    delete[] hcache ;
    delete[] estado ;
  }

  bool insert(const string& k,int v) {
    if ( ( vivos + vacios + 1 ) * 4 >= (long) tama * 3 )
      crece() ; // mantener la ocupacion bajo 3/4
    unsigned hh = (unsigned) f_hash(k) ;
    int vacio = -1 ;
    for( unsigned i = hh & (tama-1) ; ; i = (i+1) & (tama-1) ) {
      if ( estado[i] == 0 ) {
	int d = vacio >= 0 ? vacio : (int) i ;
	claves[d] = k  ;
	val[d]    = v  ;
	hcache[d] = hh ;
	estado[d] = 1  ;
	vivos++ ;
	if ( d == vacio )
	  vacios-- ;
	return true ;
      }
      if ( estado[i] == 2 ) {
	if ( vacio < 0 )
	  vacio = (int) i ;
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

  bool erase(const string& k) { // marca la celda: lo canonico en abierto
    unsigned hh = (unsigned) f_hash(k) ;
    for( unsigned i = hh & (tama-1) ; ; i = (i+1) & (tama-1) ) {
      if ( estado[i] == 0 )
	return false ;
      if ( ( estado[i] == 1 ) and ( hcache[i] == hh ) and ( claves[i] == k ) ) {
	estado[i] = 2 ;
	claves[i] = string() ;
	vivos-- ;
	vacios++ ;
	return true ;
      }
    }
  }

  long scan() { return -1 ; } // sin orden
};

#endif
