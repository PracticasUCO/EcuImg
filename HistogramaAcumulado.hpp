#ifndef __LIB_HISTOGRAMA_ACUMULADO_HPP
#define __LIB_HISTOGRAMA_ACUMULADO_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <valarray>
#include "Histograma.hpp"

using namespace FSIV;

namespace FSIV
{
  class HistogramaAcumulado : public Histograma
  {
  public:
    /** @brief Constructor de clase **/
    inline HistogramaAcumulado(const int &minimo = 0, const int &maximo = 255) : Histograma(minimo, maximo)
    {
    }

    /** @brief Constructor de copia **/
    inline HistogramaAcumulado(const HistogramaAcumulado &h) : Histograma(h)
    {
    }

    /** @brief Procesa los datos **/
    void procesarDatos(const Mat &m, const Mat &mascara = cv::Mat());

    /** @brief Busca el indice que coincida con un determinador valor **/
    unsigned int buscarValor(const double &v);

    /** @brief Normaliza el histograma **/
    void normalizar();

    inline ~HistogramaAcumulado()
    {
    }
  };
};

#endif
