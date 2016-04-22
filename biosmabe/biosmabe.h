#ifndef BIOSMABE_BIOSMABE_H
#define BIOSMABE_BIOSMABE_H

// Check windows
#if _WIN32 || _WIN64
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

#include <biosmabe/version.h>
#include <biosmabe/baseclass.h>
#include "opencv2/opencv.hpp"
#include "opencv2/opencv_modules.hpp"
# include "opencv2/nonfree/features2d.hpp"
#include <string>
#include <vector>
#include <map>
#include <math.h>

using namespace cv;
using namespace std;

namespace biosmabe{
#if defined(WINENVIRONMENT32) || defined(WINENVIRONMENT64)
	
	struct __declspec(dllimport) Equ{
		double m;
		double c;
		bool isVertical;
		bool isHorizontal;
	};


	__declspec(dllimport) void foo(void);
	__declspec(dllimport) double round(double number);
	__declspec(dllimport) double degreeToRadian(double degree);
	__declspec(dllimport) double radianToDegree(double radian);
	__declspec(dllimport) Size rotationNewCanvasSize(double degree, double angle, double h);
	__declspec(dllimport) double solveEquationY(Equ e, double x);
	__declspec(dllimport) double solveEquationX(Equ e, double y);
	__declspec(dllimport) map<string, int> rotationExtraMargins(Size &original, Size &newSize);
	__declspec(dllimport) map<string, Point> getCorners(Size &original, map<string, int> &margins);
	__declspec(dllimport) map<string, Point> getProjectedCorners(Size &s, double h, double degree, double angle);
	__declspec(dllimport) Point getCentreBetweenPoints(Point &a, Point &b);
	__declspec(dllimport) map<string, Point> getCentreBetweenOriginalsAndProjections(map<string, Point> &originals, map<string, Point> &projections);
	__declspec(dllimport) Equ getLinearEquation(Point &a, Point &b);
	__declspec(dllimport) Equ getPerpendicular(Equ e, Point p);
	__declspec(dllimport) map<string, Equ> getLinearEquationBetweenOriginalsAndProjections(map<string, Point> &originals, map<string, Point> &projections);
	__declspec(dllimport) map<string, Equ> getPerpendicularLinearEquation(map<string, Point> &originals, map<string, Point> &projections, map<string, Point> &centre);
	__declspec(dllimport) Point getColisionPoint(Equ e1, Equ e2);
	__declspec(dllimport) Mat rotateImage(Mat image, double degree);



	class __declspec(dllimport) MabeSegmentation
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

	class __declspec(dllimport) MabeROIDetection
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

	class __declspec(dllimport) MabeFeatureComparison
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

#else


#endif
}


#endif
