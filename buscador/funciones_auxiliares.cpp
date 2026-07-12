/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@  MODULO AUXILIAR DE FUNCIONES  PARA TRATAR PALABRAS  @@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#ifndef FUNCIONES_AUXILIARES_CPP
#define FUNCIONES_AUXILIARES_CPP

#include "funciones_auxiliares.hpp"


#define LETRA_MENOR  65
#define LETRA_MAYOR  90

/*/ quita tildes y pasa a minusculas /*/
// las paginas de entrada vienen en ISO-8859-1, por eso
// las vocales acentuadas van como escapes \x de esa codificacion
void trasforma_letra(char &letra) {

  switch (letra)
    {
    case '\xe1': // á
    case '\xe0': // à
    case '\xe4': // ä
    case '\xe2': // â
    case '\xc1': // Á
    case '\xc0': // À
    case '\xc4': // Ä
    case '\xc2': { letra = 'a' ; break ;} // Â
    case '\xe9': // é
    case '\xe8': // è
    case '\xeb': // ë
    case '\xea': // ê
    case '\xc9': // É
    case '\xc8': // È
    case '\xcb': // Ë
    case 'E':
    case '\xca': { letra = 'e' ; break ;} // Ê
    case '\xed': // í
    case '\xec': // ì
    case '\xef': // ï
    case '\xee': // î
    case 'I':
    case '\xcd': // Í
    case '\xcc': // Ì
    case '\xcf': // Ï
    case '\xce': { letra = 'i' ; break ;} // Î
    case '\xf3': // ó
    case '\xf2': // ò
    case '\xf6': // ö
    case '\xf4': // ô
    case 'O':
    case '\xd3': // Ó
    case '\xd2': // Ò
    case '\xd6': // Ö
    case '\xd4': { letra = 'o' ; break ;} // Ô
    case '\xfa': // ú
    case '\xf9': // ù
    case '\xfc': // ü
    case '\xfb': // û
    case 'U':
    case '\xda': // Ú
    case '\xd9': // Ù
    case '\xdb': // Û
    case '\xdc': { letra = 'u' ; break ;} // Ü
    }
  if (( letra >= LETRA_MENOR ) and ( letra <= LETRA_MAYOR )or ( letra == '\xd1' ) ) { // Ñ
    letra += 32 ;
  }
}

/*/ aceptar las letras y trasformarlas /*/
// para transformar la letra y ver si es un caracter alfabetico

bool aceptar_letra(char &c) {
// si dos palabras estan separadas por comas, puntos,etc........

  trasforma_letra( c ) ;
  return ( isalpha( (unsigned char) c ) or (c == '\xf1') ) ; // ñ
}

/*/ convierte una cadena a minusculas /*/
string minusculas(const char *palabra) {

   string res( palabra ) ;
   int longitud = res.length() ;
   for( int i = 0 ; i < longitud ; i++ ) {
     trasforma_letra ( res[i] ) ;
   }
   return res ;
}

/*/ convierte una cadena a mayusculas /*/
string mayusculas(const char *palabra) {

   string res( palabra ) ;
   int longitud = res.length() ;
   for( int i = 0 ; i < longitud ; i++ ) {
      res[i] = toupper( (unsigned char) res[i] ) ;
   }
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
