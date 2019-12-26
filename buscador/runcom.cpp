/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@@@@@@@   MODULO DE RUNCOM IMPLEMENTACION   @@@@@@@@@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#ifndef RUNCOM_CPP
#define RUNCOM_CPP

#include "runcom.hpp"


/*/ funcion auxiliar para insert /*/
string quitar_p(const char * c,int i) {

  string sol;
  int j = i-1 ;
  const char* res = c + (j) ;
  while ( (c[j] != endstrg ) and ( c[j] == ' ' ) ) {
    j++;
    res = res +1 ;
  }
  sol =  res ;
  return sol ;
}

/*/ buscar la palabra reservada /*/
int palabra_reservada(string pal) {
   
  switch (pal[0]) 
    {
    case 'L':{if ( pal=="LOAD"  )     {return LOAD;  } 
    else return OTRA; break;}
 
    case 'I':{if ( pal=="INSERT")     {return INSERT;} 
    else return OTRA; break;}
      
    case 'R':{if ( pal=="REMOVE")     {return REMOVE;} 
    else return OTRA; break;}
      
    case 'S':{if ( pal=="SAVE"  )     {return SAVE;  }
    if ( pal=="SEARCH")               {return SEARCH;}
    else return OTRA; break;}
      
    case 'H':{if ( pal=="HELP"  )     {return HELP;  } 
    else return OTRA; break;}
      
    case 'Q':{if ( pal=="QUIT"  )     {return QUIT;  } 
    else return OTRA; break;}
      
    case 'A':{if ( pal=="AND"    )    {return op_and;} 
    else return OTRA; break;}
      
    case 'O':{if ( pal=="OR"     )    {return op_or; }
    else return OTRA; break;}
  
    case 'C':{if ( pal=="COM"    )    {return op_com;} 
    if ( pal=="CLEAR" )               {return CLEAR; }
    else return OTRA; break;}
      
    case 'N':{if ( pal=="NOT"    )    {return op_not;}
    else return OTRA; break;}        
    default: 
      return OTRA ;
    }
}

/*/ comando search /*/
void search(Contenedor& T,list<string>& lista,std::ostream& out) {
   
  list<string>::iterator j = lista.begin() ;
  list<string>::iterator end = lista.end() ;
  int primera_palabra ;
  lista.pop_back() ;
  if ( j != end ) {
    string  pal_aux = (*j) ;
    string  palabra = mayusculas( (*j).c_str() )   ;
    primera_palabra = palabra_reservada( palabra ) ;
    lista.pop_front( ) ;
    switch ( primera_palabra ) 
      {
      case op_and:{ T.And(lista,out) ; break;}
      case op_or :{ T.Or (lista,out) ; break;}
      case op_not:{ T.Not(lista,out) ; break;}
      case op_com:{ T.Com(lista,out) ; break;}
      default:{
	cerr <<  alert   <<   "ERROR: argumento " 
	     <<                           pal_aux 
	     <<" no valido, consulte HELP SEARCH" 
	     <<      endl              <<  endl ;
      }
      }
  }
  else
    cerr << alert 
	 << "ERROR: pocos argumentos para SEARCH,"
         << " consulte HELP SEARCH" 
	 << endl << endl ;
}

/*/ comando load /*/
void load(Contenedor& T,list<string>& maestro,
	  list<string>& linea_comandos,std::ostream& out) {

  linea_comandos.pop_back() ;
  if ( linea_comandos.size() == 1 ) {
    T.Load(linea_comandos.front(),maestro) ;
  }
  else {
    cerr << "ERROR: numero de argumentos equivocado para la funcion load"
	 << endl << endl ;
  }
}

/*/ comando save /*/
void save(list<string>& maestro,list<string>& linea_comandos){
  
  linea_comandos.pop_back() ;
  if (linea_comandos.size() == 1 ) {
    string f_maestro = linea_comandos.front() ;
    std::ofstream out( f_maestro.c_str(),ios_base::out ) ;
    if ( out ) {
      list<string>::iterator i = maestro.begin() ;
      list<string>::iterator end = maestro.end() ;
      while ( i != end ) {
	out << (*i)<< endl ;
	i++ ;
      }
    }
    else {
      cerr << alert <<"ERROR: no se pudo abrir el fichero "
	   <<f_maestro << endl << endl ;
    }
  }
  else{
    cerr << alert << "ERROR: parametros incorrectos para SAVE: "
	 << endl  << endl;
  }
}

/*/ comando insert /*/
void insert(Contenedor& T,list<string>& maestro,
	    list<string>& linea_comandos){
 
  if ( linea_comandos.size() > 2 ) {
    string  linea = linea_comandos.back() ;
    istringstream linea_maestro(linea) ;
    string aux ;
    linea_maestro >> aux ;
    string url ;
    string nombre_fich_pagina ;
    int relevancia = 0 ;  
    linea_maestro >> url ;
    // para leer la url
    linea_maestro >> nombre_fich_pagina ;
    // para leer el nombre del fichero de pagina
    linea_maestro >> relevancia ;
    // para leer la relevancia de la pagina
    T.Insert(url,nombre_fich_pagina,relevancia) ;

    list<string>::iterator i = linea_comandos.begin() ;
    list<string>::iterator end = linea_comandos.end() ;
    list<string>::iterator anterior = maestro.begin() ;
       
    linea = quitar_p(linea.c_str() ,7 ) ;

    i = maestro.begin() ;
    end = maestro.end() ;
    if ( maestro.size() == 0 ) {
      // si no hay elementos en maestro
      maestro.push_front(linea);
    }
    else {
      i++ ;
      while (( i != end ) and ( (*anterior) < linea ) ) {
	i++ ;
	anterior++ ;
      }
      if ( i != maestro.begin() ) {
	i-- ;
	maestro.insert(i,linea) ;
      }
      else {
	maestro.push_front(linea);
      }
    }
  }
  else {
    cerr << alert 
	 <<"ERROR: en atributos del comando insert"
	 << endl << endl ;
  }
}

/*/ comando remove /*/
void remove(Contenedor& T,list<string>& maestro,
	    list<string>& linea_comandos){

  if ( linea_comandos.size() == 2 ) {
    string fich = linea_comandos.front() ;
   
    if ( T.Erase (fich) ) {
      
      list<string>::iterator i = maestro.begin() ;
      list<string>::iterator end = maestro.end() ;
      bool salir = false ;
      
      while ( ( i != end ) and ( (*i) < fich  ) ) {
	i++ ; 
      }
      istringstream linea(*i) ;
      string url ;
      linea >> url ;
   
      if ( url == fich ) {
	maestro.erase(i) ;
      }
      else {
	cerr << alert 
	     <<"ERROR: el fichero a borrar no existe"
	     << endl << endl ;
	
      }
    }
    else {
      cerr << alert 
	   <<"ERROR: el fichero a borrar no existe"
	   << endl << endl ;
    }
  }
  else {
    cerr << alert
	 <<"ERROR: en atributos del comando remove"
	 << endl << endl ;
  }
}

/*/ comando help /*/
void help(list<string>& lista, std::ostream& out) {
   
  int palabra ;
  string j ;
  if ( lista.size() > 1 )
    j = lista.front();
  else {
    j = endstrg ;
    lista.clear();
  }
  if ( j[0] != endstrg ) {
    palabra = palabra_reservada( mayusculas( j.c_str() ) );
    // la funcion 'mayusculas' esta en funciones_auxiliares.hpp/cpp
    out << endl ;
    switch(palabra) 
      {
      case HELP     : { out <<"   HELP: .... para ver la ayuda de los"<< endl
			    <<" distintos comandos, como load, insert"<< endl
			    <<" remove,  save,  search,  clear,  quit"<< endl
			    <<" and, or, not, com o xor"    << endl ; break;}
      case LOAD     : { out <<"   LOAD: .... para  cargar  un fichero"<< endl
			    <<" maestro,  ejemplo: 'load MAESTRO.txt'"<< endl
			    <<" al finalizar mostrara los datos"<<endl;break;}
      case INSERT   : { out <<"   INSERT: .. para insertar  una nueva"<< endl
			    <<" pagina web  con el siguiente formato:"<< endl
			    <<" insert URL NOMBRE_FICHERO  RELEVANCIA"
			    << endl ; break;}
      case REMOVE   : { out <<"   REMOVE: .. para  borrar una  pagina"<< endl
			    <<" web con el siguiente formato" <<endl
			    <<" remove FICHERO_PAGINA"<< endl ; break;}
      case SAVE     : { out <<"   SAVE: .... para  guardar un fichero"<< endl
			    <<" maestro con las modificaciones hechas"<< endl
			    <<" ejemplo: save FICH_MAESTRO" << endl ; break;}
      case SEARCH   : { out <<"   SEARCH: .. para realizar  busquedas"<< endl
			    <<" necesita otro comando  como puede ser"<< endl
			    <<" AND, OR, NOT, COM  o XOR;  ademas  de"<< endl
			    <<" una o varias  palabras  para realizar"<< endl
			    <<" las busquedas oportunas"    << endl ; break;}
      case CLEAR    : { out <<"   CLEAR: ... para limpiar la pantalla" 
			    << endl ; break;}
      case QUIT     : { out <<"   QUIT: .... para salir  del buscador" 
			    << endl ; break;}
      case op_or    : { out <<"   OR: ...... para buscar  las paginas"<< endl
			    <<" en las cuales aparecen almenos una de"<< endl
			    <<" las palabras  pasadas como  parametro"
			    << endl ; break;}
      case op_not   : { out <<"   NOT: ..... para buscar  las paginas"<< endl
			    <<" en las cuales  no aparece  ninguna de"<< endl
			    <<" las palabras que recibe por parametro"
			    << endl ; break;}
      case op_and   : { out <<"   AND: ..... para buscar  las paginas"<< endl
			    <<" en las que aparecen todas las palabras"<<endl
			    <<" pasadas como parametro"     << endl ; break;}
      case op_com   : { out <<"   COM: ..... para buscar  las paginas"<< endl
			    <<" en las que aparece las palabras en el"<< endl
			    <<" orden en el que estan escritas"<<endl;break;}
      default:{
	out << alert <<"   No esta en HELP la palabra: "
	    << j << endl << "pruebe con HELP HELP"
	    << endl << endl ;
      }	  
      }
  }
  else 
    out <<"   Para usar el comando HELP es necesario escribir una palabra"<<endl
	<<" como  'help help' o 'help load' o 'help and'  o 'help search'"<<endl
	<<" hay varios  comandos como insert, remove, save;  solo hay que"<<endl
	<<" que escribir 'help' y el comando a buscar la ayuda" << endl ;
}

/*/ funcion runcomand /*/
// ejecuta un comando
void runcomand(Contenedor& T,list<string>& maestro ,list<string>& linea_comandos,
	       bool& salir,std::istream& in,std::ostream& out) {
      

  int primera_palabra ;
  list<string>::iterator j = linea_comandos.begin() ;
  string pal_aux  = (*j) ;
  string palabra  =   mayusculas( (*j).c_str() ) ;
  // la funcion 'mayusculas' esta en funciones_auxiliares.hpp/cpp 
  primera_palabra = palabra_reservada( palabra ) ;
  linea_comandos.pop_front() ;
  
  switch ( primera_palabra ) 
    {
    case LOAD  :{ load(T,maestro,linea_comandos,out) ; break ; }
    case INSERT:{ insert(T,maestro,linea_comandos )  ; break ; }
    case REMOVE:{ remove(T,maestro,linea_comandos )  ; break ; }
    case SAVE  :{ save  (maestro , linea_comandos )  ; break ; }
    case SEARCH:{ search(T,linea_comandos,out)       ; break ; }
    case HELP  :{ help  (linea_comandos,out)         ; break ; }
    case CLEAR :{ clear_screen( )                    ; break ; }
    case QUIT  :{ salir = true                       ; break ; }
      
    default:// Otra palabra
      cerr << alert <<"ERROR: comando "<< pal_aux 
	   <<" no reconocido, pruebe con HELP"
	   << endl << endl ;
    }
}

#endif
/* 
  $Id: runcom.cpp
  v 1.0 28/07/2004 18:51:30 
  David Perez Cabrera
*/
