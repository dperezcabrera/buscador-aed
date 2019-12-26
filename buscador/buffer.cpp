/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@   MODULO DE LECTOR DE  BUFFER IMPLEMENTACION   @@@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#ifndef BUFFER_CPP
#define BUFFER_CPP


#include "buffer.hpp" 


/*/ cuenta el numero de caracteres c en la cadena p /*/
int cuenta(const char *p,const char c) {
   
   int   res =  0 ;
   int    i  =  0 ;
   while( p[i] != endstrg ) {
     if ( p[i] == c )
       res++;
     i++;
   }
   return res;
}

/*/ comprueba si el caracter es especial /*/
// abrir o cerrar parentesis o las comillas
// se sustitye la comilla simple por la doble

bool is_especial(char& c) {
  
   switch (c) {
      case '\'':{ c='"'; return true; break; }
      case ')' :
      case '(' :
      case '"' :{ return  true; break; }
      default  :{ return false; break; }
   }
}

/*/ limpia una linea pasada como string /*/
// se le añaden huecos en blanco antes y despues
// de los caracteres especiales  para evitar que 
// formen parte de una palabra mayor que no existe

char* limpiar_linea(string Linea) {
   
   char *linea = new char [Linea.length()+1];
   char  blanco=' ' ;
   char *linea2;
   strcpy(linea,Linea.c_str());
   int maximo=strlen(linea)+1, j=0, i=0;

   maximo += (2 * cuenta(linea,'(' ) ) ;
   maximo += (2 * cuenta(linea,')' ) ) ;
   maximo += (2 * cuenta(linea,'\'') ) ;
   maximo += (2 * cuenta(linea,'"' ) ) ;

   linea2=new char[maximo];
   
   while (linea[i] != endstrg ) {
      if ( is_especial(linea[i]) ) {
	 linea2[j]   =    space ;
	 linea2[j+1] = linea[i] ;
	 linea2[j+2] =    space ;
	 j+=3;
      }
      else { 
	linea2[j] = linea[i] ; 
	j++ ; 
      }
      i++;
   }
   linea2[j] = endstrg ;
   return linea2 ;
}

/*/ trasforma un string en una lista de palabras /*/
// usa el espacio como separador, se sustituye ".." por ( COM ..)
// y '..' por ( COM .. ), ya que las comillas simples no existen
// puesto que aparecen como comillas dobles

void crear_lista(string linea,list<string>& lista) {
   
   bool    is_open_comillas = false ;
   list<string>      lista_auxiliar ;
   istringstream leer_string(linea) ;
   string palabra;

   lista.clear();
   lista_auxiliar.clear();
   while ( leer_string >> palabra ) {
      lista_auxiliar.push_back( palabra ) ;
   }

   list<string>::iterator j = lista_auxiliar.begin() ;
   list<string>::iterator end = lista_auxiliar.end() ;
   int primera_palabra ;
   while ( j != end ) {
     if ( (*j) != "\"" ) {
       lista.push_back(*j) ;
       //cout << "[" << (*j) <<"]" << endl;
     }
     else {  
       if ( not is_open_comillas ) {
	 lista.push_back("(")   ;
	 lista.push_back("COM") ;
	 is_open_comillas = true; 
       }
       else {
	 lista.push_back(")") ;
	 is_open_comillas = false ;
       }
     }
     j++ ;
   }
}

/*/ mensaje para leer nueva orden /*/
// lo hemos hecho en otra funcion para 
// poder modificarlo con facilidad

void prompt(std::ostream& out){

  out  <<  ">> " ;
  out.flush();
}

/*/ lector de buffer /*/
// lee del buffer mientrar que la linea leida sea vacia

void Lee_buffer(list<string>& lista_pal, std::istream& in,std::ostream& out) {
   
   string linea2 ;
   bool salir = false ;
   string       linea ;
   while ( not salir ) {  
     prompt(out) ;
     while ( linea.size() == 0 ) { 
       in.clear() ;
       getline( in, linea ) ;
       if (linea.size() == 0 ) {
	 out << endl ;
	 prompt(out) ;
       }
     }
     linea2 = linea ;
     linea = limpiar_linea( linea ) ;
     crear_lista( linea, lista_pal) ;
     if ( lista_pal.size() > 0 )
       salir = true ;
   }
   lista_pal.push_back(linea2);
}

#endif

/* 
  $Id: buffer.cpp
  v 1.0 20/08/2004 11:51:30 
  David Perez Cabrera
*/
