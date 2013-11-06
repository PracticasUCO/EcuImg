#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
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

  bool HistogramaAcumulado::getPorcentaje(const unsigned int &canal, unsigned int &perc) const
  {
  }
};
