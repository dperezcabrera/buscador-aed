/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@  MODULO AUXILIAR DE FUNCIONES  PARA TRATAR PALABRAS  @@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#ifndef FUNCIONES_AUXILIARES_CPP
#define FUNCIONES_AUXILIARES_CPP

#include "funciones_auxiliares.hpp"


#define LETRA_MENOR  65
#define LETRA_MAYOR  90

/*/ quita tildes y pasa a minusculas /*/
void trasforma_letra(char &letra) {

  switch (letra)
    {
    case 'á':
    case 'à':
    case 'ä':
    case 'â':
    case 'Á':
    case 'À':
    case 'Ä':
    case 'Â': { letra = 'a' ; break ;}    
    case 'é':
    case 'è':
    case 'ë':
    case 'ê':
    case 'É':
    case 'È':
    case 'Ë':
    case 'E':
    case 'Ê': { letra = 'e' ; break ;}
    case 'í':
    case 'ì':
    case 'ï':
    case 'î':
    case 'I':
    case 'Í':
    case 'Ì':
    case 'Ï':
    case 'Î': { letra = 'i' ; break ;} 
    case 'ó':
    case 'ò':
    case 'ö':
    case 'ô':
    case 'O':
    case 'Ó':
    case 'Ò':
    case 'Ö':
    case 'Ô': { letra = 'o' ; break ;}
    case 'ú':
    case 'ù':
    case 'ü':
    case 'û':
    case 'U':
    case 'Ú':
    case 'Ù':
    case 'Ü':
    case 'Û': { letra = 'u' ; break ;}
    }
  if (( letra >= LETRA_MENOR ) and ( letra <= LETRA_MAYOR )or ( letra == 'Ñ' ) ) { 
    letra += 32 ;
  }
}

/*/ aceptar las letras y trasformarlas /*/
// para transformar la letra y ver si es un caracter alfabetico

bool aceptar_letra(char &c) {
// si dos palabras estan separadas por comas, puntos,etc........

  trasforma_letra( c ) ;
  return ( isalpha( c ) or (c == 'ñ') ) ;
}

/*/ convierte un string a minusculas /*/
void str_minusculas(string& palabra){

  char c ;
  int i,leng = palabra.length() ;
  char *pal = new char [leng+1] ;

  for ( i = 0 ; i < leng ; i++ ) {
    c = palabra[i];
    trasforma_letra( c );
    pal[i] =  c ;
  }
  pal[leng] = endstrg ;
}

/*/ convierte una cadena a minusculas /*/ 
char* minusculas(const char *palabra) {
    
   char *res = new char [ strlen(palabra)+1 ] ;
   int longitud = strlen(palabra);
   for( int i = 0 ; i < longitud ; i++ ) {
     char c = palabra[ i ] ;
     trasforma_letra ( c ) ;
     res[i] = c ;
   }
   res[longitud] = endstrg ;
   return res ;
}

/*/ convierte una cadena a mayusculas /*/
char* mayusculas(const char *palabra) {
    
   char *res = new char [ strlen(palabra)+1 ] ;
   int longitud = strlen(palabra);
	for( int i = 0 ; i < longitud ; i++ ) {
	   res[i] = toupper( palabra[i] ) ;
	}
   res[longitud] = endstrg ;
   return res ;
}

/*##########################################################*/
/*#######   FUNCIONES DE CRONOMETRO IMPLEMENTACION   #######*/
/*##########################################################*/

/*/ funcion auxiliar /*/
void muestra(int i,int tama,std::ostream& out){

  out.width(tama);
  out.fill('0');
  out << i ;
}

/*/ funcion de iniciar cronometro /*/
void cronometro::iniciar() {

  iniciado = true ;
  gettimeofday(&ti,NULL) ;
  //ti = time(0) ;
}

/*/ funcion para parar cronometro /*/
void cronometro::parar() {

  if ( iniciado ) {
     //tf = time (0) ;
     gettimeofday(&tf,NULL) ;
     iniciado = false;
     ver = true ;
  }
}

/*/ funcion que devuelve los milisegundos /*/
long cronometro::milisegundos () {

  long time_milsecond ;
  if ( ver ) {

    time_milsecond =  (tf.tv_sec -ti.tv_sec)*1000 ;
    time_milsecond += (tf.tv_usec - ti.tv_usec)/1000 ;
    //double dif = difftime(tf,ti) ;
    //time_milsecond = (long)(dif*1000.0) ;
    return time_milsecond ;
  }
  else
    return -1 ;
}

/*/ funcion que muestra por pantalla el cronometro /*/
void cronometro::mostrar(){

  int horas,minutos,segundos,centesimas;
  long milsegundos = (*this).milisegundos() ;

  if (milsegundos > 0 ) {
    horas = milsegundos / 3600000 ;
    milsegundos %= 3600000 ;
    minutos = (milsegundos / 60000) ;
    milsegundos %= 60000 ;
    segundos = ( milsegundos / 1000 ) ;
    milsegundos %= 1000 ;
    centesimas = milsegundos / 10 ;
    muestra( horas ,2, cout ) ;
    cout << ":" ;
    muestra( minutos ,2,cout ) ;
    cout << ":" ;
    muestra( segundos ,2, cout ) ;
    cout << "." ;
    muestra( centesimas ,2, cout ) ;
  }
}

#endif
/* 
  $Id: funciones_auxiliares.cpp
  v 1.0 20/08/2004 11:51:30 
  David Perez Cabrera
*/
