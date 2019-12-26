/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@   MODULO DE ESTRUCTURA DE DATOS IMPLEMENTACION   @@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

#ifndef    ESTRUCTURA_DATOS_CPP
# define   ESTRUCTURA_DATOS_CPP

#include "estructura_datos.hpp"

#define maxint  2147483647
#define SALIR  "#SALIR"


/*##########################################################*/
/*##################   OTRAS  FUNCIONES   ##################*/
/*##########################################################*/


/*/  Funcion hash /*/
int f_hash(string palabra) {

  int resultado = 0 ;
  int i = 0 ;
  while ( palabra[i] != endstrg ) {
    resultado = ((resultado *27) + (palabra[i] - 40) + i) % maxint;
    i++;
  }
  if (resultado < 0)
    resultado *= -1 ;
  resultado %= maxint ;
  return resultado ;
}

/*/ funcion hash2 basada en f_hash /*/
int f_hash2( Pagina * pag) {

   string s = pag->nom_fich ;
   return       f_hash( s ) ;
}

/*/ funcion auxiliar a la funcion COM /*/
bool f_aux_com(Nodo& res,Nodo& p,Nodo& q) {

   bool retur = false ;
   list<int>::iterator i,j,i_end,j_end ;
   res.l_aparic.clear  ( ) ;
   i = p.l_aparic.begin( ) ;
   i_end=p.l_aparic.end( ) ;
   j = q.l_aparic.begin( ) ;
   j_end=q.l_aparic.end( ) ;

   while ( (i != i_end) and (j != j_end) ) {
      if ( ((*j)-(*i)) == 1) {
	     retur = true ;
	     res.l_aparic.push_back(*j) ;
             i++ ; 
             j++ ;
      }
      else {
	 if ( ((*j) - (*i)) < 1 )
	   j++ ; 
	 else
	   i++ ;
      }   
   }
   return retur;
}

/*/ funcion auxiliar para load para leer de un fichero /*/
string lee(std::istream& in ) {

  string solucion = "" ;
  char c ;
  if ( in ) {
    in.get( c ) ;
    while ( (in ) and (not aceptar_letra(c)) ) {
      in.get( c ) ;
    }
    if ( in ) {
      solucion += c ;
      in.get( c ) ;
      while ( in and aceptar_letra(c) ) {
	solucion += c ;
	in.get( c );
      }
      return solucion ;
    }
    else
      return SALIR ;
   }
  else
    return SALIR ;
}

/*/ funcion para conocer el numero de bytes de lista<string> /*/ 
void medir_bytes(list<string>& l,void *&psizeof) {

  int * s = (int *)psizeof ;
  (*s)   += sizeof(l) ;
  psizeof = ( void *)s ;
}




/*##########################################################*/
/*##############   IMPLEMENTACION DE PAGINA   ##############*/
/*##########################################################*/

/*/ constructor /*/
Pagina::Pagina() {

   relevancia = -1 ; 
   aparicion  = -1 ;
}

/*/ otro constructor /*/
Pagina::Pagina(string& url,string& fich,int& rel,int& apa) { 

  URL = url ; 
  nom_fich   = fich ;
  relevancia = rel  ;
  aparicion  = apa  ;
}

/*/ tamaño en bytes de pagina /*/
int SizeofP(Pagina& p) { 

  int size = sizeof(p.URL)+ sizeof(p.nom_fich) + 2*sizeof(int) ; 
  return size ;
}

/*/ operador 'menor que' de Pagina /*/
bool operator<(Pagina p,Pagina q) {

  if(p.relevancia>q.relevancia)
    return true ;
 
  else {
    if (p.relevancia == q.relevancia)
       return (p.aparicion<q.aparicion);  
    else
       return false ;
  }
}

/*/ operador 'mayor que' de Pagina /*/
bool operator>(Pagina p,Pagina q) {
   return (q<p) ;
}

/*/ operador 'menor o igual que' de Pagina /*/
bool operator<=(Pagina p,Pagina q) {
   return (!(q>p)) ;
}

/*/ operador 'mayor o igual que' de Pagina /*/
bool operator>=(Pagina p,Pagina q) {
   return (!(p<q)) ;
}

/*/ operador 'igual que' de Pagina /*/
bool operator==(Pagina p,Pagina q) {
   return (p.aparicion==q.aparicion) ;
}

/*/ operador 'distinto de' de Pagina /*/
bool operator!=(Pagina p,Pagina q) {
   return ((p.aparicion) != (q.aparicion)) ;
}

/*##########################################################*/
/*###############   IMPLEMENTACION DE NODO   ###############*/
/*##########################################################*/

/*/  constructor de Nodo /*/
Nodo::Nodo(Pagina *pag,int& pos) {

   Pag = pag; 
   l_aparic.push_back(pos);
}

/*/  otro constructor de Nodo /*/
Nodo::Nodo(Pagina *pag) {
   Pag = pag; 
}

/*/ otro constructor de Nodo /*/
Nodo::Nodo() {
  Pag = NULL ;
}

/*/ destructor de nodo /*/
Nodo::~Nodo() {

   l_aparic.clear() ;
   Pag = NULL ;
}

/*/ tamaño en bytes de Nodo /*/
int SizeofN(Nodo& n) {

  int size = sizeof(n.l_aparic) ;
  size += sizeof(Pagina *) ;
  return size ;
}

/*/ operador 'igual que' de Nodo /*/
bool operator==(Nodo p,Nodo q) {
  return (*p.Pag) == (*q.Pag) ;
}

/*/ operador 'menor que' de Nodo /*/
bool operator< (Nodo p,Nodo q) {
  return (*p.Pag) < (*q.Pag) ;
}

/*/ operador 'mayor que' de Nodo /*/
bool operator> (Nodo p,Nodo q) {
  return (*q.Pag) < (*p.Pag) ;
}

/*/ operador 'mayor o igual que' de Nodo /*/
bool operator<=(Nodo p,Nodo q) {
  return not( (*q.Pag) < (*p.Pag ) ) ;
}

/*/ operador 'menor o igual que' de Nodo /*/
bool operator>=(Nodo p,Nodo q) {
  return not( (*p.Pag) < (*q.Pag) ) ;
}

/*/ operador 'distinto de' de Nodo /*/
bool operator!=(Nodo p,Nodo q) {
  return not( (*p.Pag) == (*q.Pag) ) ;
}

/*/ funcion miembro relevancia de Nodo /*/
int  Nodo::Relevancia() {
  return (*Pag).relevancia ;
}

/*/ funcion miembro aparicion de Nodo /*/
int  Nodo::Aparicion() {
  return Pag->aparicion ;
}

/*/ accion modificar pagina de Nodo /*/
void Nodo::Modif_pag(Pagina *pag) {
  Pag = pag ;
}

/*/ funcion miembro pagina de Nodo /*/
Pagina *Nodo::Pagin() {
  return Pag ;
}

/*/ funcion miembro url de Nodo /*/
string  Nodo::Url() {
  return Pag->URL ;
}

/*/ funcion miembreo nombre_fichero de Nodo/*/
string  Nodo::Fichero() {
  return Pag->nom_fich ;
}

/*##########################################################*/
/*###########   IMPLEMENTACION DE LISTA_DATOS   ############*/
/*##########################################################*/

/*/ constructor de lista /*/
Lista_datos:: Lista_datos() {

  error        = false ;
  permitir_com =  true ; 
}

/*/ constructor de lista /*/
Lista_datos::Lista_datos(Pagina *pag, int& pos) {  

   Nodo _nodo(pag,pos);
   list_nodo.push_front(_nodo);
   permitir_com =  true ;
   error        = false ;
}

/*/ destructor de lista /*/
Lista_datos::~Lista_datos() {

   list_nodo.clear();
}

/*/ borrar lista /*/
void Lista_datos::Clear() {

   list_nodo.clear (  ) ;
   permitir_com =  true ;
   error        = false ;
}

/*/ tamaño de una lista /*/
int  Lista_datos::Size() {

  int i = list_nodo.size() ;
  return i ;
}

/*/ muestra el contenido de una lista /*/
void Lista_datos::Show(std::ostream& fout)  {

   list<Nodo>::iterator j = list_nodo.begin();
   list<Nodo>::iterator end = list_nodo.end();
   int contador = 1 ;

   if ( not error ) {
     fout << list_nodo.size() << endl ;
   
     while( j != end ) {
       fout << contador <<". "<<(*j).Url() 
	    << "   " <<  (*j).Relevancia() 
	    << endl  ;
       contador++;
       j++;
     }	
   }
   else {
     cerr << alert <<"ERROR" << endl ;
   }
}

int SizeofL(Lista_datos& lista) {

   int size = 0 ;
   list<Nodo>::iterator j = lista.list_nodo.begin() ;
   list<Nodo>::iterator end = lista.list_nodo.end() ;
   
   while( j != end ) {
      size += SizeofN(*j) ;
      j++ ;
   }
   size += sizeof(lista) ;
   return size ;
}

/*/ funcion and  /*/
Lista_datos  AND(Lista_datos& p,Lista_datos& q) {

   Lista_datos solucion ;
   if ( not( p.error or q.error ) ) {
      Nodo _nodo;
      list<Nodo>::iterator i = p.list_nodo.begin();
      list<Nodo>::iterator i_end=p.list_nodo.end();  
      list<Nodo>::iterator j = q.list_nodo.begin();
      list<Nodo>::iterator j_end=q.list_nodo.end();
      solucion.permitir_com = false ;
      
      while ( (i != i_end) and (j != j_end) ) {  
	 if ( (*i).Aparicion() == (*j).Aparicion() ) {  
	    _nodo.Modif_pag( (*i).Pagin() );
	    solucion.list_nodo.push_back(_nodo);
	    i++ ;
	    j++ ;
	 }
	 else {  
	   if ( (*(*i).Pagin()) < (*(*j).Pagin()) )
	      i++ ;
	    else
	      j++ ;
	 }
      }
   }
   // condicion de error
   else {
      solucion.error        =  true ;
      solucion.permitir_com = false ;
   }
   return solucion ;
}

/*/ funcion or /*/
Lista_datos  OR(Lista_datos& p,Lista_datos& q) {

   Lista_datos solucion ;
   if ( not( p.error or q.error ) ) {
      Nodo _nodo;
      list<Nodo>::iterator i = p.list_nodo.begin();
      list<Nodo>::iterator i_end=p.list_nodo.end();
      list<Nodo>::iterator j = q.list_nodo.begin();
      list<Nodo>::iterator j_end=q.list_nodo.end();
      solucion.permitir_com = false;

      while ( (i != i_end) and (j != j_end) ) {
	 if( (*i).Aparicion() == (*j).Aparicion() ) {
	    _nodo.Modif_pag( (*i).Pagin() );
	    solucion.list_nodo.push_back(_nodo);
	    i++ ;
	    j++ ;
	 }
	 else {
	    if ( (*(*i).Pagin()) < (*(*j).Pagin()) ) {   
	       _nodo.Modif_pag( (*i).Pagin() );
	       solucion.list_nodo.push_back(_nodo);
	       i++;
	    }
	    else {
	       _nodo.Modif_pag( (*j).Pagin() );
	       solucion.list_nodo.push_back(_nodo);
	       j++;
	    }
	 }
      }
      while ( j != j_end ) {
	 _nodo.Modif_pag( (*j).Pagin() );
	 solucion.list_nodo.push_back(_nodo);
	 j++;
      }
      while ( i != i_end ) {
	 _nodo.Modif_pag( (*i).Pagin() );
	 solucion.list_nodo.push_back(_nodo);
	 i++;
      }
   }
   // condicion de error
   else {
      solucion.error        =  true ;
      solucion.permitir_com = false ;
   }
   return solucion;
}

/*/ funcion xor /*/
Lista_datos  XOR(Lista_datos& p,Lista_datos& q) {

   Lista_datos solucion ;
   if ( not( p.error or q.error ) ) {
      Nodo _nodo; 
      list<Nodo>::iterator i = p.list_nodo.begin();
      list<Nodo>::iterator i_end=p.list_nodo.end();
      list<Nodo>::iterator j = q.list_nodo.begin();
      list<Nodo>::iterator j_end=q.list_nodo.end();
      solucion.permitir_com = false ;

      while ( ( i != i_end ) and ( j != j_end ) ) {
	 if( (*i).Aparicion() == (*j).Aparicion() ) {
	    i++; 
	    j++; 
	 }      
	 else {  
	    if ( (*(*i).Pagin()) < (*(*j).Pagin()) )  {   
	       _nodo.Modif_pag( (*i).Pagin() ) ;
	       solucion.list_nodo.push_back(_nodo) ;
	       i++;
	    }
	    else {   
	       _nodo.Modif_pag( (*j).Pagin() ) ;
	       solucion.list_nodo.push_back(_nodo) ;
	       j++;
	    }
	 }
      }
      while (j != j_end) {
	 _nodo.Modif_pag( (*j).Pagin() );
	 solucion.list_nodo.push_back(_nodo);
	 j++;
      }
      while (i != i_end) {
	 _nodo.Modif_pag( (*i).Pagin() );
	 solucion.list_nodo.push_back(_nodo);
	 i++;
      }
   }
   // condicion de error
   else {
      solucion.error        =  true ;
      solucion.permitir_com = false ;
   }
   return solucion;
}

/*/ para optimizar el not /*/
Lista_datos  OPN(Lista_datos& p,Lista_datos& q) {

   Lista_datos solucion ;
   if ( not( p.error or q.error ) ) {

      Nodo _nodo;
      list<Nodo>::iterator i = p.list_nodo.begin() ;
      list<Nodo>::iterator i_end=p.list_nodo.end() ;
      list<Nodo>::iterator j = q.list_nodo.begin() ;
      list<Nodo>::iterator j_end=q.list_nodo.end() ;

      solucion.permitir_com = false ;
      while ( (i != i_end ) and (j != j_end) ) {
	 if ( (*i).Aparicion() == (*j).Aparicion() ) {
	    i++; 
	    j++;
	 }    
	 else {  
	    if ( (*(*i).Pagin()) < (*(*j).Pagin()) )  {   
	       _nodo.Modif_pag( (*i).Pagin() );
	       solucion.list_nodo.push_back(_nodo);
	       i++;
	    }
	    else
	      j++;
	 }
      }
      while (i != i_end) {
	 _nodo.Modif_pag( (*i).Pagin() );
	 solucion.list_nodo.push_back(_nodo);
	 i++;
      }
   }
   // condicion de error
   else {
      solucion.error        =  true ;
      solucion.permitir_com = false ;
   }
   return solucion;
}

/*/ realiza un com entre dos listas /*/
Lista_datos COM(Lista_datos& p,Lista_datos& q) {

   Lista_datos solucion ;
   if ( not( p.error or q.error ) and p.permitir_com and q.permitir_com ) {

      Nodo _nodo;
      list<Nodo>::iterator i = p.list_nodo.begin();
      list<Nodo>::iterator i_end=p.list_nodo.end();
      list<Nodo>::iterator j = q.list_nodo.begin();
      list<Nodo>::iterator j_end=q.list_nodo.end();

      solucion.permitir_com = true ;
      while ( ( i != i_end )  and ( j != j_end ) ) {
	 if ( (*i).Aparicion() == (*j).Aparicion() ) {
	    bool res=f_aux_com( _nodo , (*i) , (*j) );
	    if ( res ) {
	       _nodo.Modif_pag( (*i).Pagin() );
	       solucion.list_nodo.push_back(_nodo);
	    }
	    i++;
	    j++;
	 }
	 else {  
	   if ( (*(*i).Pagin()) < (*(*j).Pagin()) ) 
	     i++;
	    else
	      j++;
	 }  
      }
   }
   // condicion de error
   else {
      solucion.error        =  true ;
      solucion.permitir_com = false ;
   }
   return solucion ;
}

/*/ funcion para devolver el primer nodo de la lista /*/
Nodo Lista_datos::Front() {
  return list_nodo.front() ;
}

/*/ accion de ordenar de lista_datos /*/
void Lista_datos::Sort() {

  if( not error )
    list_nodo.sort() ;
}

/*/ para ordenar la lista_datos /*/
void ordenar_lista_y_medir(Lista_datos& l,void *&psizeof) {

  l.Sort();
  int * s = (int *)psizeof ;
  (*s)   += SizeofL(l) ;
  psizeof = ( void *)s ;
}

/*##########################################################*/
/*##############   IMPLEMENTACION DE TABLA   ###############*/
/*##########################################################*/

/*/ destructor /*/
Contenedor::~Contenedor() {

  cargado = false ;
  read_word   = 0 ;
  word_dist   = 0 ;
  web_leidas  = 0 ;
  tabla_pag.clear() ;
  tabla_palabras.clear() ;
  list_dat_total.Clear() ;
}

/*/ borrar el contenedor /*/
void Contenedor::Clear ( ) {

  cargado = false ;
  read_word   = 0 ;
  word_dist   = 0 ;
  web_leidas  = 0 ;
  tabla_pag.clear() ;
  tabla_palabras.clear() ;
  list_dat_total.Clear() ;
}

/*/ constructor controlado /*/
void Contenedor::Init  ( ){

  tabla_pag.init     (size_paginas ,f_hash2) ; 
  tabla_palabras.init(size_palabras,f_hash ) ;
  read_word    = 0 ;
  web_leidas   = 0 ;
  word_dist    = 0 ;
  cargado  = false ;
}

/*/ constructor controlado /*/
void Contenedor::Init  (int num_pag,int num_pal) {

  tabla_pag.init(num_pag,f_hash2) ; 
  tabla_palabras.init(num_pal,f_hash) ;
  read_word    = 0 ;
  web_leidas   = 0 ;
  word_dist    = 0 ;
  cargado  = false ;
}


/*/ tamañodel contenedor /*/
int  Contenedor::Size  ( ) {
  return  tabla_palabras.size() ;
}

/*/ lista total de paginas /*/
Lista_datos Contenedor::Lista_total_paginas( ) {
   return list_dat_total ;
}

/*/ funcion auxiliar /*/
void Contenedor::inserta_pagina(string& pal,Pagina* pag ) {

   list<string> *lista = tabla_pag.search( pag ) ;

   if ( lista ) {
     if ( (*lista).size() == 0 ) {
       (*lista).push_front(pal) ;
     }
     else {
       list<string>::iterator i = (*lista).begin() ;
       list<string>::iterator end = (*lista).end() ;
       list<string>::iterator j = (*lista).begin() ;
       
       if ( (*i) > pal ) {
	 (*lista).push_front(pal) ;
       }
       else {
	 i++;
	 while ( ( i != end ) and ( (*j) < pal ) ) {
	   i++ ;
	   j++ ;
	 }
	 if ( ( i == end ) and ( (*j) < pal ) ) {
	   (*lista).push_back(pal) ;
	 }
	 else {
	   (*lista).insert( i , pal) ;
	 }
       }   
     }
   }
   else {
     cerr << alert << "ERROR : en  inserta_pag "<< endl ;
     exit(1);
   }
}

/*/ funcion auxiliar /*/
void Contenedor::inserta_palabra(string& pal,Pagina* pag,int aparicion) {

   Lista_datos *lista = tabla_palabras.search(pal) ;

   if ( lista ) {
      // caso en el cual la palabra ya estaba 
      // y ya ha aparecido en esta pagina
      if ( lista->list_nodo.front().Pag == pag ) {

	 lista->list_nodo.front().l_aparic.push_back(aparicion) ;
      }
      // caso en el cual la palabra ya estaba
      // y aun no habia aparecido en esta pagina
      else {

	Nodo nod(pag,aparicion) ; 
	lista->list_nodo.push_front(nod) ;
	// ahora queda insertarlo en la tabla de paginas
	(*this).inserta_pagina(pal,pag) ;
      }	   
   }
   // caso en el cual la palabra no estaba
   else {
     word_dist++ ;
     Nodo nod(pag,aparicion) ;
     Lista_datos  list_nueva ;
     
     list_nueva.list_nodo.push_front(nod)  ;
     tabla_palabras.insert(pal,list_nueva) ;
     
     (*this).inserta_pagina(pal,pag) ;
   }
}

/*##########################################################*/
/*#####   IMPLEMENTACION DE  LAS FUNCIONES DE TABLA   ######*/
/*##########################################################*/

/*/ funcion or /*/
void Contenedor::Or(list<string>& lista,std::ostream& out) {

  if (cargado) {
    cronometro c ;
    list<string>::iterator j = lista.begin() ;
    list<string>::iterator end = lista.end() ;

    if ( j != end ) {
      c.iniciar() ;
      string     buscar ;
      Lista_datos L_aux ;
      Lista_datos P_aux ;
      Lista_datos I_aux ;

      buscar=minusculas( (*j).c_str() ) ;
      L_aux = (*this).Search( buscar ) ;
      j++ ;
      bool salir = false ;

      while ( ( j != end ) and ( not salir) ) {
	I_aux  = L_aux ;
	buscar = minusculas( (*j).c_str() ) ;
	P_aux  = (*this).Search( buscar ) ;
	L_aux  = OR( I_aux, P_aux ) ;
	j++ ;
	if (L_aux.Size() == web_leidas) {
	  salir = true ;
	}
      }
      c.parar();
      out << c.milisegundos() <<" ms"<< endl;
      if ( salir ) {
	list_dat_total.Show( out ) ;
      }
      else {
	L_aux.Show( out ) ;
      }
      out << endl ;
    }
    else
      cerr << alert 
	   <<"ERROR: faltan parametros en funcion OR consulte HELP OR"
	   << endl << endl ;
  }
  else {
    cerr << alert <<"ERROR: no se ha realizado ninguna carga" 
	 << endl  << endl ;
  } 
}

/*/ funcion not /*/
void Contenedor::Not(list<string>& lista,std::ostream& out) {
 
  if ( cargado ) {  
   list<string>::iterator j = lista.begin() ;
   list<string>::iterator end = lista.end() ;

   if ( j != end ) {
     cronometro c ;
     c.iniciar() ;
     string buscar;
     Lista_datos L_aux;
     Lista_datos P_aux;
     Lista_datos I_aux;
     
     buscar = minusculas( (*j).c_str( ) ) ;
     I_aux  = (*this).Search(buscar) ;
     P_aux  = list_dat_total ;
     L_aux  = XOR( I_aux , P_aux ) ;
     j++;
     bool salir = false ;
     while ( ( j != end ) and ( not salir ) ) {
       I_aux  = L_aux ;
       buscar = minusculas( (*j).c_str() ) ;
       P_aux  = (*this).Search( buscar ) ;
       L_aux  = OPN( I_aux , P_aux ) ;
       j++ ;
       if ( L_aux.Size() == 0 ) {
	 salir = true ; 
       }
     }
     c.parar();
     out << c.milisegundos() <<" ms"<< endl;
     L_aux.Show( out ) ;
     out << endl ;
   }
   else
     cerr << alert 
	  <<"ERROR: faltan parametros en funcion NOT" 
          <<" consulte HELP NOT" << endl << endl ;
  }
  else {
    cerr << alert <<"ERROR: no se ha realizado ninguna carga" 
	 << endl << endl ;
  } 
}

/*/ funcion comillas /*/
void Contenedor::Com(list<string>& lista,std::ostream& out) {
  
  if (cargado) { 
   list<string>::iterator j = lista.begin() ;
   list<string>::iterator end = lista.end() ;

   if ( j != end ) {
     
     cronometro c ;
     c.iniciar() ;
     string     buscar ;
     Lista_datos L_aux ;
     Lista_datos P_aux ;
     Lista_datos I_aux ;
     
     buscar = minusculas( (*j).c_str() ) ;
     L_aux  = (*this).Search( buscar ) ;
     j++ ;
      bool salir = false ;
      while ( ( j != end ) and ( not salir ) ) {
	I_aux  = L_aux ;
	buscar = minusculas( (*j).c_str() ) ;
	P_aux  = (*this).Search( buscar ) ;
	L_aux  = COM( I_aux, P_aux) ;
	j++ ;
	if ( L_aux.Size() == 0 ) {
	  salir = true ; 
	}
      }
      c.parar();
      out << c.milisegundos() <<" ms"<< endl;
      L_aux.Show( out ) ;
      out << endl ;
   }
   else
     cerr << alert 
	  <<"ERROR: faltan parametros en funcion COM consulte HELP COM" 
	  << endl << endl ;
  }
  else {
    cerr << alert <<"ERROR: no se ha realizado ninguna carga" 
	 << endl << endl ;
  }
}

/*/ funcion and /*/
void Contenedor::And(list<string>& lista,std::ostream& out) {
   
  if ( cargado ) {
    list<string>::iterator j = lista.begin() ;
    list<string>::iterator end = lista.end() ;
    
    if ( j != end ) {
      
      cronometro c ;
      c.iniciar() ;
      string     buscar ;
      Lista_datos L_aux ;
      Lista_datos P_aux ;
      Lista_datos I_aux ;
      
      buscar = minusculas( (*j).c_str() ) ;
      L_aux  = (*this).Search( buscar ) ;
      j++ ;
      bool salir = false ;
      while ( ( j != end )and ( not salir ) ) {
	 I_aux  = L_aux ;
	 buscar = minusculas( (*j).c_str() ) ;
	 P_aux  = (*this).Search( buscar ) ;
	 L_aux  = AND( I_aux, P_aux) ;
	 j++ ;
	 if ( L_aux.Size() == 0 ) {
	   salir = true ; 
	 }
      }
      c.parar();
      out << c.milisegundos() <<" ms"<< endl;
      L_aux.Show( out ) ;
      out << endl ;
   }
   else
     cerr << alert 
	  <<"ERROR: faltan parametros en funcion AND"
	  <<" consulte HELP AND"
	  << endl << endl ;
  }
  else {
    cerr << alert <<"ERROR: no se ha realizado ninguna carga" 
	 << endl << endl ;
  } 
}

/*/ buscar palabra /*/
Lista_datos Contenedor::Search( string& pal ) {

  Lista_datos *lista = tabla_palabras.search( pal ) ;
  
  if ( lista )
    return (*lista) ;
  else {
    lista = new(Lista_datos) ;
    lista->error = false ;
    lista->permitir_com = false ;
    return (*lista) ;
  }
}

/*/ insertar nueva web /*/
void Contenedor::inserta_nueva(string url,string nombre_fich_pagina,int relevancia) {
   
   std::ifstream fichero( nombre_fich_pagina.c_str() ) ;
   if ( fichero ) {
    
     web_leidas++ ;
     Pagina *pag = new Pagina(url,nombre_fich_pagina,relevancia,web_leidas) ;
     list<string> lista_nueva ;
      
     if ( not tabla_pag.insert(pag, lista_nueva ) ) { 
       cerr << alert 
	    <<" ERROR: en close_hash<T,key>::insert()" 
	    << endl ;
       exit(1);
     }
     Nodo _nodo(pag);
     list_dat_total.list_nodo.push_front(_nodo) ;
     int aparicion = 1 ;
     string      palabra ;
     palabra = lee (fichero) ;
     palabra = minusculas( palabra.c_str() ) ;
     while ( ( not fichero.eof() ) and  palabra != SALIR ) {
       read_word++ ;
       //cout <<"[" << palabra << "]" ;
       // para ver las palabras leidas 
       inserta_palabra(palabra,pag,aparicion) ;
       aparicion++ ;
       palabra = lee (fichero) ;
     }
   }
   else {
     cerr << alert
	  << " ERROR: no se pudo abrir el fichero: "
	  <<   nombre_fich_pagina    <<    endl    ;
   }
}

/*/ mostrar datos de la carga /*/
void Contenedor::mostrar_datos(cronometro c,int num_bytes) {
  
  cout << endl ;
  cout << tab << "Tiempo empleado en carga: "
       << "........." ;

  cout.width(9);
  cout.fill('.');
  cout << c.milisegundos() <<" ms"
       << endl << tab <<"HH:MM:SS.CC" 
       << ": ......................." ;
  c.mostrar();
  cout.fill('.');
  cout << endl << endl ;
  cout << tab << "Memoria usada: " 
       << "...................." ;
  cout.width(10);
  cout << num_bytes <<" B"
       << endl  << tab ;
  cout.width(3);
  cout.fill(' ');
  cout << ((num_bytes / 1048576)/1024) % 1024 
       << " GB       ";
  cout.width(4);
  cout << (num_bytes / 1048576)% 1024 
       << " MB       ";
  cout.width(4);
  cout << (num_bytes / 1024)% 1024
       << " KB       ";
  cout.width(4);
  cout << (num_bytes) % 1024 
       << " B" << endl << endl ;
  cout << tab << "Numero de palabras leidas: " 
       << "........" ;
  cout.width(12);
  cout.fill('.');
  cout << read_word << endl ;
  cout << tab << "Numero de palabras distintas: " 
       << ".....";
  cout.width(12);
  cout << word_dist << endl ;
  cout << tab << "Numero de paginas web: "  
       << "............";
  cout.width(12);
  cout << web_leidas << endl << endl ;
}

/*/ accion auxiliar para borrar una pagina /*/
void Contenedor::borrar_pagina(Pagina* pag) {

  list<string> *lista = tabla_pag.search(pag) ;
  if ( lista ) {
    list<string>::iterator i = (*lista).begin() ;
    list<string>::iterator end = (*lista).end() ;
    Lista_datos *l;
     
    while ( i != end ) {
      l = tabla_palabras.search(*i) ;
      if ( l ) {
	list<Nodo>::iterator j  =  (*l).list_nodo.begin() ;
	list<Nodo>::iterator j_end = (*l).list_nodo.end() ;

	while ( ( j != j_end ) and ( (*j).Pagin() != pag ) ) {
	  j++ ;
	}
	if ( j != j_end ) {
	  (*l).list_nodo.erase(j);
	}
	else {
	  cerr << alert <<"ERROR: durante la carga"
	       << endl << endl ;
	  exit(1);
	}
	if ( (*l).Size() == 0 ) {
	  tabla_palabras.erase(*i);
	}
      }
      else {
	cerr << alert <<"ERROR: durante la carga"
	     << endl << endl ;
	exit(1);
      }
      i++ ;
    }
    web_leidas-- ;
    (*lista).clear() ;
    tabla_pag.erase(pag) ;
  }
  else {
    cerr << alert 
	 <<"ERROR: fichero inexistente en la base de datos"
	 << endl << endl ;
  }
}

/*/ insert /*/
bool Contenedor::Insert(string url,string fich_pag,int relevancia) {

  read_word = 0 ;
  word_dist = 0 ;
  cronometro  c ;
  std::ifstream fichero( fich_pag.c_str() ) ;
  if ( fichero ) {
 
    c.iniciar() ;
    (*this).inserta_nueva(url,fich_pag,relevancia) ;
    int i = (*this).Sizeof_and_sort() ;
    Nodo nod = list_dat_total.list_nodo.front() ;
    list_dat_total.list_nodo.pop_front() ;
    
    list<Nodo>::iterator j = list_dat_total.list_nodo.begin() ;
    list<Nodo>::iterator end = list_dat_total.list_nodo.end() ;
    
    while ( ( j != end ) and ( (*j)< nod ) ) {
      j++ ;
    }
    if ( j == end ) {
      list_dat_total.list_nodo.push_back(nod) ;
    }
    else {
      list_dat_total.list_nodo.insert(j,nod) ;
    }
    c.parar() ;
    (*this).mostrar_datos(c,i) ;
    cargado = true ;
  }
  else {
    cerr << alert <<"ERROR: no existe el fichero: "
	 << fich_pag << endl << endl ;
  }
}

/*/ sizeof /*/
int  Contenedor::Sizeof_and_sort( ) {

 int * i= new (int) ;
 (*i) = 0 ;
 list_dat_total.Sort() ;
 tabla_palabras.for_each(ordenar_lista_y_medir,i) ;
 tabla_pag.for_each(medir_bytes,i);
 (*i) += sizeof(list_dat_total) ;
 (*i) += tabla_palabras.size_of() ;
 (*i) += tabla_pag.size_of() ;
 (*i) += sizeof(int) * 2 ;
 return (*i) ;
}

/*/ load /*/
bool Contenedor::Load(string& maest, list<string>& fich_maestro) {

  cronometro c;
  c.iniciar();
  if ( web_leidas > 0 ) {
    (*this).Clear() ;
    (*this).Init (10000,500000) ;
  }
  std::ifstream maestro( maest.c_str() ) ;
  string line_of_master ;
  if ( maestro ) {
    fich_maestro.clear() ;
     
    while (not maestro.eof() ) {
      getline( maestro, line_of_master) ;
      if((line_of_master[0] != '#')and(line_of_master[0]!=endstrg)){
	fich_maestro.push_back( line_of_master ) ;
      }
    }
    maestro.close() ;
    
    list<string>::iterator j = fich_maestro.begin() ;
    list<string>::iterator end = fich_maestro.end() ; 
    while ( j != end ) {
      
      istringstream linea_maestro(*j) ;
      if ( (*j)[0] != '#' and (*j)[0]!= endstrg ) {
	
	string url ;
	string nombre_fich_pagina ;
	int relevancia = 0 ;
	    
	linea_maestro >> url ;
	linea_maestro >> nombre_fich_pagina ;
	linea_maestro >> relevancia ;
	(*this).inserta_nueva(url,nombre_fich_pagina,relevancia) ;
      }
      j++;
    }
    fich_maestro.sort();
    c.parar() ;
    int num_bytes = (*this).Sizeof_and_sort() ;
    (*this).mostrar_datos(c,num_bytes) ;
    cargado = true ;
    return true ;
  }
  else {
    cargado = false ;
    return false ;
  }
}

/*/ borrar una pagina /*/
bool Contenedor::Erase (string& fichero_borrar) {

  list<Nodo>::iterator j = list_dat_total.list_nodo.begin() ;
  list<Nodo>::iterator end = list_dat_total.list_nodo.end() ;
  bool salir = false ;
  while ( ( j != end ) and  ( fichero_borrar != (*j).Fichero() ) ) {
 
    j++ ;
  }
  cout << " Erase" << endl ;
  if ( j != end ) {
    Nodo nod = (*j) ;
    Pagina *pag = nod.Pagin();
    fichero_borrar = (*pag).URL ;
    (*this).borrar_pagina(pag);
    list_dat_total.list_nodo.erase(j) ;
    delete (pag) ;
    return true ;
  }
  else {
    return false ;
  }
}

#endif
/*
  $Id: estructura_datos.cpp
  v 1.0 28/07/2004 18:59:10
  David Perez Cabrera
*/
