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
  EcualizarImagen::EcualizarImagen(const unsigned int &radio)
  {
    this->setRadio(radio);
  }

  unsigned int EcualizarImagen::getRadio() const
  {
    return _radio;
  }

  void EcualizarImagen::setRadio(const unsigned int &radio)
  {
    _radio = radio;
  }

  bool EcualizarImagen::hayVentanas()
  {
    return this->getRadio() != 0;
  }

  Mat EcualizarImagen::ecualizar(const Mat &imagen, const Mat &mascara)
  {
    assert((mascara.data == NULL) || (mascara.size() == imagen.size()));
    assert((mascara.data == NULL) || (mascara.type() == imagen.type()));

    Mat matrizFinal(imagen.size(), imagen.type());
    static HistogramaAcumulado histograma;

    if(matrizFinal.channels() == 1) //Imagen monocroma
      {
	if(this->hayVentanas()) //Ecualizacion por ventanas
	  {
	    for(int i = this->getRadio(); i < matrizFinal.rows - static_cast<int>(this->getRadio()) - 1; i++)
	      {
		for(int j = this->getRadio(); j < matrizFinal.cols - static_cast<int>(this->getRadio()) - 1; j++)
		  {
		    Mat ventana(imagen, Rect(i, j, 2 * this->getRadio() + 1, 2 * this->getRadio() + 1));
		    Mat ventanaMascara;
		    bool permitirPaso = true;

		    if(mascara.data != NULL)
		      {
			ventanaMascara = mascara(Rect(i, j, 2 * this->getRadio() + 1, 2 * this->getRadio() + 1));
			permitirPaso = mascara.at<unsigned char>(i, j) != 255;
		      }

		    if(permitirPaso)
		      {
			unsigned char centroVentana = imagen.at<unsigned char>(i, j);
			unsigned char nuevoValor;

			histograma.clear();
			histograma.procesarDatos(ventana, ventanaMascara);
			histograma.normalizar();
		    
			nuevoValor = static_cast<unsigned char>(histograma[centroVentana] * histograma.getMaximo());
			
			matrizFinal.at<unsigned char>(i, j) = nuevoValor;
		      }
		    ventana.release();
		  }
	      }

	    //Cuando acaban los bucles la imagen no tiene los bordes, debido a que la ventana no
	    //pudo pasar por ellos. Con el rellenado de bordes nos quitamos el problema.
	    //No hago un imagen.clone al principio debido a que, aunque me solucionaria el problema
	    //ya que los bordes estaria copiados, copiaria un monton de datos que voy a cambiar
	    //justo despues.
	    this->rellenarBordes(imagen, matrizFinal);
	  }
	else
	  {
	    cout << "Aun no implementada la ecualizacion monocroma sin ventanas" << endl;
	  }
      }
    else
      {
	cout << "Aun no implementada la ecualizacion cromatica" << endl;
      }

    return matrizFinal;
  }

  void EcualizarImagen::rellenarBordes(const Mat &imagen, Mat &imagenFinal)
  {
    for(unsigned int i = 0; i < this->getRadio(); i++)
      {
	for(unsigned int j = 0; j < static_cast<unsigned int>(imagen.cols); j++)
	  {
	    imagenFinal.at<unsigned char>(i, j) = imagen.at<unsigned char>(i, j);
	  }
      }

    for(unsigned int i = static_cast<unsigned int>(imagen.rows) - this->getRadio(); i < static_cast<unsigned int>(imagen.rows); i++)
      {
	for(unsigned int j = 0; j < static_cast<unsigned int>(imagen.cols); j++)
	  {
	    imagenFinal.at<unsigned char>(i, j) = imagen.at<unsigned char>(i, j);
	  }
      }

    for(unsigned int i = this->getRadio(); i < static_cast<unsigned int>(imagen.rows) - this->getRadio(); i++)
      {
	for(unsigned int j = 0; j < static_cast<unsigned int>(this->getRadio()); j++)
	  {
	    imagenFinal.at<unsigned char>(i, j) = imagen.at<unsigned char>(i, j);
	  }
      }

    for(unsigned int i = this->getRadio(); i < static_cast<unsigned int>(imagen.rows) - this->getRadio(); i++)
      {
	for(unsigned int j = static_cast<unsigned int>(imagen.cols) - this->getRadio(); j < static_cast<unsigned int>(imagen.cols); j++)
	  {
	    imagenFinal.at<unsigned char>(i, j) = imagen.at<unsigned char>(i, j);
	  }
      }
  }
}
