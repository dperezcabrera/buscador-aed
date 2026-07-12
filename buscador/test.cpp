/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@@@@@@@   AUTOCOMPROBACION DEL BUSCADOR   @@@@@@@@@@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

// compilar y ejecutar con: make test

#include "estructura_datos.cpp"
#include "buffer.cpp"

#include <cassert>

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

  cout << "OK: todas las comprobaciones pasan" << endl ;
  return 0 ;
}
