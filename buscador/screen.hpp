/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@   MODULO DE CONTROL DE PANTALLA IMPLEMENTACION   @@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#ifndef SCREEN_HPP
#define SCREEN_HPP

#include   <iostream>

using namespace std ;

/*/ definimos espacio /*/
#ifndef space
#define space      ' '
#endif

/*/ definimos volver un espacio /*/
#ifndef backspace
#define backspace '\b'
#endif

/*/ definimos la alarma /*/
#ifndef alert
#define alert     '\a'
#endif

/*/ definimos inicio linea /*/
#ifndef backline 
#define backline  '\r'
#endif

/*/ definimos tabulador /*/
#ifndef tab
#define tab       '\t'
#endif

/*/ definimos nueva linea /*/
#ifndef endl
#define endl      '\n'
#endif

/*/ definimos fin string /*/
#ifndef endstrg
#define endstrg   '\0'
#endif

void clear_screen() ;
void goto_xy(int i,int j) ;
void paint(int k, int  l, char *pantalla[] ) ;

#endif
/* 
  $Id: screen.hpp
  v 1.0 12/07/2004 10:25:30 
  David Perez Cabrera
*/
