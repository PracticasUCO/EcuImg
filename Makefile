.PHONY: clean-objects clean-all

CXX = g++
INCLUDE = -I.
OPENCV = `pkg-config --cflags --libs opencv`
GTK_FLAGS = `pkg-config --cflags --libs gtk+-3.0`
CXXFLAGS = $(INCLUDE) -Wall -O3 -std=c++11 -lm $(OPENCV) $(GTK_FLAGS)

ecuImg: EcualizarImagen.o HistogramaAcumulado.o Histograma.o main.o
	$(CXX) $(CXXFLAGS) -o $@ $^

EcualizarImagen.o: EcualizarImagen.cpp EcualizarImagen.hpp Histograma.o HistogramaAcumulado.o
	$(CXX) $(CXXFLAGS) -c -o $@ EcualizarImagen.cpp

HistogramaAcumulado.o: HistogramaAcumulado.cpp Histograma.o HistogramaAcumulado.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ HistogramaAcumulado.cpp

Histograma.o: Histograma.cpp Histograma.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ Histograma.cpp

main.o: main.cpp EcualizarImagen.o Histograma.o HistogramaAcumulado.o
	$(CXX) $(CXXFLAGS) -c -o $@ main.cpp

gui.o: gui.c gui.h
	$(CXX) $(CXXFLAGS) -c -o $@ gui.c

clean-objects:
	-rm *.o

clean-all: clean-objects
	-rm ecuImg
