#include "biosmabe/biosmabe.h"
using namespace std;
using namespace cv;
using namespace biosmabe;

int main(int argc, char *argv[]) {
  
	char *RutaFolderREFERENCIAFRONTAL = "../../dataset/FRONTAL/Prueba1/";
	char *RutaFolderREFERENCIADIAGONAL = "../../dataset/DIAGONAL/Prueba1/";
	char *RutaFolderPRUEBAFRONTAL = "../../dataset/FRONTAL/Prueba14/";
	char *RutaFolderPRUEBADIAGONAL = "../../dataset/DIAGONAL/Prueba2";

	if (argc < 5)
	{
		std::cout << std::endl << "Cargando DATASET de Referencia y Prueba por defecto";
	}
	else{
		RutaFolderREFERENCIAFRONTAL = argv[1];
		RutaFolderREFERENCIADIAGONAL = argv[2];
		RutaFolderPRUEBAFRONTAL = argv[3];
		RutaFolderPRUEBADIAGONAL = argv[4];
	}


	/*************************************************************************************************/
	//FRONTAL REFERENCIA
	//SUPERIOR
	char *NombreBaseFRONTALSUPERIOR_RGB = "KinectSuperior/framesRGB/FrameRGB_0.xml";
	char *NombreBaseFRONTALSUPERIOR_DEPTH = "KinectSuperior/framesDepth/FrameDepth_0.xml";
	char *NombreBaseFRONTALSUPERIOR_IR = "KinectSuperior/framesIR/FrameIR_0.xml";
	char *NombreBaseFRONTALINFERIOR_RGB = "KinectInferior/framesRGB/FrameRGB_0.xml";
	char *NombreBaseFRONTALINFERIOR_DEPTH = "KinectInferior/framesDepth/FrameDepth_0.xml";
	char *NombreBaseFRONTALINFERIOR_IR = "KinectInferior/framesIR/FrameIR_0.xml";
	//INFERIOR
	char *NombreBaseDIAGONALSUPERIOR_RGB = "KinectSuperior/framesRGB/FrameRGB_0.xml";
	char *NombreBaseDIAGONALSUPERIOR_DEPTH = "KinectSuperior/framesDepth/FrameDepth_0.xml";
	char *NombreBaseDIAGONALSUPERIOR_IR = "KinectSuperior/framesIR/FrameIR_0.xml";
	char *NombreBaseDIAGONALINFERIOR_RGB = "KinectInferior/framesRGB/FrameRGB_0.xml";
	char *NombreBaseDIAGONALINFERIOR_DEPTH = "KinectInferior/framesDepth/FrameDepth_0.xml";
	char *NombreBaseDIAGONALINFERIOR_IR = "KinectInferior/framesIR/FrameIR_0.xml";
	/*************************************************************************************************/



	char *RutaImagenReferenciaFRONTALSUPERIORRGB = new char[200]();
	strcat(RutaImagenReferenciaFRONTALSUPERIORRGB, RutaFolderREFERENCIAFRONTAL);
	strcat(RutaImagenReferenciaFRONTALSUPERIORRGB, NombreBaseFRONTALSUPERIOR_DEPTH);
	std::cout << std::endl << std::endl << "Cargando: " << std::endl << RutaImagenReferenciaFRONTALSUPERIORRGB << std::endl << "..." << std::endl;
	
	FileStorage NombreImagenReferenciaFRONTALSUPERIORXML;
	NombreImagenReferenciaFRONTALSUPERIORXML.open(RutaImagenReferenciaFRONTALSUPERIORRGB, FileStorage::READ);
	if (!NombreImagenReferenciaFRONTALSUPERIORXML.isOpened())
	{
		std::cout << "No se pudo cargar el archivo: " << RutaImagenReferenciaFRONTALSUPERIORRGB << std::endl;
		return -1;
	}

	char *RutaImagenPruebaFRONTALSUPERIORRGB = new char[200]();
	strcat(RutaImagenPruebaFRONTALSUPERIORRGB, RutaFolderPRUEBAFRONTAL);
	strcat(RutaImagenPruebaFRONTALSUPERIORRGB, NombreBaseFRONTALSUPERIOR_DEPTH);
	std::cout << std::endl << std::endl << "Cargando: " << std::endl << RutaImagenPruebaFRONTALSUPERIORRGB << std::endl << "..." << std::endl;
	FileStorage NombreImagenPruebaFRONTALSUPERIORXML;
	NombreImagenPruebaFRONTALSUPERIORXML.open(RutaImagenPruebaFRONTALSUPERIORRGB, FileStorage::READ);
	if (!NombreImagenPruebaFRONTALSUPERIORXML.isOpened())
	{
		std::cout << "No se pudo cargar el archivo: " << RutaImagenPruebaFRONTALSUPERIORRGB << std::endl;
		return -1;
	}


	//Loading "mtx" variable to cv::Mat
	Mat ImagenReferenciaFrontalRGBSuperior;
	NombreImagenReferenciaFRONTALSUPERIORXML["mtx"] >> ImagenReferenciaFrontalRGBSuperior;
	Mat ImagenPruebaFrontalRGBSuperior;
	NombreImagenReferenciaFRONTALSUPERIORXML["mtx"] >> ImagenPruebaFrontalRGBSuperior;
	Mat ir_image1 = cv::Mat::zeros(ImagenReferenciaFrontalRGBSuperior.rows, ImagenReferenciaFrontalRGBSuperior.cols, CV_8UC1);
	ImagenReferenciaFrontalRGBSuperior.convertTo(ir_image1, CV_8UC1, 1.0f);
	Mat ir_image2 = cv::Mat::zeros(ImagenReferenciaFrontalRGBSuperior.rows, ImagenReferenciaFrontalRGBSuperior.cols, CV_8UC1);
	ImagenPruebaFrontalRGBSuperior.convertTo(ir_image2, CV_8UC1, 1.0f);

	//Fin Loading "mtx" variable to cv::Mat

	//Verifying Data integrity
	if (!ImagenReferenciaFrontalRGBSuperior.data || !ImagenPruebaFrontalRGBSuperior.data)
	{
		printf("No hay datos de Imagen \n");
		return -1;
	}

	//Rotating Input Images
	Mat ReferenciaRotated = biosmabe::rotateImage(ir_image1, -90.0);
	Mat PruebaRotated = biosmabe::rotateImage(ir_image2, -90.0);
	
	
	//Processing and Analysing
	Mat croppedImage;
	cv::Rect ROI(30, 30, 300, 300);
	ReferenciaRotated(ROI).copyTo(croppedImage);
	MabeFeatureComparison ModuleFC(croppedImage, PruebaRotated);
	Mat IMatches;
	if (ModuleFC.FlannMatcher())
	{
		IMatches = ModuleFC.getImageMatches();
		std::cout << "Comparing Features Succesfull." << std::endl;
		namedWindow("Matches", WINDOW_NORMAL);
		imshow("Matches", IMatches);
	}else{
		std::cout << "Could not Compare Features. Check your Images." << std::endl;
	}

	//Visualizing Input Images
	namedWindow("Imagen Referencia Frontal RGB Superior", WINDOW_NORMAL);
	namedWindow("Imagen Prueba Frontal RGB Superior", WINDOW_NORMAL);
	imshow("Imagen Referencia Frontal RGB Superior", ReferenciaRotated);
	imshow("Imagen Prueba Frontal RGB Superior", PruebaRotated);

	waitKey(0);
	delete[] RutaImagenReferenciaFRONTALSUPERIORRGB;
	delete[] RutaImagenPruebaFRONTALSUPERIORRGB;
	return 0;
}
