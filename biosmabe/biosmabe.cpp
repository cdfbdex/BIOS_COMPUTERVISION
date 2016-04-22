#include "biosmabe.h"
#include <stdio.h>
using namespace biosmabe;
#ifndef M_PI
#define M_PI 3.141592654
#endif

#if defined(WINENVIRONMENT32) || defined(WINENVIRONMENT64)

__declspec(dllexport) void biosmabe::foo(void) {
	std::cout << "This is BIOSMABE Project" << std::endl;
}

__declspec(dllexport) double biosmabe::round(double number) {
	return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
}

__declspec(dllexport) double biosmabe::degreeToRadian(double degree) {
	return (M_PI*degree) / 180.0;
}

__declspec(dllexport) double biosmabe::radianToDegree(double radian) {
	return (180 * radian) / M_PI;
}

__declspec(dllexport) Size biosmabe::rotationNewCanvasSize(double degree, double angle, double h) {
	Size canvas(0, 0);
	double radian = degreeToRadian(degree);
	if ((degree >= 0 && degree <= 90) || (degree >= 181 && degree <= 270)) {
		canvas.height = round((h*fabs(sin(angle + radian))) * 2);
		canvas.width = round((h*fabs(cos(M_PI - angle + radian))) * 2);
	}
	else {
		canvas.height = round((h*fabs(sin(M_PI - angle + radian))) * 2);
		canvas.width = round((h*fabs(cos(angle + radian))) * 2);
	}
	return canvas;
}

__declspec(dllexport) double biosmabe::solveEquationY(Equ e, double x) {

	cout << x << endl;

	if (e.isVertical) return x;

	return e.m*x + e.c;
}

__declspec(dllexport) double biosmabe::solveEquationX(Equ e, double y) {

	if (e.isVertical) return e.c;
	if (e.isHorizontal) return 0;

	return (y - e.c) / e.m;
}

__declspec(dllexport) map<string, int> biosmabe::rotationExtraMargins(Size &original, Size &newSize) {

	map<string, int> m = map<string, int>();

	if (newSize.height >= original.height) {
		m["top"] = round((newSize.height - original.height) / 2.0);
		m["bottom"] = round((newSize.height - original.height) / 2.0);
	}
	else {
		m["top"] = 0;
		m["bottom"] = 0;
	}

	if (newSize.width >= original.width) {
		m["left"] = round((newSize.width - original.width) / 2.0);
		m["right"] = round((newSize.width - original.width) / 2.0);
	}
	else {
		m["left"] = 0;
		m["right"] = 0;
	}

	return m;
}

__declspec(dllexport) map<string, Point> biosmabe::getCorners(Size &original, map<string, int> &margins) {
	map<string, Point> m = map<string, Point>();

	m["tl"] = Point(margins["left"], margins["top"]);
	m["tr"] = Point(margins["left"] + original.width, margins["top"]);
	m["bl"] = Point(margins["left"], margins["top"] + original.height);
	m["br"] = Point(margins["left"] + original.width, margins["top"] + original.height);

	return m;

}

__declspec(dllexport) map<string, Point> biosmabe::getProjectedCorners(Size &s, double h, double degree, double angle) {

	map<string, Point> m = map<string, Point>();
	double radian = degreeToRadian(degree);
	int top, left;

	m["tl"] = Point(
		round(s.width / 2.0 + h*cos(M_PI - angle + radian)),
		round(s.height / 2.0 - h*sin(M_PI - angle + radian)));

	top = m["tl"].y;
	left = m["tl"].x;

	m["tr"] = Point(
		round(s.width / 2.0 + h*cos(angle + radian)),
		round(s.height / 2.0 - h*sin(angle + radian)));

	if (top > m["tr"].y) top = m["tr"].y;
	if (left > m["tr"].x) left = m["tr"].x;

	m["br"] = Point(
		round(s.width / 2.0 + h*cos(-angle + radian)),
		round(s.height / 2.0 - h*sin(-angle + radian)));

	if (top > m["br"].y) top = m["br"].y;
	if (left > m["br"].x) left = m["br"].x;

	m["bl"] = Point(
		round(s.width / 2.0 + h*cos(M_PI + angle + radian)),
		round(s.height / 2.0 - h*sin(M_PI + angle + radian)));

	if (top > m["bl"].y) top = m["bl"].y;
	if (left > m["bl"].x) left = m["bl"].x;

	m["tl"].x -= left;
	m["tr"].x -= left;
	m["bl"].x -= left;
	m["br"].x -= left;

	m["tl"].y -= top;
	m["tr"].y -= top;
	m["bl"].y -= top;
	m["br"].y -= top;


	return m;

}

__declspec(dllexport) Point biosmabe::getCentreBetweenPoints(Point &a, Point &b) {
	Point c(0, 0);
	double tmp;

	tmp = abs(a.x - b.x) / 2.0;

	if (b.x <= a.x) c.x = round(tmp + b.x);
	else c.x = round(tmp + a.x);

	tmp = abs(a.y - b.y) / 2.0;

	if (b.y <= a.y) c.y = round(tmp + b.y);
	else c.y = round(tmp + a.y);

	return c;
}

__declspec(dllexport) map<string, Point> biosmabe::getCentreBetweenOriginalsAndProjections(map<string, Point> &originals, map<string, Point> &projections) {
	map<string, Point> m = map<string, Point>();

	m["tl"] = getCentreBetweenPoints(originals["tl"], projections["tl"]);
	m["tr"] = getCentreBetweenPoints(originals["tr"], projections["tr"]);
	m["bl"] = getCentreBetweenPoints(originals["bl"], projections["bl"]);
	m["br"] = getCentreBetweenPoints(originals["br"], projections["br"]);

	return m;

}

__declspec(dllexport) biosmabe::Equ biosmabe::getLinearEquation(Point &a, Point &b) {

	Equ equation;

	equation.isVertical = false;
	equation.isHorizontal = false;

	if (a.x - b.x == 0) {
		equation.c = a.x;
		equation.m = 0;
		equation.isVertical = true;
	}
	else if (a.y - b.y == 0) {
		equation.c = a.y;
		equation.m = 0;
		equation.isHorizontal = true;
	}
	else {
		equation.m = (double)(a.y - b.y) / (a.x - b.x);
		equation.c = a.y - equation.m*a.x;
	}

	return equation;

}

__declspec(dllexport) biosmabe::Equ biosmabe::getPerpendicular(Equ e, Point p) {
	Equ equation;

	equation.isVertical = false;
	equation.isHorizontal = false;

	if (e.isHorizontal) {
		equation.c = p.x;
		equation.m = 0;
		equation.isVertical = true;
	}
	else if (e.isVertical) {
		equation.c = p.y;
		equation.m = 0;
		equation.isHorizontal = true;
	}
	else {
		equation.m = -1.0 / e.m;
		equation.c = -p.x*equation.m + p.y;
	}

	return equation;

}

__declspec(dllexport) map<string, biosmabe::Equ> biosmabe::getLinearEquationBetweenOriginalsAndProjections(map<string, Point> &originals, map<string, Point> &projections) {
	map<string, Equ> m = map<string, Equ>();

	m["tl"] = getLinearEquation(originals["tl"], projections["tl"]);
	m["tr"] = getLinearEquation(originals["tr"], projections["tr"]);
	m["bl"] = getLinearEquation(originals["bl"], projections["bl"]);
	m["br"] = getLinearEquation(originals["br"], projections["br"]);

	return m;
}

__declspec(dllexport) map<string, biosmabe::Equ> biosmabe::getPerpendicularLinearEquation(map<string, Point> &originals, map<string, Point> &projections, map<string, Point> &centre) {
	map<string, Equ> m = map<string, Equ>();

	m["tl"] = getPerpendicular(getLinearEquation(originals["tl"], projections["tl"]), centre["tl"]);
	m["tr"] = getPerpendicular(getLinearEquation(originals["tr"], projections["tr"]), centre["tr"]);
	m["bl"] = getPerpendicular(getLinearEquation(originals["bl"], projections["bl"]), centre["bl"]);
	m["br"] = getPerpendicular(getLinearEquation(originals["br"], projections["br"]), centre["br"]);

	return m;
}

__declspec(dllexport) Point biosmabe::getColisionPoint(Equ e1, Equ e2) {

	Point2d p = Point2d(0, 0);

	if (e1.isHorizontal && e2.isVertical) {

		p.x = e2.c;
		p.y = e1.c;

	}
	else if (e1.isVertical && e2.isHorizontal) {

		p.x = e1.c;
		p.y = e2.c;

	}
	else if (e1.isHorizontal) {

		p.y = e1.c;
		p.x = (p.y - e2.c) / e2.m;

	}
	else if (e2.isHorizontal) {

		p.y = e2.c;
		p.x = (p.y - e1.c) / e1.m;

	}
	else if (e1.isVertical) {

		p.x = e1.c;
		p.y = p.x*e2.m + e2.c;

	}
	else if (e2.isVertical) {

		p.x = e2.c;
		p.y = p.x*e1.m + e1.c;

	}
	else {

		p.y = fabs(e1.c*e2.m - e2.c*e1.m) / fabs(e1.m - e2.m);
		p.x = (p.y - e1.c) / e1.m;

	}

	return Point(round(p.x), round(p.y));
}

__declspec(dllexport) Mat biosmabe::rotateImage(Mat image, double degree)
{

	Mat copy, schema, rot_mat, dst;
	map<string, int> margins;
	Size img, canvas, workspace;
	double h, angle;
	map<string, Point> original, projection, centre;
	map<string, Equ> pEqu;
	Point2f src_centre;
	int speed = 30;
	int type = 0;

	img = Size(image.cols, image.rows);
	h = sqrt(pow(img.width / 2.0, 2) + pow(img.height / 2.0, 2));
	angle = atan((double)img.height / img.width);

	canvas = rotationNewCanvasSize(degree, angle, h);
	margins = rotationExtraMargins(img, canvas);
	copyMakeBorder(image, copy, margins["top"], margins["bottom"], margins["left"], margins["right"], BORDER_CONSTANT, Scalar(0, 0, 0));
	copy.copyTo(schema);
	workspace = Size(copy.cols, copy.rows);

	original = getCorners(img, margins);
	projection = getProjectedCorners(canvas, h, degree, angle);
	centre = getCentreBetweenOriginalsAndProjections(original, projection);
	pEqu = getPerpendicularLinearEquation(original, projection, centre);

	if (img.width > canvas.width || img.height > canvas.height) {
		Point p = getColisionPoint(pEqu["tl"], pEqu["tr"]);
		src_centre = Point2f(p.x, p.y);
	}
	else {
		src_centre = Point2f(canvas.width / 2.0, canvas.height / 2.0);
	}

	if (type == 0) {
		dst.create(canvas.height, canvas.width, image.channels());
		rot_mat = getRotationMatrix2D(src_centre, degree, 1.0);
		cv::warpAffine(copy, dst, rot_mat, canvas);
	}
	return dst;
}
//Fin Utilidades

//Implmentación Clase MabeSegmentation
biosmabe::MabeSegmentation::MabeSegmentation(Mat Imagen){
	ImagenEntrada = Imagen.clone();
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
biosmabe::MabeSegmentation::~MabeSegmentation(){}
//Fin Implmentación Clase MabeSegmentation


//Implmentación Clase MabeROIDetection
biosmabe::MabeROIDetection::MabeROIDetection(Mat Imagen){
	ImagenEntrada = Imagen.clone();
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
Mat biosmabe::MabeROIDetection::getImagenSalida(){
	return ImagenSalida;
}
Mat biosmabe::MabeROIDetection::getImagenEntrada(){
	return ImagenEntrada;
}
biosmabe::MabeROIDetection::~MabeROIDetection(){}
//Fin Implmentación Clase MabeROIDetection

//Implmentación Clase MabeFeatureComparison
biosmabe::MabeFeatureComparison::MabeFeatureComparison(Mat Imagen){
	ImagenEntrada = Imagen.clone();
}
biosmabe::MabeFeatureComparison::MabeFeatureComparison(Mat I1, Mat I2){
	if (I1.channels() > 1 || I2.channels() > 1){
		cvtColor(I1, Image_Matches1, CV_BGR2GRAY);
		cvtColor(I2, Image_Matches2, CV_BGR2GRAY);
	}else{
		Image_Matches1 = I1.clone();
		Image_Matches2 = I2.clone();
	}

}
bool biosmabe::MabeFeatureComparison::Tecnica1(){

	if (!ImagenEntrada.data)
		return false;
	cvtColor(ImagenEntrada, ImagenSalida, CV_BGR2GRAY);
	return true;
}
bool biosmabe::MabeFeatureComparison::Tecnica2(){
	if (!ImagenEntrada.data)
		return false;
	cvtColor(ImagenEntrada, ImagenSalida, CV_BGR2HSV);
	return true;
}
Mat biosmabe::MabeFeatureComparison::getImagenSalida(){
	return ImagenSalida;
}
Mat biosmabe::MabeFeatureComparison::getImagenEntrada(){
	return ImagenEntrada;
}

bool biosmabe::MabeFeatureComparison::FlannMatcher()
{
	if (!Image_Matches1.data || !Image_Matches2.data)
		return false;

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 10;

	SurfFeatureDetector detector(minHessian);

	detector.detect(Image_Matches1, keypoints_1);
	detector.detect(Image_Matches2, keypoints_2);

	//-- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor extractor;

	Mat descriptors_1, descriptors_2;

	extractor.compute(Image_Matches1, keypoints_1, descriptors_1);
	extractor.compute(Image_Matches2, keypoints_2, descriptors_2);

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors_1, descriptors_2, matches);

	double max_dist = 0.6; double min_dist = 0.001;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_1.rows; i++)
	{
		double dist = matches[i].distance;
		//std::cout << dist << std::endl;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	std::cout<<std::endl<<"-- Max dist : "<< max_dist<<std::endl;
	std::cout << "-- Min dist : " << min_dist << std::endl;
	return true;
}

Mat biosmabe::MabeFeatureComparison::getImageMatches(){
	//-- Draw only "good" matches
	Mat img_matches;
	drawMatches(Image_Matches1, keypoints_1, Image_Matches2, keypoints_2,
		good_matches, Image_Matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	for (int i = 0; i < (int)good_matches.size(); i++)
	{
		printf("-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx);
	}

	return Image_Matches;
}
biosmabe::MabeFeatureComparison::~MabeFeatureComparison(){}
//Fin Implmentación Clase MabeFeatureComparison
#else

#endif