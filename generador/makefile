# AED - Curso 2003/2004 - Gines Garcia Mateos - UM
# Generador de paginas web
# Version 0.0

generador: main.o generador.o
	c++ -o generador main.o generador.o

main.o: main.cpp generador.hpp conjunto.hpp bolsa.hpp
	c++ -c main.cpp

generador.o: generador.cpp generador.hpp conjunto.hpp bolsa.hpp
	c++ -c generador.cpp
