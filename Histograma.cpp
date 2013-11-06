#include <vector>
#include <cassert>
#include <iostream>
#include <iomanip>
#include "Histograma.hpp"

using namespace std;

namespace FSIV
{
  Histograma::Histograma(const int &minimo, const int &maximo)
  {
    this->inicializar(minimo, maximo);
  }

  Histograma::Histograma(const Histograma &h)
  {
    *this = h;
  }

  void Histograma::inicializar(const int &minimo, const int &maximo)
  {
    assert(minimo < maximo);
    this->setMinimo(minimo);
    this->setMaximo(maximo);
  }

  void Histograma::setMinimo(const int &minimo)
  {
    _minimo = minimo;

    if(maximo < minimo)
      {
	this->setMaximo(minimo + 1);
      }
  }

  void Histograma::setMaximo(const int &maximo)
  {
    _maximo = maximo;

    if(minimo > maximo)
      {
	this->setMinimo(maximo - 1);
      }
  }

  void Histograma::setAmplitud(const unsigned int &amplitud)
  {
    _amplitud = amplitud;
  }

  void Histograma::dimensionarHistograma()
  {
  }

  void Histograma::setElemento(const unsigned int &canal, const unsigned int &posicion, const double &elemento)
  {
  }

  void Histograma::setElementoPlus(const unsigned int &canal, const unsigned int &posicion)
  {
  }

  int Histograma::getMinimo() const
  {
  }

  int Histograma::getMaximo() const
  {
  }

  unsigned int Histograma::getAmplitud() const
  {
  }

  unsigned int Histograma::getCanales() const
  {
  }

  vector<double> Histograma::getHistograma() const
  {
  }

  unsigned int Histograma::getTamanio() const
  {
  }

  double Histograma::getElemento(const unsigned int &canal, const unsigned int &x) const
  {
  }

  void Histograma::clear()
  {
  }

  void Histograma::normalizar()
  {
  }

  void Histograma::mostrarHistograma()
  {
  }

  unsigned short int Histograma::getAncho()
  {
    if(this->comprobarAncho())
      {
	unsigned int canales;
	unsigned int maximo = 0;
	unsigned int width = 1;
	
	canales = this->getCanales();
	
	for(unsigned int ch = 0; ch < canales; ch++)
	  {
	    for(unsigned int m = 0; m < this->getTamanioComprimido(); m++)
	      {
		if(this->getElemento(ch, m) > maximo)
		  {
		    maximo = this->getElemento(ch, m);
		  }
	      }
	  }
	
	if(maximo < 0)
	  {
	    maximo *= -1;
	  }
	
	while(maximo > 9)
	  {
	    maximo /= 10;
	    width++;
	  }
	
	if(width > _ancho)
	  {
	    _ancho = width;
	  }
	
	this->comprobarAncho(false);
      }

    return _ancho;
  }

  void Histograma::comprobarAncho(const bool &c)
  {
    _comprobarAncho = c;
  }

  bool Histograma::comprobarAncho() const
  {
    return _comprobarAncho;
  }

  ostream& operator <<(ostream &output, Histograma &h)
  {
  }

  double Histograma::operator [](const unsigned int &posicion)
  {
  }
};
