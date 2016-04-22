#ifndef BIOSMABE_BIOSMABE_H
#define BIOSMABE_BIOSMABE_H


// Check windows
#ifdef _WIN32 || _WIN64
    #if _WIN64
    #define WINENVIRONMENT64
    #else
    #define WINENVIRONMENT32
    #endif
#endif

// Check GCC
#if __GNUC__
	#if __x86_64__ || __ppc64__
	#define ENVIRONMENTGNU64
	#else
	#define ENVIRONMENTGNU32
	#endif
#endif


#ifdef _WIN32 || _WIN64
#  define EXPORTIT __declspec( dllexport )
#else
#  define EXPORTIT
#endif


#include <biosmabe/version.h>
#include "opencv2/opencv.hpp"
#include "opencv2/opencv_modules.hpp"
# include "opencv2/nonfree/features2d.hpp"
#include <string>
#include <vector>
#include <map>
#include <math.h>

// Additional classes
#include <biosmabe/baseclass.h>

using namespace cv;
using namespace std;

namespace biosmabe{
//#if defined(WINENVIRONMENT32) || defined(WINENVIRONMENT64)

	struct EXPORTIT Equ{
		double m;
		double c;
		bool isVertical;
		bool isHorizontal;
	};


	EXPORTIT void foo(void);
	EXPORTIT double round(double number);
	EXPORTIT double degreeToRadian(double degree);
	EXPORTIT double radianToDegree(double radian);
	EXPORTIT Size rotationNewCanvasSize(double degree, double angle, double h);
	EXPORTIT double solveEquationY(Equ e, double x);
	EXPORTIT double solveEquationX(Equ e, double y);
	EXPORTIT map<string, int> rotationExtraMargins(Size &original, Size &newSize);
	EXPORTIT map<string, Point> getCorners(Size &original, map<string, int> &margins);
	EXPORTIT map<string, Point> getProjectedCorners(Size &s, double h, double degree, double angle);
	EXPORTIT Point getCentreBetweenPoints(Point &a, Point &b);
	EXPORTIT map<string, Point> getCentreBetweenOriginalsAndProjections(map<string, Point> &originals, map<string, Point> &projections);
	EXPORTIT Equ getLinearEquation(Point &a, Point &b);
	EXPORTIT Equ getPerpendicular(Equ e, Point p);
	EXPORTIT map<string, Equ> getLinearEquationBetweenOriginalsAndProjections(map<string, Point> &originals, map<string, Point> &projections);
	EXPORTIT map<string, Equ> getPerpendicularLinearEquation(map<string, Point> &originals, map<string, Point> &projections, map<string, Point> &centre);
	EXPORTIT Point getColisionPoint(Equ e1, Equ e2);
	EXPORTIT Mat rotateImage(Mat image, double degree);



	class EXPORTIT MabeSegmentation
	{

	public:
		MabeSegmentation(Mat);
		bool Tecnica1();
		bool Tecnica2();
		Mat getImagenSalida();
		Mat getImagenEntrada();
		~MabeSegmentation();
	private:
		Mat ImagenEntrada;
		Mat ImagenSalida;
	};

	class EXPORTIT MabeROIDetection
	{

	public:
		MabeROIDetection(Mat);
		bool Tecnica1();
		bool Tecnica2();
		Mat getImagenSalida();
		Mat getImagenEntrada();
		~MabeROIDetection();
	private:
		Mat ImagenEntrada;
		Mat ImagenSalida;

	};

	class EXPORTIT MabeFeatureComparison
	{

	public:
		MabeFeatureComparison(Mat);
		MabeFeatureComparison(Mat, Mat);
		bool Tecnica1();
		bool Tecnica2();
		bool FlannMatcher();
		Mat getImageMatches();
		Mat getImagenSalida();
		Mat getImagenEntrada();
		~MabeFeatureComparison();
	private:
		Mat ImagenEntrada;
		Mat ImagenSalida;

		Mat Image_Matches;
		std::vector<KeyPoint> keypoints_1, keypoints_2;
		Mat Image_Matches1, Image_Matches2;
		std::vector< DMatch > good_matches;
	};
}


#endif
