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
    espacio = ESPACIO_COLOR_HSV;
  }

  bool radioFlag;
  bool imagenFlag;
  bool mascaraFlag;

  int radioValue;
  char * imagenPath;
  char * mascaraPath;

  enum espacioColor espacio;
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

  while((variable = getopt(argc, argv, "r:i:m:e:")) != -1)
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

	case 'e':
	  if((strstr(optarg, "HSV") != NULL) || (strstr(optarg, "hsv") != NULL))
	    {
	      opciones.espacio = ESPACIO_COLOR_HSV;
	    }
	  else if((strstr(optarg, "CIE") != NULL) || (strstr(optarg, "cie") != NULL))
	    {
	      opciones.espacio = ESPACIO_COLOR_CIE;
	    }
	  else if(strstr(optarg, "YCrCb") != NULL)
	    {
	      opciones.espacio = ESPACIO_COLOR_YCrCb;
	    }
	  else if((strstr(optarg, "HSL") != NULL) || (strstr(optarg, "hsl") != NULL))
	    {
	      opciones.espacio = ESPACIO_COLOR_HSL;
	    }
	  else
	    {
	      cerr << "Error" << endl;
	      cerr << "El parametro -e es incorrecto. Solo puede tomar los siguientes valores:" << endl;
	      cerr << "HSV o hsv --> Para hacer la transformacion en el espacio de color HSV (por defecto)" << endl;
	      cerr << "CIE o cie --> Para hacer la transformacion en el espacio de color CIE" << endl;
	      cerr << "YCrCb --> Para hacer la transformación en el espacio de color YCrCb" << endl;
	      cerr << "HSL --> Para hacer la transformacion en el espacio de color HSL" << endl;
	      exit(1);
	    }

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
      imagen = imread(opciones.imagenPath, CV_LOAD_IMAGE_UNCHANGED);
    }

  if(opciones.mascaraFlag == true)
    {
      mascara = imread(opciones.mascaraPath, CV_LOAD_IMAGE_UNCHANGED);
    }

  EcualizarImagen claseEcualizar(opciones.radioValue, opciones.espacio);
 
  resultado = claseEcualizar.ecualizar(imagen, mascara);

  parametrosSalida.push_back(CV_IMWRITE_PNG_COMPRESSION);
  parametrosSalida.push_back(9);

  cout << "Fichero salida: ";
  cin >> ficheroSalida;
  cout << endl;
  
  imwrite(ficheroSalida, resultado, parametrosSalida);

  return 0;
}
