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
    _size = Size(0, 0);
    _canales = 0;
    this->setRadio(0);
  }

  EcualizarImagen::EcualizarImagen(cv::Mat mascara, cv::Size size, unsigned int nCanales, unsigned int radio)
  {
    _mask = mascara.clone();
    _size = size;
    _canales = nCanales;
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

  cv::Size EcualizarImagen::getTamanioMascara() const
  {
    return _size;
  }

  Mat EcualizarImagen::ecualizacionCanalSinVentana(const Mat &matriz, const Mat &mascara, HistogramaAcumulado &histo)
  {
    Mat matrizFinal;
    matrizFinal.create(matriz.size(), matriz.type());

    matriz.convertTo(matrizFinal, CV_64FC1);

    for(int i = 0; i < matriz.rows; i++)
      {
	for(int j = 0; j < matriz.cols; j++)
	  {
	    if((mascara.data == NULL) || (mascara.at<double>(i, j) != 255))
	      {
		double valorLeido = matriz.at<double>(i, j);
		unsigned int traduccion = static_cast<unsigned int>(valorLeido);
		//cerr << "Dara violacion en: " << valorLeido << "-" << traduccion << endl;
		valorLeido = histo[traduccion] * histo.getMaximo();
		matrizFinal.at<double>(i, j) = valorLeido;
	      }
	  }
      }

    matrizFinal.convertTo(matrizFinal, CV_8UC1);

    return matrizFinal;
  }

  Mat EcualizarImagen::ecualizar(const Mat &imagen)
  {
    assert(_canales == static_cast<unsigned int>(imagen.channels()));
    assert(_size == imagen.size());

    Mat matrizTrabajo = imagen.clone();
    Mat matrizFinal;

    cerr << "Seguro? " << imagen.channels() << endl;

    matrizFinal.create(imagen.size(), imagen.type());

    if(this->getRadio() == 0)
      {
	

	//Ecualizar toda la imagen
	if(imagen.channels() == 1)
	  {
	    //Monocroma
	    HistogramaAcumulado histograma;
	    histograma.procesarDatos(matrizTrabajo, _mask);
	    histograma.normalizar();
	    matrizFinal = this->ecualizacionCanalSinVentana(matrizTrabajo, _mask, histograma);
	  }
	else
	  {
	    //Colorida Se pasa a HSV
	    vector<Mat> canalesMatriz;
	    HistogramaAcumulado histograma;

	    cvtColor(matrizTrabajo, matrizTrabajo, CV_BGR2HSV);

	    split(matrizTrabajo, canalesMatriz);

	    histograma.procesarDatos(canalesMatriz[2], _mask);
	    histograma.normalizar();

	    canalesMatriz[2] = this->ecualizacionCanalSinVentana(matrizTrabajo, _mask, histograma);
	    merge(canalesMatriz, matrizFinal);
	    cvtColor(matrizFinal, matrizFinal, CV_HSV2BGR);
	  }
      }
    else
      {
	//Ecualizar por ventanas
	if(matrizTrabajo.channels() == 1)
	  {
	    //Mono
	    for(int i = static_cast<int>(this->getRadio()); i < matrizTrabajo.rows - static_cast<int>(this->getRadio()); i++)
	      {
		for(int j = static_cast<int>(this->getRadio()); j < matrizTrabajo.cols - static_cast<int>(this->getRadio()); j++)
		  {
		    Mat ventana;
		    ventana.create(2*this->getRadio() + 1, 2*this->getRadio() + 1, matrizTrabajo.type());
		    Mat mascara;
		    mascara.create(2*this->getRadio() + 1, 2*this->getRadio() + 1, matrizTrabajo.type());

		    for(int k = 0; k < ventana.rows; k++)
		      {
			for(int h = 0; h < ventana.cols; h++)
			  {
			    ventana.at<unsigned char>(k, h) = matrizTrabajo.at<unsigned char>(k+i-this->getRadio(), h+j-this->getRadio());
			    
			    if(_mask.data != NULL)
			      {
				mascara.at<unsigned char>(k, h) = _mask.at<unsigned char>(k + i - this->getRadio(), h + j - this->getRadio());
			      }
			  }
		      }

		    HistogramaAcumulado histograma;
		    histograma.procesarDatos(ventana, mascara);
		    histograma.normalizar();
		    double pixel = static_cast<double>(ventana.at<unsigned char>(ventana.rows/2, ventana.cols/2));
		    pixel *= histograma[pixel] * histograma.getMaximo();
		    
		    matrizFinal.at<unsigned char>(i, j) = static_cast<unsigned char>(pixel);
		  }
	      }
	  }
	else
	  {
	    cvtColor(matrizTrabajo, matrizTrabajo, CV_BGR2HSV);
	    vector<Mat> canalesMatriz;

	    split(matrizTrabajo, canalesMatriz);
	    //Mono
	    for(int i = static_cast<int>(this->getRadio()); i < canalesMatriz[2].rows - static_cast<int>(this->getRadio()); i++)
	      {
		for(int j = static_cast<int>(this->getRadio()); j < canalesMatriz[2].cols - static_cast<int>(this->getRadio()); j++)
		  {
		    Mat ventana;
		    ventana.create(2*this->getRadio() + 1, 2*this->getRadio() + 1, canalesMatriz[2].type());
		    Mat mascara;
		    mascara.create(2*this->getRadio() + 1, 2*this->getRadio() + 1, canalesMatriz[2].type());

		    for(int k = 0; k < ventana.rows; k++)
		      {
			for(int h = 0; h < ventana.cols; h++)
			  {
			    ventana.at<unsigned char>(k, h) = canalesMatriz[2].at<unsigned char>(k+i-this->getRadio(), h+j-this->getRadio());
			    
			    if(_mask.data != NULL)
			      {
				mascara.at<unsigned char>(k, h) = _mask.at<unsigned char>(k + i - this->getRadio(), h + j - this->getRadio());
			      }
			  }
		      }

		    HistogramaAcumulado histograma;
		    histograma.procesarDatos(ventana, mascara);
		    histograma.normalizar();
		    double pixel = static_cast<double>(ventana.at<unsigned char>(ventana.rows/2, ventana.cols/2));
		    pixel *= histograma[pixel] * histograma.getMaximo();
		    
		    canalesMatriz[2].at<unsigned char>(i, j) = static_cast<unsigned char>(pixel);
		  }
	      }
	    merge(canalesMatriz, matrizFinal);
	    cvtColor(matrizFinal, matrizFinal, CV_HSV2BGR);
	  }
      }
    
    return matrizFinal;
  }
}
