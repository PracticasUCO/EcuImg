#include <gtk/gtk.h>
#include "EcualizarImagen.hpp"

void startGUI(int argc, char ** argv)
{
  GtkWidget * mainWindow = NULL; //!< Creates the main window
  GtkWidget * mainLayout = NULL; //!< Creates the window's layout. It will be a Gtk Grid
  GtkWidget * HSLRadioButton = NULL; //!< Creates a radio button to select HSL space color
  GtkWidget * HSVRadioButton = NULL; //!< Creates a radio button to select HSV space color
  GtkWidget * inputImageSelector = NULL; //!< Creates a widget that let you select your input image
  GtkWidget * outputImageSelector = NULL; //!< Creates a widget that let you select your output image
  GtkWidget * closeButton = NULL; //!< Creates a simple close button, that ends with the program
  GtkWidget * acceptButton = NULL; //!< Creates a simple accept button, that start program works
  GtkWidget * helpButton = NULL; //!< Creates a simple help button, that show a windows with some basic help
  GtkWidget * authorsButton = NULL; //!< Creates an about button. It will show a about dialog
  GtkWidget * selectSpaceColorLabel = NULL;
  GtkWidget * inputImageLabel = NULL;
  GtkWidget * outputImageLabel = NULL;
}
