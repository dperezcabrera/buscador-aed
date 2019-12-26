/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@@@@@@@@   MODULO DE  INTERFAZ GENERAL   @@@@@@@@@@@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#include           "screen.cpp"
#include           "runcom.cpp"
#include           "buffer.cpp"
#include "estructura_datos.cpp"

#include              <fstream>
#include             <iostream>

using namespace std;

void saludo_inicial() {

  clear_screen();
  cout << "  ################################################################" << endl
       << "  #                                                              #" << endl 
       << "  #   ########################################################   #" << endl
       << "  #   #                                                      #   #" << endl 
       << "  #   #     AED  -  Curso  2003/2004  -  BUSCADOR  -  UM     #   #" << endl
       << "  #   #                                                      #   #" << endl 
       << "  #   #         Version 1.0        --  2004  --              #   #" << endl
       << "  #   #                                                      #   #" << endl
       << "  #   #                             2º   I.T.I. de Gestion   #   #" << endl
       << "  #   #                                                      #   #" << endl
       << "  #   #             por:                                     #   #" << endl
       << "  #   #                        David Perez Cabrera           #   #" << endl 
       << "  #   #                                                      #   #" << endl
       << "  #   ########################################################   #" << endl
       << "  #                                                              #" << endl
       << "  ################################################################" << endl
       << endl << endl ;
}

void saludo_final() {

 clear_screen();
 cout << "  --  Gracias  por usar  nuestro  buscador  --"<< endl
      << "  esperamos que haya encontrado lo que buscaba"<< endl << endl ;
}

void Buscador(std::istream& in,std::ostream& out){

  bool salir = false ;
  Contenedor  T(1000,1000000) ;
  list<string> linea_comandos ; 
  list<string>       fmaestro ; 
  in.tie(&out) ;
  saludo_inicial() ;
  while ( not salir ) {	     

    Lee_buffer(linea_comandos,in,out) ;
    runcomand (T,fmaestro,linea_comandos,salir,in,out);
  }
  saludo_final() ;
}

int main(int argc, char *argv[]) {
   
   if ( (argc == 3) or (argc == 1) ) {
      char *f_in  = NULL ;
      char *f_out = NULL ;
      bool es_standar = true ;	
      if (argc == 3 ) {
	 f_in  = argv[1] ;
	 f_out = argv[2] ;
	 es_standar = false ;
      }
     
      std::ifstream in( f_in ) ;
      if (not in)
        es_standar = true ;
      
      std::ofstream out(f_out);
      if (not out)
	es_standar = true ;
      if ( es_standar )
	Buscador( cin, cout ) ;
      else
	Buscador(  in,  out ) ; 
   }
   else {
     cerr << alert <<"ERROR: numero de argumentos invalido, se necesitan dos o ninguno" << endl << endl ;
     return 1;
   }
}

/* 
  $Id: principal.cpp
  v 1.0 28/07/2004 18:51:30 
  David Perez Cabrera
*/
