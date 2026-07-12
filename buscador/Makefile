#   AED  - Curso  2003/2004 -  UM
#   David      Perez      Cabrera
#   Miguel Angel Orenes Fernandez 
#   Buscador   de   paginas   web
#   Version 1.0
#
#   profesor:
#            Norberto Marin Perez

# todo el programa es una sola unidad de traduccion
# (principal.cpp incluye los demas .cpp), de ahi que
# el binario dependa de todas las fuentes

CXX      = c++
CXXFLAGS = -O2 -Wall

Buscador: $(wildcard *.cpp *.hpp)
	$(CXX) $(CXXFLAGS) -o Buscador principal.cpp

test: $(wildcard *.cpp *.hpp)
	$(CXX) $(CXXFLAGS) -o test_buscador test.cpp
	./test_buscador

clean:
	rm -f Buscador test_buscador

.PHONY: clean test

#  $Id: makefile
#  v 1.0 30/07/2004 18:51:30
#  David Perez Cabrera
	  
