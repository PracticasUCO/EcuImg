#ifndef __LIB_HISTOGRAMA_FSIV_HPP
#define __LIB_HISTOGRAMA_FSIV_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace std;
using namespace cv;

namespace FSIV
{
  /** @class Histograma
      @brief Define la clase Histograma, que alamacena Histogramas de uso
             general
  **/
  class Histograma
  {
  protected:
    valarray<double> _histograma; //!< Vector del histograma
    int _minimo; //!< Valor más pequenio que almacenara el histograma
    int _maximo; //!< Valor mas grande que almacenara el histograma
    unsigned short int _ancho; //!< Ancho minimo de salida de cada numero estandar (para la funcion mostrarHistograma
    bool _comprobarAncho; //!< Comprueba que se deba de comprobar el ancho

    /** @brief Establece el valor más pequeño del histograma
	@param minimo. Valor minimo del histograma
     **/
    void setMinimo(const int &minimo);

    /** @brief Establece el valor maximo del histograma
	@param maximo. Valor maximo del histograma
     **/
    void setMaximo(const int &maximo);

    /** @brief Redimensiona el vector al tamaño adecuado
     **/
    void dimensionarHistograma();

    /** @brief Establece un valor a un determinado elemento **/
    void setElemento(const unsigned int &posicion, const double &elemento);

    /** @brief Incrementa el valor de una determinada posicion en uno **/
    void setElementoPlus(const unsigned int &posicion);

    /** @brief Devuelve el ancho que deben de tener todos los numeros al mostrar el histograma por pantalla **/
    unsigned short int getAncho();

    /** @brief Actualiza el estado de la bandera comprobar ancho **/
    void comprobarAncho(const bool &c);

    /** @brief Devuelve true si hay que comprobar el ancho **/
    bool comprobarAncho() const;

  public:
    /** @brief Constructor del Histograma
	@param minimo. Representa el valor minimo que tendra el histograma
	@param maximo. Representa el valor maximo que tendra el histograma.
	@param amplitud. Representa la el incremento necesario entre distintos valores del histograma
    **/
    Histograma(const int &minimo = 0, const int &maximo = 255);

    /** @brief Constructor de copia
	@param h. Histograma a copiar
    **/
    Histograma(const Histograma &h);

    /** @brief Permite la inicializacion o re-inicializacion de la clase **/
    void inicializar(const int &minimo = 0, const int &maximo = 255);

    /** @brief Devuelve el valor más pequenio del histograma
	@return Valor minimo del histograma
    **/
    int getMinimo() const;

    /** @brief Devuelve el valor más grande del histograma
	@return Valor maximo del histograma
    **/
    int getMaximo() const;

    /** @brief Devuelve el histograma, tal y como esta almacenado, sin ningun tipo de procesamiento extra.
	@return Histograma en crudo.
    **/
    vector<double> getHistograma() const;

    /** @brief Devuelve el tamaño del histograma
	@return Tamañio del histograma
    **/
    unsigned int getTamanio() const;

    /** @brief Devuelve el elemento n-esimo del histograma
     **/
    double getElemento(const unsigned int &canal, const unsigned int &x) const;

    /** @brief Borra el histograma
     **/
    void clear();

    /** @Procesa una matriz de elementos
	@param m. La matriz a procesar
	@pre Los valores de dicha matriz deben de estar entre el valor maximo y el minimo establecidos
    **/
    void procesarDatos(const Mat &m, const Mat &mascara);

    /** @Normaliza el programa, para que la suma de todos sus componentes valga uno **/
    void normalizar();

    /** @brief Muestra el vector de forma compacta **/
    void mostrar();

    /** Sobrecarga del operador << **/
    friend ostream& operator <<(ostream &output, Histograma &h);

    /** Sobrecarga del operador [] **/
    double operator [](const unsigned int &posicion); //No usar si se tiene más de un canal.

    /** Sobrecarga del operador = **/
    Histograma& operator =(const Histograma &h);

    inline ~Histograma()
    {
    }
  };
};
#endif
