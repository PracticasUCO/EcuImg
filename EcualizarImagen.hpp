#ifndef __LIB_ECUALIZAR_IMAGEN_HPP
#define __LIB_ECUALIZAR_IMAGEN_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Histograma.hpp"
#include "HistogramaAcumulado.hpp"

using namespace FSIV;
using namespace cv;

namespace FSIV
{
  /** @brief Clase que realiza la ecualización de una imagen. **/
  class EcualizarImagen
  {
  private:
    cv::Mat _mask;
    cv::Size _size;
    unsigned int _canales;
    unsigned int _radio; //!< Radio final de la mascara de procesamiento

    Mat ecualizacionCanalSinVentana(const Mat &matriz, const Mat &mascara, HistogramaAcumulado &histo);

  public:
    /** @brief Constructor vacio. Inicializa la imagen con los parametros en blanco.**/
    EcualizarImagen(unsigned int radio = 0);
    
    /** @brief Inicializa la clase con la mascara **/
    EcualizarImagen(cv::Mat mascara, cv::Size size, unsigned int nCanales = 1, unsigned int radio = 0);

    /** @brief Devuelve el radio asignado
	@return Radio de la mascara. 
    **/
    unsigned int getRadio() const;

    /** @brief Actualiza el radio de la mascara
	@param radio Nuevo radio de la mascara.**/
    void setRadio(const unsigned int &radio);

    /** @brief Devuelve el tamaño de la mascara **/
    cv::Size getTamanioMascara() const;

    /** @brief Realiza la ecualización de la imagen
	@param imagen -->La imagen a ser ecualizada
	@pre Si se inicializo una mascara, deben de coincidir
	@return Una imagen ecualizada
    **/
    Mat ecualizar(const Mat &imagen);
  };
};

#endif
