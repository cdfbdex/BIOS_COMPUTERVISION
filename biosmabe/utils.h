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

using namespace cv;
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

}
#endif // UTILS_H