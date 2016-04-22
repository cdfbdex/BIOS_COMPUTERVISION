#include "biosmabe/biosmabe.h"
using namespace std;
using namespace cv;
using namespace biosmabe;

int main(int argc, char *argv[]) {
  
  
  /* TESTING BIOSMABE LIB */

  // Testing Independent Functions
  biosmabe::foo(); 

  //Testing Class in Separate File
  BaseClass O1(1999, 4, 5);
  std::cout << "Year = " << O1.getYear() <<std::endl; 

  // Testing Classes
  MabeSegmentation I1(imread("Prueba.jpg")); 
  MabeROIDetection I2(imread("Prueba.jpg"));
  MabeFeatureComparison I3(imread("Prueba.jpg"));
  Mat RotatedImage;

  bool resultado1 = I1.Tecnica1();
  bool resultado2 = I2.Tecnica1();
  bool resultado3 = I3.Tecnica1();
  RotatedImage = biosmabe::rotateImage(I1.getImagenSalida(), 60.0);

  imshow("Modulo 1: MabeSegmentation Salida", I1.getImagenSalida());
  imshow("Modulo 2: MabeROIDetection Salida", I2.getImagenSalida());
  imshow("Modulo 3: MabeFeatureComparison Salida", I3.getImagenSalida());
  imshow("Rotated Image", RotatedImage);
  

  waitKey(0);

  return 0;
}
