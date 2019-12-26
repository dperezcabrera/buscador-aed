/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@@@   MODULO DE LECTOR DE BUFFER DEFINICION   @@@@@@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#ifndef BUFFER_HPP
#define BUFFER_HPP

#include       <list>
#include    <sstream>
#include     <string>
#include     <cstring>

#include "screen.cpp"

using namespace std ;


void Lee_buffer
  (list<string>& lista_pal, std::istream& in,std::ostream& out) ;

#endif

/*
  $Id: buffer.hpp
  v 1.0 20/08/2004 11:51:30 
  David Perez Cabrera
*/
