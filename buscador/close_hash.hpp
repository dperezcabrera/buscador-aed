/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/*@@@@   MODULO DE PLANTILLA DE CLOSE_HASH DEFINICION   @@@@*/
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/


#ifndef __CLOSE_HASH_HPP
#define __CLOSE_HASH_HPP

template<class T, class key>
class close_hash 
{
public:

   /*/ defino un tipo de funcion /*/
  typedef int(*funcion_hash)(const key) ;
  typedef void(accion)(T&,void*&);

private:

  /*/ elementos /*/
  int         *indices ; 
  int      *siguientes ;
  T        **elementos ;
  key         **claves ;
  int    gestor_vacios ;
  int    num_elementos ;
  int             tama ;

  /*/ campo de funcion como elemento /*/
  funcion_hash  f_hash ;

  /*/ funciones privadas /*/
  bool inserta   ( key*, T* )          ; // &
  bool pop_front (  )                  ; // &
  bool front     ( key*& ,T*& )        ; // &

  /*/ funciones solo para depuracion /*/
  void show     (  )                   ; // &
  void show     (std::ostream&)        ; // &

public:
  
  close_hash ( funcion_hash )                             ; // &
  close_hash (const int, funcion_hash )                   ; // &
  ~close_hash (  )                                        ; // &

  void init(const int , funcion_hash ) ; // &
  void for_each (accion,void*)         ; // &
  bool insert   (const key,const T& )  ; // &
  bool erase    (const key)            ; // &

  T*   search   (const key)            ; // &
  void clear    (  )                   ; // &
  int  size     (  )                   ; // &
  int  size_of  (  )                   ; // &

  void resize   ( const int )          ; // &
  bool asign    ( close_hash<T,key>&)  ; // &
} ;

#endif
/*
  $Id: close_hash.hpp
  v 1.0 16/07/2004 18:54:45
  David Perez Cabrera
*/
