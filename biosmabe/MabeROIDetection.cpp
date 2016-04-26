#include "MabeROIDetection.h"

//Implementación Clase MabeROIDetection
biosmabe::MabeROIDetection::MabeROIDetection(cv::Mat Imagen){
	ImagenEntrada = Imagen.clone();
}

biosmabe::MabeROIDetection::~MabeROIDetection()
{

}

bool biosmabe::MabeROIDetection::Tecnica1(){

	if (!ImagenEntrada.data)
		return false;
	cvtColor(ImagenEntrada, ImagenSalida, CV_BGR2GRAY);
	return true;
}

bool biosmabe::MabeROIDetection::Tecnica2(){
	if (!ImagenEntrada.data)
		return false;
	cvtColor(ImagenEntrada, ImagenSalida, CV_BGR2HSV);
	return true;
}

cv::Mat biosmabe::MabeROIDetection::getImagenSalida(){
	return ImagenSalida;
}

cv::Mat biosmabe::MabeROIDetection::getImagenEntrada(){
	return ImagenEntrada;
}

//Fin Implementación Clase MabeROIDetection