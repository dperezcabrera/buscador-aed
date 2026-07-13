/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@   ESTRUCTURA: FILTRO BLOOM + HASH CLASICO DETRAS   @@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

// no compite: complementa — mata los miss sin tocar la tabla

#ifndef BLOOM_HASH_HPP
#define BLOOM_HASH_HPP

#include     <string>
#include    <cstddef>

#include "hash_clasico.hpp"

using std::string;

class BloomHash
{
 private:
  /*/ elementos /*/
  ClassicHash          t ;
  unsigned long*    bits ;
  static const unsigned M = 1u<<24 ; // 16M bits = 2 MB, 16 bits/clave

  /*/ operaciones /*/
  static unsigned fnv(const string& k) {
    unsigned x = 2166136261u ;
    for( unsigned i = 0 ; i < k.size() ; i++ ) {
      x ^= (unsigned char) k[i] ;
      x *= 16777619u ;
    }
    return x ;
  }

  void marca(const string& k) {
    unsigned h1 = (unsigned) f_hash(k), h2 = fnv(k) | 1 ;
    for( int i = 0 ; i < 8 ; i++ ) {
      unsigned b = (h1 + i*h2) & (M-1) ;
      bits[b>>6] |= 1ul << (b&63) ;
    }
  }

  bool quiza(const string& k) {
    unsigned h1 = (unsigned) f_hash(k), h2 = fnv(k) | 1 ;
    for( int i = 0 ; i < 8 ; i++ ) {
      unsigned b = (h1 + i*h2) & (M-1) ;
      if ( not ( bits[b>>6] >> (b&63) & 1 ) )
	return false ;
    }
    return true ;
  }

 public:
  /*/ operaciones /*/
  BloomHash(int cubetas) : t(cubetas) {
    bits = new unsigned long[M/64]() ;
  }

  bool insert(const string& k,int v) { marca(k) ; return t.insert(k,v) ; }
  int* search(const string& k)       { return quiza(k) ? t.search(k) : NULL ; }
  bool erase (const string& k)       { return t.erase(k) ; } // el filtro queda sucio: correcto
  long scan  ()                      { return -1 ; }
};

#endif
