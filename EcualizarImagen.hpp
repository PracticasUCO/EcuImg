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
  enum espacioColor { ESPACIO_COLOR_HSV, ESPACIO_COLOR_CIE, ESPACIO_COLOR_YCrCb, ESPACIO_COLOR_HSL};

  /** @brief Clase que realiza la ecualización de una imagen. **/
  class EcualizarImagen
  {
  private:
    unsigned int _radio; //!< Radio de la ventana que ecualizara la imagen

    /** @brief Rellena los bordes de una imagen **/
    void rellenarBordes(const Mat &imagen, Mat &imagenFinal);

    /** @brief Ecualiza el pixel central de la ventana, teniendo en cuenta todos los pixeles
	de alrededor, siempre y cuando la mascara se lo permita **/
    void ecualizarVentana(const Mat &ventana, Mat &ventanaSalida, const Mat &mascara = cv::Mat());

    /** @brief Ecualiza la imagen entera **/
    void ecualizarImagen(Mat &imagen, const Mat &mascara = cv::Mat());

    /** @brief Convierte la imagen al espacio de color especificado **/
    void cambiarEspacioColor(Mat &imagen, const enum espacioColor &espacio);

  public:
    /** @brief Constructor vacio. Inicializa la imagen con los parametros en blanco.**/
    EcualizarImagen(const unsigned int &radio = 0);

    /** @brief Devuelve el radio asignado
	@return Radio de la mascara. 
    **/
    unsigned int getRadio() const;

    /** @brief Actualiza el radio de la mascara
	@param radio Nuevo radio de la mascara.**/
    void setRadio(const unsigned int &radio);

    /** @brief Indica si es posible la ecualizacion por ventanas
	@return TRUE si es posible ecualizar por ventanas, y FALSE en caso contrario
    **/
    bool hayVentanas();

    /** @brief Realiza la ecualización de la imagen
	@param imagen -->La imagen a ser ecualizada
	@pre Si se inicializo una mascara, deben de coincidir
	@return Una imagen ecualizada
    **/
    Mat ecualizar(const Mat &imagen, const Mat &mascara = cv::Mat());
  };
};

#endif
