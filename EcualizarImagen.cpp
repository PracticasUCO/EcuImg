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
	    for(int i = this->getRadio(); i < imagen.rows - static_cast<int>(this->getRadio()); i++)
	      {
		for(int j = this->getRadio(); j < imagen.cols - static_cast<int>(this->getRadio()); j++)
		  {
		    Mat ventana(imagen, Rect(j - this->getRadio(), i - this->getRadio(), this->getRadio()*2 + 1, this->getRadio() * 2 + 1));
		    Mat ventanaMascara;
		    bool permitirPaso = true;

		    if(!mascara.empty())
		      {
			ventanaMascara = mascara(Rect(j - this->getRadio(), i - this->getRadio(), 2*this->getRadio() + 1, 2*this->getRadio() + 1));
			permitirPaso = mascara.at<unsigned char>(i, j) != 0;
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
		    else //Si no procesa el pixel, que copie al menos su valor original a la matriz de salida
		      {
			matrizFinal.at<unsigned char>(i, j) = imagen.at<unsigned char>(i, j);
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
	    histograma.clear();
	    histograma.procesarDatos(imagen, mascara);
	    histograma.normalizar();
	    
	    for(int i = 0; i < matrizFinal.rows; i++)
	      {
		for(int j = 0; j < matrizFinal.cols; j++)
		  {
		    if((mascara.empty()) || (mascara.at<unsigned char>(i, j) != 0))
		      {
			unsigned char valorLeido = imagen.at<unsigned char>(i, j);
			matrizFinal.at<unsigned char>(i, j) = static_cast<unsigned char>(histograma[valorLeido] * histograma.getMaximo());
		      }
		    else
		      {
			matrizFinal.at<unsigned char>(i, j) = imagen.at<unsigned char>(i, j);
		      }
		  }
	      }
	  }
      }
    else
      {
	Mat canalIluminacion(imagen.size(), CV_8UC1);
	vector<Mat> canales;
	const int ILUMINACION = 2;
	
	cvtColor(imagen, matrizFinal, CV_BGR2HSV);
	split(matrizFinal, canales);

	if(this->hayVentanas()) //Ecualizacion cromatica por ventanas
	  {
	    for(int i = this->getRadio(); i < canales[ILUMINACION].rows - static_cast<int>(this->getRadio()); i++)
	      {
		for(int j = this->getRadio(); i < canales[ILUMINACION].cols - static_cast<int>(this->getRadio()); j++)
		  {
		    int altura = (2*this->getRadio()) + 1;
		    Mat ventana(canales[ILUMINACION], Rect(j - this->getRadio(), i - this->getRadio(), altura, altura));
		    
		    Mat ventanaMascara;
		    bool permitirPaso = true;

		    if(!mascara.empty())
		      {
			ventanaMascara = mascara(Rect(j - this->getRadio(), i - this->getRadio(), altura, altura));
			permitirPaso = mascara.at<unsigned char>(i, j) != 0;
		      }

		    if(permitirPaso)
		      {
			unsigned char centroVentana = canales[ILUMINACION].at<unsigned char>(i, j);
			unsigned char nuevoValor;

			histograma.clear();
			histograma.procesarDatos(ventana, ventanaMascara);
			histograma.normalizar();
		    
			nuevoValor = static_cast<unsigned char>(histograma[centroVentana] * histograma.getMaximo());
			
			canalIluminacion.at<unsigned char>(i, j) = nuevoValor;
		      }
		    else //Si no procesa el pixel, que copie al menos su valor original a la matriz de salida
		      {
			canalIluminacion.at<unsigned char>(i, j) = imagen.at<unsigned char>(i, j);
		      }
		    ventana.release();
		  }
	      }
	    canales[ILUMINACION] = canalIluminacion.clone();
	    merge(canales, matrizFinal);
	  }
	else
	  {
	    histograma.clear();
	    histograma.procesarDatos(canales[ILUMINACION], mascara);
	    histograma.normalizar();

	    for(int i = 0; i < canalIluminacion.rows; i++)
	      {
		for(int j = 0; j < canalIluminacion.cols; j++)
		  {
		    if((mascara.empty()) || (mascara.at<unsigned char>(i, j) != 0))
		      {
			unsigned char valorLeido = canales[ILUMINACION].at<unsigned char>(i, j);
			canalIluminacion.at<unsigned char>(i, j) = static_cast<unsigned char>(histograma[valorLeido] * histograma.getMaximo());
		      }
		    else
		      {
			canalIluminacion.at<unsigned char>(i, j) = canales[ILUMINACION].at<unsigned char>(i, j);
		      }
		  }
	      }
	    canales[ILUMINACION] = canalIluminacion.clone();
	    merge(canales, matrizFinal);
	  }
	cvtColor(matrizFinal, matrizFinal, CV_HSV2BGR);
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
