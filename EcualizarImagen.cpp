#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cassert>
#include <iostream> //Debug
#include "EcualizarImagen.hpp"
#include "Histograma.hpp"
#include "HistogramaAcumulado.hpp"

using namespace cv;
using namespace std; //Debug

namespace FSIV
{
  EcualizarImagen::EcualizarImagen()
  {
  }

  EcualizarImagen::EcualizarImagen(cv::Mat mascara, cv::Size size, unsigned int nCanales, unsigned int radio)
  {
  }

  unsigned int EcualizarImagen::getRadio() const
  {
    return _radio;
  }

  void EcualizarImagen::setRadio(const unsigned int &radio)
  {
    _radio = radio;
  }

  cv::Size EcualizarImagen::getTamanioMascara() const
  {
    return _size;
  }

  Mat EcualizarImagen::ecualizacionCanalSinVentana(const Mat &matriz, const Mat &mascara, HistogramaAcumulado &histo)
  {
    return cv::Mat();
  }

  Mat EcualizarImagen::ecualizar(const Mat &imagen)
  {
    return cv::Mat();
  }
}
