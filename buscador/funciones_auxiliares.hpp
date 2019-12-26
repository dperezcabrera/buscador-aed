/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@  MODULO AUXILIAR DE FUNCIONES  PARA TRATAR PALABRAS  @@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#ifndef FUNCIONES_AUXILIARES_HPP
#define FUNCIONES_AUXILIARES_HPP

#include        <ctype.h>
#include        <stdio.h>
#include       <string.h>
#include       <iostream>
#include        <fstream>
#include        <sstream>
#include        <cstdlib>
#include           <list>
#include     <sys/time.h>
#include     "screen.cpp"
using namespace std;

class cronometro
{
private:
  struct timeval ti,tf ;
  //time_t ti,tf ;
  bool iniciado,ver;

 public:
  cronometro():iniciado(false),ver(false){}
  void iniciar();
  void parar();
  long milisegundos ();
  void mostrar();
};

void str_minusculas  (string& palabra)     ; // &
void  trasforma_letra(char &letra)         ; // &
bool  aceptar_letra  (char &c)             ; // &
char* mayusculas     (const char *palabra) ; // &
char* minusculas     (const char *palabra) ; // &

#endif
/* 
  $Id: funciones_auxiliares.hpp
  v 1.0 24/08/2004 11:51:30 
  David Perez Cabrera
*/
