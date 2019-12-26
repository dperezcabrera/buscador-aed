/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@   MODULO DE PLANTILLA DE CLOSE_HASH IMPLEMENTACION   @@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#ifndef  __CLOSE_HASH_CPP
#define  __CLOSE_HASH_CPP

#include       <iostream>

#include "close_hash.hpp"
#include     "screen.cpp"

template<class T, class key>
//creacion de tabla close_hash para i elementos
close_hash<T,key>::close_hash(const int i , funcion_hash f_h) {
  
  f_hash = f_h ;
  if ( i > 0 ) {
    tama = i ;
    indices    = new int[i] ;
    siguientes = new int[i] ;
    elementos  = new T*[i] ;
    claves     = new key*[i] ;
    num_elementos = 0 ;
    gestor_vacios = 0 ;
    /*/ inicializamos la tabla /*/
      for (int j = 0; j < i ; j++ ) {
	siguientes[j] = j + 1 ;
	indices[j]    =    -1 ;
	claves[j]     =  NULL ;
	elementos[j]  =  NULL ;
      }
      siguientes[i-1] = -1 ;
  }
  else {
    cerr << alert << "ERROR: parametro del constructor incorrecto" 
	 << endl ;
    exit(1) ;
  }
}


template<class T,class key>
//creacion de una tabla close_hash para 100 elementos por defecto
close_hash<T,key>::close_hash(funcion_hash f_h) {

  f_hash = f_h ;
  int i = 1000 ;
  tama  =    i ;
  indices    = new int[i] ;
  siguientes = new int[i] ;
  elementos  = new T*[i] ;
  claves     = new key*[i] ;
  num_elementos = 0 ;
  gestor_vacios = 0 ;
  /*/ inicializamos la tabla /*/
    for (int j = 0; j < i ; j++ ) {
      siguientes[j] = j + 1 ;
      indices[j]    =    -1 ;
      claves[j]     =  NULL ;
      elementos[j]  =  NULL ;
    }
    siguientes[i-1] = -1 ;
}


template<class T,class key>
//inicializacion de la tabla con i elementos
void close_hash<T,key>::init(const int i, funcion_hash f_h) {
  
  f_hash = f_h ;
  if ( i > 0 ) {
    tama = i ;
    indices    = new int[i] ;
    siguientes = new int[i] ;
    elementos  = new T*[i] ;
    claves     = new key*[i] ;
    num_elementos = 0 ;
    gestor_vacios = 0 ;
    /*/ inicializamos la tabla /*/
      for (int j = 0; j < i ; j++ ) {
	siguientes[j] = j + 1 ;
	indices[j]    =    -1 ;
	elementos[j]  =  NULL ;
	claves[j]     =  NULL ;
      }
      siguientes[i-1] = -1 ;
  }	  
  else {
    cerr << alert << "ERROR: parametro del constructor incorrecto"
	 << endl ;
    exit(1) ;
  }
}



template<class T,class key>
//func destructor de una tabla close_hash
close_hash<T,key>::~close_hash() {
 
  if ( tama > 0 ) {
    if ( elementos  != NULL ) {
      for(int k = 0 ; k < tama ; k++ ) {
	if (elementos[k] != NULL) {
	  delete (elementos[k]) ;
	}
      }
      delete  (elementos) ;
    }
    if (  indices  != NULL ) {
      delete (indices) ;
    }
    if (   claves  != NULL ) {
      for(int k = 0 ; k < tama ; k++) {
	if (claves[k] != NULL) {
	  delete(claves[k]);
	}
      }
      delete (claves) ;
    }
    if (siguientes != NULL ) {
      delete siguientes ;
    }
    tama          = 0 ;
    gestor_vacios = 0 ;
    num_elementos = 0 ;
  }
}

template<class T, class key>
//para limpiar la tabla close_hash
void close_hash<T,key>::clear() {
  
  if (tama > 0) {
    if ( elementos  != NULL ) {
      for(int k = 0 ; k < tama ; k++ ) {
	if (elementos[k] != NULL) {
	  delete (elementos[k]) ;
	}
      }
      delete  (elementos) ;
    }
    if (  indices  != NULL )
      delete (indices) ;
    if (   claves  != NULL ) {
      for(int k = 0 ; k < tama ; k++) {
	if (claves[k] != NULL)
	  delete(claves[k]);
      }
      delete (claves) ;
    }
    if (siguientes != NULL )
    delete (siguientes) ;
    tama          = 0 ;
    gestor_vacios = 0 ;
    num_elementos = 0 ;
  }
}

template<class T, class key>
//devuelve num de elementos en la tabla close_hash
int close_hash<T,key>::size(){
  return num_elementos;//aquí estará el num de elementos
}


template<class T,class key>
// insertra un elemento con su clave
bool close_hash<T,key>::insert(const key clav,const T &dato) {
  
  if ( tama > 0 ) {
    T   *p = new T   ;
    key *c = new key ;
    (*p)   =    dato ;
    (*c)   =    clav ;
    //cout << " antes " << endl ;
    return (*this).inserta(c,p) ;
  }
  else {
    cerr << alert << "ERROR: elemento del tipo close_hash "
	 << "destruido " << endl ;
    exit(1);
  }
}


template<class T,class key>
// funcion auxiliar par insertar 
// manteniendo siempre el mismo algoritmo
bool close_hash<T,key>::inserta( key *clav, T* dato) {

  if ( ( num_elementos < tama ) and (gestor_vacios != -1 ) ) {
    
    //cout << " entra " << endl ;
    int indice_inicial = f_hash(*clav) % tama ;
    if (indice_inicial < 0 ) {
      cerr << alert << "ERROR: en definicion de funcion hash" << endl;
      exit(1);
    }
    
    int j = indices[indice_inicial] ;
    int j_end = -1 ;

    if ( j == j_end ) {
     
      // crea nueva lista
      //cout << " caso 1 " << endl ;
      indices[indice_inicial] = gestor_vacios ;
      j = gestor_vacios ;
      gestor_vacios = siguientes[j] ;
      num_elementos++ ;
      claves[j]       =    clav ;
      elementos[j]    =    dato ;
      siguientes[j]   =   j_end ;
      if ( ( gestor_vacios < -1 ) or ( gestor_vacios >= tama ) ) {
	cerr <<"ERROR: en inserta" << endl ;
	exit(1);
      }
      return true;
    }
    else {
      key *clave = claves[j] ;
      int anterior = indice_inicial ;
      while ( ( (*clave) < (*clav) ) and ( siguientes[j] != j_end ) ) {
	anterior    =   j ;
	j = siguientes[j] ;
	clave = claves[j] ;
      }
      if ( (*clave) == (*clav) ) {
	
	//cout << " caso 2 " << endl ;
	if ( ( gestor_vacios < -1 ) or ( gestor_vacios >= tama ) ) {
	  cerr <<"ERROR: en inserta" << endl ;
	  exit(1);
	}
	return false ;
      }
      else {
	// inserta en la primera posicion de la lista
	if ( anterior == indice_inicial ) {
	  
	  //cout << " caso 3 " << endl;
	  int    aux = indices[anterior] ;
	  indices[anterior] = gestor_vacios ;
	  int      i = gestor_vacios ;
	  gestor_vacios = siguientes[i] ;
	  siguientes[i] =  aux ;
	  elementos [i] = dato ;
	  claves[i]     = clav ;
	  num_elementos++;
	  if ( ( gestor_vacios < -1 ) or ( gestor_vacios >= tama ) ) {
	    cerr <<"ERROR: en inserta" << endl ;
	    exit(1);
	  }
	  return true ;
	}
	else {
	  // j es mayor 
	  // que el elemento que queremos insertar
	  // entrara por este if
	  // y retrocedera hasta el indice anterior
	  // pero si hemos llegado al final de la lista
	  // y el elemento que vamos a insertar es mayor
	  // no entrara por el if
	  if ((*clav) < (*clave) ) {
	    j = anterior ;
	  }
	  //cout << "caso 4 " << endl ;
	  int       aux = siguientes[j] ;
	  siguientes[j] = gestor_vacios ;
	  int         i = gestor_vacios ;
	  gestor_vacios = siguientes[i] ;
	  siguientes[i] =           aux ;
	  elementos[i]  =          dato ;
	  claves[i]     =          clav ;
	  num_elementos++;
	  if ( ( gestor_vacios < -1 ) or ( gestor_vacios >= tama ) ) {
	    cerr <<"ERROR: en inserta" << endl ;
	    exit(1);
	  }
	  return true ;
	}
      }
    }
  }
  else {
    (*this).resize(200) ;
    return (*this).inserta(clav,dato) ;
  }
}



template<class T,class key>
// borra un elemento si existe
bool close_hash<T,key>::erase(const key clav) {

  if ( tama > 0 ) {
    int indice_inicial = f_hash(clav) % tama ;
    int anterior = indice_inicial ;
    if (indice_inicial < 0 ) {
      cerr << alert << "ERROR: en definicion de funcion hash" << endl;
      exit(1);
    }
    int j = indices[indice_inicial] ;
    int j_end = -1 ;
    if ( j != j_end ) {
      key *clave = claves[j] ;
      while ( ( (*clave) < clav) and ( siguientes[j] != j_end ) ) {
	anterior     =  j ;
	j = siguientes[j] ;
	clave = claves[j] ;
      }
      if ( (*clave) == clav ) {
	if ( anterior != indice_inicial ) {
	  siguientes[anterior] = siguientes[j] ;
	  siguientes[j]        = gestor_vacios ;
	  gestor_vacios        =             j ;
	  num_elementos-- ;
	  delete (elementos[j]) ;
	  elementos[j]  =  NULL ;
	  delete  ( claves[j] ) ;
	  claves[j]     =  NULL ;
	  if ( ( gestor_vacios < -1 ) or ( gestor_vacios >= tama ) ) {
	    cerr <<"ERROR: en erase" << endl ;
	    exit(1);
	  }
	  return true ;
	}
	else {
	  indices[indice_inicial] = siguientes[j] ;
	  siguientes[j] = gestor_vacios ;
	  gestor_vacios = j ;
	  num_elementos-- ;
	  delete (elementos[j]) ;
	  elementos[j]  =  NULL ;
	  delete  ( claves[j] ) ;
	  claves[j]     =  NULL ;
	  if ( ( gestor_vacios < -1 ) or ( gestor_vacios >= tama ) ) {
	    cerr <<"ERROR: en erase" << endl ;
	    exit(1);
	  }
	  return true ;
	}
      }
      else {
	if ( ( gestor_vacios < -1 ) or ( gestor_vacios >= tama ) ) {
	  cerr <<"ERROR: en erase" << endl ;
	  exit(1);
	}
	return false ;
      }
    }
  }
  else {
    cerr << alert << "ERROR: elemento del tipo close_hash "
	 << "destruido " << endl ;
    exit(1);
  }  
}

template<class T, class key>
// busca un elemento si existe
// si no devuelve NULL
T* close_hash<T,key>::search(const key clav) {
  
  if ( tama <= 0 ) {
    cerr << alert << "ERROR: elemento del tipo close_hash "
	 << "destruido " << endl ;
    exit(1);
  }
  int indice_inicial = (f_hash(clav) % tama) ;
  int anterior = indice_inicial ;
  if (indice_inicial < 0 ) {
    cerr << alert << "ERROR: en definicion de funcion hash" << endl;
    exit(1);
  }
  int j = indices[indice_inicial] ;
  int j_end = -1 ;
  if ( j != j_end ) {
    key *clave = claves[j] ;
    while ( ( (*clave) < clav ) and ( siguientes[j] != j_end ) ) {
      anterior    =   j ;
      j = siguientes[j] ;
      clave = claves[j] ;
    }
    if ( (*clave) == clav ) {
      return (elementos[j]) ;
    }
    else
      return NULL ;
  }
  else
    return NULL ;
}

template<class T,class key>
// devuelve el primer elemento de close_hash 
bool close_hash<T,key>::front (key*& clav, T*& elem) {
  
  if ( tama > 0 ) {
    if ( num_elementos == 0 ) {
      return false ;
    }
    else {
      int i = 0 ;
      while ( ( i < tama ) and ( indices[i] == -1 ) ) {
	i++ ;
      }
      if ( i < tama ) {
	int j =   indices[i] ;
	elem  = elementos[j] ;
	clav  =    claves[j] ;
	indices[i]    = siguientes[j] ;
	siguientes[j] = gestor_vacios ;
	gestor_vacios =             j ;
	num_elementos-- ;
	elementos[j]  =  NULL ;
	claves[j]     =  NULL ;
	return  true ;
      }
      else {
	return false ;
      }
    }
  }
  else {
    cerr << alert << "ERROR: elemento del tipo close_hash "
	 << "destruido " << endl ;
    exit(1);
  }
}

template<class T,class key>
// borra el primer elemento de close_hash
bool close_hash<T,key>::pop_front() {

  if ( tama > 0 ) {
    if ( num_elementos == 0 ) {
      return false ;
    }
    else {
      int i = 0 ;
      while ( ( i < tama ) and ( indices[i] == -1 ) ) {
	i++ ;
      }

      if ( i < tama ) {
	int   j       =    indices[i] ;
	indices[i]    = siguientes[j] ;
	siguientes[j] = gestor_vacios ;
	gestor_vacios =             j ;

	num_elementos-- ;
	delete (elementos[j]) ;
	elementos[j]  =  NULL ;
	delete ( claves [j] ) ;
	claves[j]     =  NULL ;
	return  true ;
      }  
      else {  
	return false ;
      }
    }
  }
  else {
    cerr << alert << "ERROR: elemento del tipo close_hash "
	 << "destruido " << endl ;
    exit(1);
  }
}

template<class T,class key>
// muestra por pantalla
// funcion hecha solo para la depuracion
void close_hash<T,key>::show () {
  (*this).show(cout) ;
}

template<class T,class key>
// muestra la tabla con sus datos
// funcion hecha solo para la depuracion
void close_hash<T,key>::show (std::ostream& out) {

  out.flush();
  if ( tama > 0 ) {
    if ( elementos and claves and siguientes and indices ) {
      cout <<" GESTOR VACIOS "<< gestor_vacios << endl ;

      for(int i = 0 ; i < tama ; i++ ) {

	if (elementos[i] and claves[i] ) {
	  out <<  i << tab  << (*elementos[i]) 
	      << tab  <<  (*claves[i]) <<  tab
	      <<     indices[i]     <<     tab 
	      <<   siguientes[i]    <<  endl ;
	}
	else  {
	  out << i  <<  tab <<  "(NULL)" << tab
	      << "(NULL)" << tab  << indices[i]
	      << tab << siguientes[i] << endl ;
	}   
      }
      out << endl ;
    }
    else {
      out << "NADA QUE MOSTRAR" << endl ;
    }   
  }
  else {
     out << "NADA QUE MOSTRAR" << endl ;
  }
}

template<class T,class key>
// redimensiona close_hash un porcentage si pasamos 50
// close_hash se convertira en la mitad de la original
// y si pasamos 200 se convertira en el doble de grande
void close_hash<T,key>::resize   (const int porcentage) {

  if ( tama > 0 ) {
    if ( porcentage > 0 ) {

      close_hash<T,key> ampliada( ((tama*porcentage )/100),f_hash) ;
      ampliada.asign( *this ) ;
      tama          =          ampliada.tama ;
      elementos     =     ampliada.elementos ;
      ampliada.elementos        =       NULL ;
      indices       =       ampliada.indices ;
      ampliada.indices          =       NULL ;
      siguientes    =    ampliada.siguientes ;
      ampliada.siguientes       =       NULL ;
      claves        =        ampliada.claves ;
      ampliada.claves           =       NULL ;
      gestor_vacios = ampliada.gestor_vacios ;
    }
    else {
      (*this).clear() ;
    }
  }
}

template<class T,class key>
// asigna una close_hash a otra
bool close_hash<T,key>::asign(close_hash<T,key>& fuente) {

  if ( fuente.tama > 0) {
    T *element = NULL ;
    key  *clav = NULL ;
    while ( fuente.front(clav,element) ) {
      (*this).inserta(clav,element) ;
    }
  }
  else {
    cerr << alert << "ERROR: elemento del tipo close_hash "
	 << "destruido " << endl ;
    exit(1);
  }
}

template<class T,class key>
// devuelve el tamaño en bytes 
// que necesita la estructura close_hash
int  close_hash<T,key>::size_of() {

  int sizeofbytes ;
  sizeofbytes = sizeof( int ) * 3 + sizeof( funcion_hash ) + sizeof(key**) ;
  sizeofbytes += sizeof(T**) + sizeof(int *)*2 + sizeof(int) * (2 * tama ) ;
  sizeofbytes += ( sizeof(T*) + sizeof(key*) ) * tama ;
  for ( int i = 0 ; i < tama ; i++ ) {
    if ( claves[i] != NULL ) {
      sizeofbytes += sizeof (*( claves[i] ));
    }
  }
  return sizeofbytes ;
}

template<class T,class key>
// accion que realizamos a todos los elementos
void close_hash<T,key>::for_each (accion hacer,void *datos) {

  if ( tama > 0 ) {
    int i = 0 ; 
    if ( ( num_elementos > 0 ) and ( tama > 0 ) ) { 
      while ( i < tama ) {
	if ( elementos[i] != NULL ) {
	  hacer( (*(elementos[i])),  datos ) ;
	}
	i++ ;
      }
    }
  }
  else {
    cerr << alert << "ERROR: elemento del tipo close_hash "
	 << "destruido " << endl ;
    exit(1);
  }
}

#endif
/*
  $Id: close_hash.cpp
  v 1.0 21/07/2004 10:36:00
  David Perez Cabrera
*/
