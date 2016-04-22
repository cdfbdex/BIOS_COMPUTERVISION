#include "MabeSegmentation.h"

//Implementación Clase MabeSegmentation
biosmabe::MabeSegmentation::MabeSegmentation(Mat Imagen){
	ImagenEntrada = Imagen.clone();
}

biosmabe::MabeSegmentation::~MabeSegmentation()
{
}

bool biosmabe::MabeSegmentation::Tecnica1(){

	if (!ImagenEntrada.data)
		return false;
	cvtColor(ImagenEntrada, ImagenSalida, CV_BGR2GRAY);
	return true;
}

bool biosmabe::MabeSegmentation::Tecnica2(){
	if (!ImagenEntrada.data)
		return false;
	cvtColor(ImagenEntrada, ImagenSalida, CV_BGR2HSV);
	return true;
}

Mat biosmabe::MabeSegmentation::getImagenSalida(){
	return ImagenSalida;
}

Mat biosmabe::MabeSegmentation::getImagenEntrada(){
	return ImagenEntrada;
}

//Fin Implementación Clase MabeSegmentation
