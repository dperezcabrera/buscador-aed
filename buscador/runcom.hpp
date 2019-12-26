/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@@@@@@@@   MODULO DE RUNCOM DEFINICION   @@@@@@@@@@@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#ifndef RUNCOM_HPP
#define RUNCOM_HPP

#include                  <sstream>

#include     "estructura_datos.cpp"
#include "funciones_auxiliares.cpp"
#include               "screen.cpp"


#define OTRA             10000
#define LOAD             10001
#define INSERT           10002
#define REMOVE           10003
#define SAVE             10004
#define SEARCH           10005
#define HELP             10006
#define QUIT             10007
#define CLEAR            10009
#define op_and             103
#define op_or              102
#define op_com             104
#define op_not             105
#define abre_parentesis    106 
#define cierra_parentesis  107


void runcomand
     (Contenedor& T,list<string>& maestro ,list<string>& linea_comandos,
      bool& salir,std::istream& in,std::ostream& out) ;

#endif
/* 
  $Id: runcom.hpp
  v 1.0 28/07/2004 18:51:30 
  David Perez Cabrera
*/
