/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@@@@@@   MODULO DE CONTROL DE PANTALLA   @@@@@@@@@@@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#ifndef SCREEN_CPP
#define SCREEN_CPP

#include "screen.hpp"

/*/ limpiar pantalla /*/
void clear_screen() {
   char c = 27 ;
   cout << c << "[2J"   ;
   cout << c << "[1;1H" ;
}

/*/ ir a posicion /*/
void goto_xy(int i,int j) {

   char c = 27 ;
   cout << c << "["
        << i << ";" 
        << j << "H" ;
}

/*/ pintar en pantalla /*/
void paint(int k, int  l, char *pantalla[] ) {

   int i,j ;
   char c = 27 ;
   char aux ;
   clear_screen( );
   for ( i = 0 ; i < k ; i++ ) {
      for ( j = 0 ; j < l ; j++ ) {
	 cout << pantalla[i][j] ;
      }
      cout << endl ;
   }
}

#endif
/* 
  $Id: screen.cpp
  v 1.0 12/07/2004 10:51:30 
  David Perez Cabrera
*/
