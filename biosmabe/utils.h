#ifndef UTILS_H
#define UTILS_H

/// DO NOT MODIFY: Pre-processor directive to control export process between platforms WIN and GNU.
#ifdef _WIN32 || _WIN64		// WIN export directive
#  define EXPORTIT __declspec( dllexport )
#else						// GNU export directive
#  define EXPORTIT
#endif

#ifndef M_PI
	#define M_PI 3.141592654
#endif

/// External libraries
#include "opencv2/opencv.hpp"
//#include "opencv2/opencv_modules.hpp"
//#include "opencv2/nonfree/features2d.hpp"
//#include <string>
//#include <vector>
//#include <map>
//#include <math.h>

//using namespace cv;
using namespace std;

// DO NOT MODIFY: Namespace for library
namespace biosmabe{ 

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
	EXPORTIT cv::Size rotationNewCanvasSize(double degree, double angle, double h);
	EXPORTIT double solveEquationY(Equ e, double x);
	EXPORTIT double solveEquationX(Equ e, double y);
	EXPORTIT map<string, int> rotationExtraMargins(cv::Size &original, cv::Size &newSize);
	EXPORTIT map<string, cv::Point> getCorners(cv::Size &original, map<string, int> &margins);
	EXPORTIT map<string, cv::Point> getProjectedCorners(cv::Size &s, double h, double degree, double angle);
	EXPORTIT cv::Point getCentreBetweenPoints(cv::Point &a, cv::Point &b);
	EXPORTIT map<string, cv::Point> getCentreBetweenOriginalsAndProjections(map<string, cv::Point> &originals, map<string, cv::Point> &projections);
	EXPORTIT Equ getLinearEquation(cv::Point &a, cv::Point &b);
	EXPORTIT Equ getPerpendicular(Equ e, cv::Point p);
	EXPORTIT map<string, Equ> getLinearEquationBetweenOriginalsAndProjections(map<string, cv::Point> &originals, map<string, cv::Point> &projections);
	EXPORTIT map<string, Equ> getPerpendicularLinearEquation(map<string, cv::Point> &originals, map<string, cv::Point> &projections, map<string, cv::Point> &centre);
	EXPORTIT cv::Point getColisionPoint(Equ e1, Equ e2);
	EXPORTIT cv::Mat rotateImage(cv::Mat image, double degree);

}
#endif // UTILS_H