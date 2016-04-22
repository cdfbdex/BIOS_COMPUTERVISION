#include "biosmabe/biosmabe.h"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;
using namespace biosmabe;

/// Global variables

int threshold_value = 0;
int threshold_type = 3;;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;

Mat src, src_gray, ImReferencia, dst;
char* window_name = "Threshold Demo";

char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
char* trackbar_value = "Value";
char* trackbar_cannymin = "Min. Canny";
char* trackbar_cannymax = "Max. Canny";

/// Function headers
void Threshold_Demo(int, void*);


int main(int argc, char *argv[]) {

	char *RutaFolderREFERENCIAFRONTAL = "../../dataset/FRONTAL/Prueba1/";
	char *RutaFolderPRUEBAFRONTAL = "../../dataset/FRONTAL/Prueba1/";

	char *NombreBaseFRONTALSUPERIOR_DEPTH = "KinectSuperior/framesDepth/FrameDepth_0.xml";

	char *RutaImagenReferencia = new char[200]();
	strcat(RutaImagenReferencia, RutaFolderREFERENCIAFRONTAL);
	strcat(RutaImagenReferencia, NombreBaseFRONTALSUPERIOR_DEPTH);
	std::cout << std::endl << std::endl << "Cargando: " << std::endl << RutaImagenReferencia << std::endl << "..." << std::endl;
	FileStorage NombreReferencia;
	NombreReferencia.open(RutaImagenReferencia, FileStorage::READ);
	if (!NombreReferencia.isOpened())
	{
		std::cout << "No se pudo cargar el archivo: " << RutaImagenReferencia << std::endl;
		return -1;
	}

	char *RutaImagenPrueba = new char[200]();
	strcat(RutaImagenPrueba, RutaFolderPRUEBAFRONTAL);
	strcat(RutaImagenPrueba, NombreBaseFRONTALSUPERIOR_DEPTH);
	std::cout << std::endl << std::endl << "Cargando: " << std::endl << RutaImagenPrueba << std::endl << "..." << std::endl;
	FileStorage NombrePrueba;
	NombrePrueba.open(RutaImagenPrueba, FileStorage::READ);
	if (!NombrePrueba.isOpened())
	{
		std::cout << "No se pudo cargar el archivo: " << RutaImagenPrueba << std::endl;
		return -1;
	}


	//Loading "mtx" variable to cv::Mat
	Mat ImReferenciaAux, ImPruebaAux;
	NombreReferencia["mtx"] >> ImReferenciaAux;
	NombrePrueba["mtx"] >> ImPruebaAux;


	double nDepthMaxReliableDistance = 4500.0, nDepthMinReliableDistance = 0.0;
	double scale = 255.0 / (nDepthMaxReliableDistance - nDepthMinReliableDistance);
	Mat ImColorMap, ImScale;
	ImReferenciaAux.convertTo(ImScale, CV_8UC1, scale);
	applyColorMap(ImScale, ImColorMap, cv::COLORMAP_JET);

	ImReferencia = biosmabe::rotateImage(ImScale, -90.0);
	



	/// Create a window to display results
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	/// Create Trackbar to choose type of Threshold
	createTrackbar(trackbar_type,
		window_name, &threshold_type,
		max_type, Threshold_Demo);

	createTrackbar(trackbar_value,
		window_name, &threshold_value,
		max_value, Threshold_Demo);

	/// Call the function to initialize
	Threshold_Demo(0, 0);

	/// Wait until user finishes program
	while (true)
	{
		int c;
		c = waitKey(20);
		if ((char)c == 27)
		{
			break;
		}
	}
	waitKey(0);
	delete[] RutaImagenReferencia;
	delete[] RutaImagenPrueba;
	return 0;
}


/**
* @function Threshold_Demo
*/
void Threshold_Demo(int, void*)
{
	/* 0: Binary
	1: Binary Inverted
	2: Threshold Truncated
	3: Threshold to Zero
	4: Threshold to Zero Inverted
	*/



	threshold(ImReferencia, dst, threshold_value, max_BINARY_value, threshold_type);
	Mat  cdst;
	Canny(dst, cdst, 50, 200, 3);
	//cvtColor(dst, cdst, CV_GRAY2BGR);
#if 1
	vector<Vec2f> lines;
	HoughLines(dst, lines, 1, CV_PI / 180, 100, 0, 0);

	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
	}
#else
	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180, 50, 50, 10);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
	}
#endif

	imshow(window_name, dst);
}