#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <valarray>
#include "Histograma.hpp"
#include "HistogramaAcumulado.hpp"

using namespace FSIV;
using namespace std;
using namespace cv;

namespace FSIV
{
  void HistogramaAcumulado::procesarDatos(const Mat &m, const Mat &mascara)
  {
    assert((mascara.data == NULL) || (mascara.size() == m.size()));
    assert((mascara.data == NULL) || (mascara.type() == m.type()));
    Histograma::procesarDatos(m, mascara);

    double acumulado = 0;

    for(unsigned int i = 0; i < _histograma.size(); i++)
      {
	acumulado += this->getElemento(i);
	_histograma[i] = acumulado;
      }
  }

  void HistogramaAcumulado::normalizar()
  {
    _histograma /= _histograma[_histograma.size() - 1];
  }

  unsigned int HistogramaAcumulado::buscarValor(const double &v)
  {
    assert((v >= 0.0) && (v <= 1.0));
    for(unsigned int i = 0; i < this->getTamanio(); i++)
      {
	if(this->getElemento(i) >= v)
	  {
	    return i;
	  }
      }
    return this->getMaximo();
  }

  void HistogramaAcumulado::binormalizar()
  {
    unsigned int valorCentral = this->buscarValor(0.5);
    double vCentral = _histograma[valorCentral];

    for(unsigned int i = 0; i <= valorCentral; i++)
      {
	_histograma[i] = _histograma[i]/_histograma[valorCentral];
      }

    for(unsigned int i = valorCentral + 1; i < _histograma.size(); i++)
      {
	_histograma[i] = (_histograma[i] - vCentral) / (_histograma[_histograma.size() - 1] - vCentral);
      }
  }
};
