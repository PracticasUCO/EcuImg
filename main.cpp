#include <iostream>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "EcualizarImagen.hpp"
#include "HistogramaAcumulado.hpp"

using namespace std;
using namespace cv;
using namespace FSIV;

struct opcionesPrograma
{
  inline opcionesPrograma()
  {
    radioFlag = false;
    imagenFlag = false;
    mascaraFlag = false;
    radioValue = 0;
    imagenPath = NULL;
    mascaraPath = NULL;
  }

  bool radioFlag;
  bool imagenFlag;
  bool mascaraFlag;

  int radioValue;
  char * imagenPath;
  char * mascaraPath;
};

int main(int argc, char ** argv)
{
  struct opcionesPrograma opciones;
  Mat imagen;
  Mat mascara;
  Mat resultado;

  string ficheroSalida;
  vector<int> parametrosSalida;

  int variable;

  while((variable = getopt(argc, argv, "r:i:m:")) != -1)
    {
      switch(variable)
	{
	case 'r':
	  opciones.radioFlag = true;
	  opciones.radioValue = atoi(optarg);
	  break;

	case 'i':
	  opciones.imagenFlag = true;
	  opciones.imagenPath = optarg;
	  break;

	case 'm':
	  opciones.mascaraFlag = true;
	  opciones.mascaraPath = optarg;
	  break;

	case '?':
	  if(optopt == 'r')
	    {
	      cerr << "La opcion r requiere un parametro entero" << endl;
	      exit(1);
	    }
	  else if(optopt == 'i')
	    {
	      cerr << "La opcion i requiere un parametro" << endl;
	      exit(1);
	    }
	  else if(optopt == 'm')
	    {
	      cerr << "La opcion m requiere un parametro" << endl;
	      exit(1);
	    }
	  else if(isprint(optopt))
	    {
	      cerr << "Se desconoce el parametro " << optopt << endl;
	      exit(1);
	    }
	  else
	    {
	      cerr << "Parametro desconocido no imprimible" << endl;
	      exit(1);
	    }
	  break;
	}
    };

  if(opciones.imagenFlag == false)
    {
      cerr << "Es obligatorio especificar el path de la imagen" << endl;
      cerr << "Use -i <path> " << endl;
      exit(1);
    }
  else
    {
      imagen = imread(opciones.imagenPath, CV_LOAD_IMAGE_GRAYSCALE);
    }

  if(opciones.mascaraFlag == true)
    {
      mascara = imread(opciones.mascaraPath, CV_LOAD_IMAGE_UNCHANGED);
    }

  EcualizarImagen claseEcualizar(mascara, imagen.size(), imagen.channels(), opciones.radioValue);
 
  resultado = claseEcualizar.ecualizar(imagen);

  parametrosSalida.push_back(CV_IMWRITE_PNG_COMPRESSION);
  parametrosSalida.push_back(9);

  cout << "Fichero salida: ";
  cin >> ficheroSalida;
  cout << endl;
  
  imwrite(ficheroSalida, resultado, parametrosSalida);

  return 0;
}
