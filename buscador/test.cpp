/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@@@@@@@   AUTOCOMPROBACION DEL BUSCADOR   @@@@@@@@@@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

// compilar y ejecutar con: make test

#include "estructura_datos.cpp"
#include "buffer.cpp"

#include "estructuras/hash_clasico.hpp"
#include "estructuras/flat_hash.hpp"
#include "estructuras/bloom_hash.hpp"
#include "estructuras/trie.hpp"
#include "estructuras/radix.hpp"
#include "estructuras/avl.hpp"
#include "estructuras/arbol_b.hpp"
#include "estructuras/arbol_b_estrella.hpp"
#include "estructuras/arbol_b_mas.hpp"

#include <cassert>

/*/ tortura de borrado: inserta n, borra pares, verifica,  /*/
/*/ borra impares, verifica vacio, reinserta               /*/
template<class Tabla>
void tortura(Tabla* t, int n) {

  for( int i = 0 ; i < n ; i++ )
    assert( t->insert( "clave" + std::to_string(i), i ) ) ;
  for( int i = 0 ; i < n ; i += 2 )
    assert( t->erase( "clave" + std::to_string(i) ) ) ;
  for( int i = 0 ; i < n ; i++ ) {
    int* v = t->search( "clave" + std::to_string(i) ) ;
    if ( i % 2 )
      assert( v and ( *v == i ) ) ;
    else
      assert( v == NULL ) ;
  }
  assert( not t->erase("clave0") ) ; // doble borrado
  for( int i = 1 ; i < n ; i += 2 )
    assert( t->erase( "clave" + std::to_string(i) ) ) ;
  for( int i = 0 ; i < n ; i += n/50 )
    assert( t->search( "clave" + std::to_string(i) ) == NULL ) ;
  long s = t->scan() ;
  assert( ( s == 0 ) or ( s == -1 ) ) ;  // vacia (o sin orden)
  assert( t->insert( "clave1", 111 ) ) ; // reinsertar tras vaciar
  assert( *t->search("clave1") == 111 ) ;
  delete t ;
}

int main() {

  /*/ close_hash: insertar, buscar, borrar /*/
  close_hash<int,string> h(101,f_hash) ;
  h.insert(string("hola") ,1) ;
  h.insert(string("adios"),2) ;
  assert( *h.search(string("hola"))  == 1 ) ;
  assert( *h.search(string("adios")) == 2 ) ;
  assert(  h.search(string("nada"))  == NULL ) ;
  assert(  h.erase (string("hola")) ) ;
  assert( not h.erase(string("hola"))  ) ; // borrar dos veces
  assert( not h.erase(string("jamas")) ) ; // clave de bucket vacio
  assert(  h.search(string("hola"))  == NULL ) ;
  assert( *h.search(string("adios")) == 2 ) ;

  /*/ crecimiento automatico: resize al llenarse /*/
  close_hash<int,string> chica(4,f_hash) ;
  for ( int n = 0 ; n < 20 ; n++ ) {
    ostringstream clave ;
    clave << "clave" << n ;
    chica.insert( clave.str() , n ) ;
  }
  assert( chica.size() == 20 ) ; // el contador sobrevive al resize
  for ( int n = 0 ; n < 20 ; n++ ) {
    ostringstream clave ;
    clave << "clave" << n ;
    assert( *chica.search( clave.str() ) == n ) ;
  }
  assert( not chica.insert( string("clave7") , 99 ) ) ; // duplicado
  assert( chica.size() == 20 ) ;

  /*/ minusculas: quita tildes latin-1 y baja a minuscula /*/
  assert( minusculas("HoLa")       == "hola"   ) ;
  assert( minusculas("cami\xf3n")  == "camion" ) ; // camión
  assert( minusculas("CAMI\xd3N")  == "camion" ) ; // CAMIÓN

  /*/ limpiar_html: etiquetas fuera, entidades decodificadas /*/
  istringstream html("<p>cami&oacute;n <script>var x=1;</script>"
		     "<style>.rojo{}</style>ROJO</p> 42") ;
  istringstream texto( limpiar_html(html) ) ;
  assert( lee(texto) == "camion" ) ; // entidad + tilde plegada
  assert( lee(texto) == "rojo"   ) ; // script y style no se indexan
  assert( lee(texto) == SALIR    ) ; // "42" no es palabra

  /*/ tokenizador: comillas se traducen a ( COM ... ) /*/
  list<string> tokens ;
  crear_lista( limpiar_linea("busca 'foo bar'"), tokens ) ;
  string plano ;
  list<string>::iterator i = tokens.begin() ;
  while ( i != tokens.end() ) {
    plano += (*i) + " " ;
    i++ ;
  }
  assert( plano == "busca ( COM foo bar ) " ) ;

  /*/ borrado completo en las estructuras del benchmark /*/
  const int N = 20000 ;
  tortura( new ClassicHash(4096), N ) ;
  tortura( new FlatHash(1<<16),   N ) ;
  tortura( new BloomHash(4096),   N ) ;
  tortura( new Trie(),            N ) ;
  tortura( new Radix(),           N ) ;
  tortura( new AVL(),             N ) ;
  tortura( new BTree(),           N ) ;
  tortura( new BStar(),           N ) ;
  tortura( new BPlus(),           N ) ;

  cout << "OK: todas las comprobaciones pasan" << endl ;
  return 0 ;
}
