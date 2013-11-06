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
    this->dimensionarHistograma();
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

  void Histograma::dimensionarHistograma()
  {
    _histograma.resize(this->getMaximo() - this->getMinimo() + 1, 0);
  }

  void Histograma::setElemento(const unsigned int &posicion, const double &elemento)
  {
    assert(posicion < this->getTamanio());
    _histograma[posicion] = elemento;
  }

  void Histograma::setElementoPlus(const unsigned int &canal, const unsigned int &posicion)
  {
    assert(posicion < this->getTamanio());
    _histograma[posicion] = _histograma[posicion] + 1;
  }

  int Histograma::getMinimo() const
  {
    return _minimo;
  }

  int Histograma::getMaximo() const
  {
    return _maximo;
  }

  valarray<double> Histograma::getHistograma() const
  {
    return _histograma;
  }

  unsigned int Histograma::getTamanio() const
  {
    return static_cast<unsigned int>(this->getMaximo() - this->getMinimo() + 1);
  }

  double Histograma::getElemento(const unsigned int &x) const
  {
    assert(x < this->getTamanio());
    return _histograma[x];
  }

  void Histograma::clear()
  {
    _histograma = 0;
  }

  void Histograma::procesarDatos(const Mat &m, const Mat &mascara)
  {
    assert((mascara.data == NULL) || (mascara.size() == m.size()));
    assert((mascara.data == NULL) || (mascara.type() == m.type()));

    valarray<Mat> canalesMatriz;
    valarray<Mat> canalesMascara;

    split(m, canalesMatriz);
    split(mascara, canalesMascara);

    for(unsigned int ch = 0; ch < canalesMatriz.size(); ch++)
      {
	canalesMatriz[ch].converTo(canalesMatriz[ch], CV_8UC1);
       
	if(mascara.data != NULL)
	  {
	    canalesMascara[ch].converTo(canalesMatriz[ch], CV_8UC1);
	  }

	for(int i = 0; i < canalesMatriz[ch].size(); i++)
	  {
	    for(int j = 0; j < canalesMatriz[ch].size(); j++)
	      {
		if((mascara.data == NULL) || (canalesMascara[ch].at<unsigned char>(i, j) != 255))
		  {
		    unsigned int valorLeido = static_cast<unsigned int>(canalesMatriz[ch].at<unsigned int>(i, j));
		    this->setElementoPlus(valorLeido);
		  }
	      }
	  }
      }
  }

  void Histograma::normalizar()
  {
    _histograma /= _histograma.sum();
  }

  void Histograma::mostrar()
  {
    unsigned short int ancho = this->getAncho();
    for(unsigned int i = 0; i < _histograma.size(); i++)
      {
	cout << setw(ancho) <<_histograma[i];
      }
    cout << endl;

    for(unsigned int i = 0; i < _histograma.size(); i++)
      {
	cout << setw(ancho) << i;
      }
    cout << endl;
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
    h.mostrar();
    return output;
  }

  double Histograma::operator [](const unsigned int &posicion)
  {
    assert(posicion < this->getTamanio());
    return _histograma[posicion];
  }
};
