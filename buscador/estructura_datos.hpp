/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@   MODULO DE ESTRUCTURA  DE DATOS DEFINICION   @@@@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#ifndef  ESTRUCTURA_DATOS_HPP
#define  ESTRUCTURA_DATOS_HPP

#include           <list>
#include      <algorithm>
#include       <string.h>
#include        <cstdlib>
#include        <fstream>
#include       <iostream>
#include         <string>

#include     "screen.cpp"
#include "close_hash.cpp"
#include "funciones_auxiliares.cpp"

#define size_palabras  500000
#define size_paginas    20000

using namespace std;

int f_hash(string) ;
typedef void(*CargaDatos)
  (string&,string&,int&,std::ifstream&,std::ifstream&,string&);

/*##########################################################*/
/*################   DEFINICION DE PAGINA   ################*/
/*##########################################################*/

class Pagina
{
 public:
   
  /*/ elementos /*/
  string URL ;
  string nom_fich ;
  int relevancia ; 
  int aparicion ;
  
  /*/ operaciones /*/
  Pagina()                                  ; // &
  Pagina(string&,string&,int&,int&)         ; // &
  int SizeofP(Pagina& p)                    ; // &
  bool friend operator< (Pagina p,Pagina q) ; // &     
  bool friend operator> (Pagina p,Pagina q) ; // &
  bool friend operator<=(Pagina p,Pagina q) ; // &
  bool friend operator>=(Pagina p,Pagina q) ; // &
  bool friend operator==(Pagina p,Pagina q) ; // &
  bool friend operator!=(Pagina p,Pagina q) ; // &
  /*/ operador asignador de pagina /*/
  Pagina operator =(Pagina p) {
     URL = p.URL ;
     nom_fich   = p.nom_fich   ;
     relevancia = p.relevancia ;
     aparicion  = p.aparicion  ;
     return *this ;
  } 
};

int f_hash2  (Pagina* pag ) ;

/*##########################################################*/
/*#################   DEFINICION DE NODO   #################*/
/*##########################################################*/

class Nodo // & completo
{
 public:
   
  /*/ elementos /*/
  list<int> l_aparic;
  Pagina *Pag;
  
  /*/ operaciones /*/
  Nodo(Pagina*,int&)                    ; // &
  Nodo(Pagina*)                         ; // &
  Nodo()                                ; // &
  ~Nodo()                               ; // &
  bool friend operator==(Nodo p,Nodo q) ; // &
  bool friend operator< (Nodo p,Nodo q) ; // &
  bool friend operator> (Nodo p,Nodo q) ; // & 
  bool friend operator<=(Nodo p,Nodo q) ; // &   
  bool friend operator>=(Nodo p,Nodo q) ; // &
  bool friend operator!=(Nodo p,Nodo q) ; // &
  int  SizeofN(Nodo&)                   ; // &
  int  Relevancia()               ; // &
  int  Aparicion()                ; // &
  void Modif_pag(Pagina *pag)     ; // &
  string  Fichero()               ; // &
  Pagina *Pagin()                 ; // &
  string  Url()                   ; // &
  /*/ operador asignador de Nodo /*/
  Nodo& operator =(Nodo& n) {
     Pag = n.Pag ;
     list<int>::iterator i = n.l_aparic.begin() ;
     list<int>::iterator end = n.l_aparic.end() ;
     if (n.l_aparic.size() > 0 ) {
	l_aparic.clear() ;
	while( i != end ) {
	   l_aparic.push_back(*i) ;
	   i++ ;
	}	
     }
     return *this;
  }
 
};

/*##########################################################*/
/*#############   DEFINICION DE LISTA_DATOS   ##############*/
/*##########################################################*/


class Lista_datos // &
{
 public:
  /*/ elementos /*/
  bool permitir_com; // para saber si se puede aplicar la operacion com
  bool error;        // para saber si ha habido algun error
  list<Nodo> list_nodo;
  
  /*/ operaciones /*/
   Lista_datos()                     ; // &
   Lista_datos(Pagina *pag, int& pos); // &
  ~Lista_datos()                     ; // &
  Lista_datos  AND(Lista_datos&,Lista_datos&)     ; // &
  Lista_datos   OR(Lista_datos&,Lista_datos&)     ; // &
  Lista_datos  XOR(Lista_datos&,Lista_datos&)     ; // &
  Lista_datos  COM(Lista_datos&,Lista_datos&)     ; // &
  Lista_datos  OPN(Lista_datos&,Lista_datos&)     ; // &
  int  SizeofL(Lista_datos&)                      ; // &
  void Show(std::ostream&)           ; // &
  Nodo Front()                       ; // &
  void Sort()                        ; // &
  void Clear()                       ; // &
  int  Size()                        ; // &
  /*/ operador asignador de Lista_datos /*/
  Lista_datos& operator = (Lista_datos fuente) {
     permitir_com = fuente.permitir_com ;
     error        =        fuente.error ;
     if ( not error ) {
	list_nodo.clear();
	list<Nodo>::iterator it=fuente.list_nodo.begin();
	list<Nodo>::iterator end= fuente.list_nodo.end();
	while ( it != end ) {
	   list_nodo.push_back(*it) ;
	   it++ ;
	}
     }  
     return (*this) ;
  }

};

/*##########################################################*/
/*################   DEFINICION DE TABLA   #################*/
/*##########################################################*/


class Contenedor
{
 private:
  /*/ elementos /*/
  int  read_word ;
  int  word_dist ;
  int web_leidas ;
  bool   cargado ;
  close_hash<list<string>,Pagina*>    tabla_pag ;
  close_hash<Lista_datos,string> tabla_palabras ;
  Lista_datos                    list_dat_total ;

  /*/ operaciones /*/
  void inserta_pagina (string&,Pagina*)     ; // &
  void inserta_palabra(string&,Pagina*,int) ; // &
  void inserta_nueva  (string,string,int)   ; // &
  void borrar_pagina  (Pagina*)             ; // &
  void mostrar_datos  (cronometro,int)      ; // &
  int  Sizeof_and_sort( )                   ; // &
  Lista_datos Lista_total_paginas( )        ; // &
  Lista_datos Search( string& )             ; // &

public:
  /*/ operaciones /*/
  Contenedor() :tabla_pag(size_paginas,f_hash2), 
		tabla_palabras(size_palabras,f_hash),
		read_word(0), web_leidas(0), 
                cargado(false),word_dist(0){}   ; // &
  Contenedor(int num_pag,int num_pal) 
               :tabla_pag(num_pag,f_hash2), 
		tabla_palabras(num_pal,f_hash),
		cargado(false) ,word_dist(0),
		read_word(0), web_leidas(0) {}   ; // &
  ~Contenedor()                                  ; // &
  void Init  ( )                     ; // &
  void Init  (int,int)               ; // &
  void Clear ( )                     ; // &
  int  Size  ( )                     ; // &
  bool Erase (string&)               ; // &
  bool Load  (string&,list<string>&) ; // &
  bool Insert(string,string,int)     ; // &
  void Or (list<string>& lista,std::ostream& out); // &
  void Not(list<string>& lista,std::ostream& out); // &
  void Com(list<string>& lista,std::ostream& out); // &
  void And(list<string>& lista,std::ostream& out); // &

};

#endif
/*
  $Id: estructura_datos.cpp
  v 1.0 11/08/2004 10:59:10
  David Perez Cabrera
*/
