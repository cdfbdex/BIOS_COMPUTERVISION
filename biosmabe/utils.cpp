#include "utils.h"

void biosmabe::foo(void) {
	std::cout << "This is BIOSMABE Project" << std::endl;
}

double biosmabe::round(double number) {
	return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
}

double biosmabe::degreeToRadian(double degree) {
	return (M_PI*degree) / 180.0;
}

double biosmabe::radianToDegree(double radian) {
	return (180 * radian) / M_PI;
}

cv::Size biosmabe::rotationNewCanvasSize(double degree, double angle, double h) {
	cv::Size canvas(0, 0);
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

double biosmabe::solveEquationY(Equ e, double x) {

	cout << x << endl;

	if (e.isVertical) return x;

	return e.m*x + e.c;
}

double biosmabe::solveEquationX(Equ e, double y) {

	if (e.isVertical) return e.c;
	if (e.isHorizontal) return 0;

	return (y - e.c) / e.m;
}

map<string, int> biosmabe::rotationExtraMargins(cv::Size &original, cv::Size &newSize) {

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

map<string, cv::Point> biosmabe::getCorners(cv::Size &original, map<string, int> &margins) {
	map<string, cv::Point> m = map<string, cv::Point>();

	m["tl"] = cv::Point(margins["left"], margins["top"]);
	m["tr"] = cv::Point(margins["left"] + original.width, margins["top"]);
	m["bl"] = cv::Point(margins["left"], margins["top"] + original.height);
	m["br"] = cv::Point(margins["left"] + original.width, margins["top"] + original.height);

	return m;

}

map<string, cv::Point> biosmabe::getProjectedCorners(cv::Size &s, double h, double degree, double angle) {

	map<string, cv::Point> m = map<string, cv::Point>();
	double radian = degreeToRadian(degree);
	int top, left;

	m["tl"] = cv::Point(
		round(s.width / 2.0 + h*cos(M_PI - angle + radian)),
		round(s.height / 2.0 - h*sin(M_PI - angle + radian)));

	top = m["tl"].y;
	left = m["tl"].x;

	m["tr"] = cv::Point(
		round(s.width / 2.0 + h*cos(angle + radian)),
		round(s.height / 2.0 - h*sin(angle + radian)));

	if (top > m["tr"].y) top = m["tr"].y;
	if (left > m["tr"].x) left = m["tr"].x;

	m["br"] = cv::Point(
		round(s.width / 2.0 + h*cos(-angle + radian)),
		round(s.height / 2.0 - h*sin(-angle + radian)));

	if (top > m["br"].y) top = m["br"].y;
	if (left > m["br"].x) left = m["br"].x;

	m["bl"] = cv::Point(
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

cv::Point biosmabe::getCentreBetweenPoints(cv::Point &a, cv::Point &b) {
	cv::Point c(0, 0);
	double tmp;

	tmp = abs(a.x - b.x) / 2.0;

	if (b.x <= a.x) c.x = round(tmp + b.x);
	else c.x = round(tmp + a.x);

	tmp = abs(a.y - b.y) / 2.0;

	if (b.y <= a.y) c.y = round(tmp + b.y);
	else c.y = round(tmp + a.y);

	return c;
}

map<string, cv::Point> biosmabe::getCentreBetweenOriginalsAndProjections(map<string, cv::Point> &originals, map<string, cv::Point> &projections) {
	map<string, cv::Point> m = map<string, cv::Point>();

	m["tl"] = getCentreBetweenPoints(originals["tl"], projections["tl"]);
	m["tr"] = getCentreBetweenPoints(originals["tr"], projections["tr"]);
	m["bl"] = getCentreBetweenPoints(originals["bl"], projections["bl"]);
	m["br"] = getCentreBetweenPoints(originals["br"], projections["br"]);

	return m;

}

biosmabe::Equ biosmabe::getLinearEquation(cv::Point &a, cv::Point &b) {

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

biosmabe::Equ biosmabe::getPerpendicular(Equ e, cv::Point p) {
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

map<string, biosmabe::Equ> biosmabe::getLinearEquationBetweenOriginalsAndProjections(map<string, cv::Point> &originals, map<string, cv::Point> &projections) {
	map<string, Equ> m = map<string, Equ>();

	m["tl"] = getLinearEquation(originals["tl"], projections["tl"]);
	m["tr"] = getLinearEquation(originals["tr"], projections["tr"]);
	m["bl"] = getLinearEquation(originals["bl"], projections["bl"]);
	m["br"] = getLinearEquation(originals["br"], projections["br"]);

	return m;
}

map<string, biosmabe::Equ> biosmabe::getPerpendicularLinearEquation(map<string, cv::Point> &originals, map<string, cv::Point> &projections, map<string, cv::Point> &centre) {
	map<string, Equ> m = map<string, Equ>();

	m["tl"] = getPerpendicular(getLinearEquation(originals["tl"], projections["tl"]), centre["tl"]);
	m["tr"] = getPerpendicular(getLinearEquation(originals["tr"], projections["tr"]), centre["tr"]);
	m["bl"] = getPerpendicular(getLinearEquation(originals["bl"], projections["bl"]), centre["bl"]);
	m["br"] = getPerpendicular(getLinearEquation(originals["br"], projections["br"]), centre["br"]);

	return m;
}

cv::Point biosmabe::getColisionPoint(Equ e1, Equ e2) {

	cv::Point2d p = cv::Point2d(0, 0);

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

	return cv::Point(round(p.x), round(p.y));
}

cv::Mat biosmabe::rotateImage(cv::Mat image, double degree)
{

	cv::Mat copy, schema, rot_mat, dst;
	map<string, int> margins;
	cv::Size img, canvas, workspace;
	double h, angle;
	map<string, cv::Point> original, projection, centre;
	map<string, Equ> pEqu;
	cv::Point2f src_centre;
	int speed = 30;
	int type = 0;

	img = cv::Size(image.cols, image.rows);
	h = sqrt(pow(img.width / 2.0, 2) + pow(img.height / 2.0, 2));
	angle = atan((double)img.height / img.width);

	canvas = rotationNewCanvasSize(degree, angle, h);
	margins = rotationExtraMargins(img, canvas);
	copyMakeBorder(image, copy, margins["top"], margins["bottom"], margins["left"], margins["right"], cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
	copy.copyTo(schema);
	workspace = cv::Size(copy.cols, copy.rows);

	original = getCorners(img, margins);
	projection = getProjectedCorners(canvas, h, degree, angle);
	centre = getCentreBetweenOriginalsAndProjections(original, projection);
	pEqu = getPerpendicularLinearEquation(original, projection, centre);

	if (img.width > canvas.width || img.height > canvas.height) {
		cv::Point p = getColisionPoint(pEqu["tl"], pEqu["tr"]);
		src_centre = cv::Point2f(p.x, p.y);
	}
	else {
		src_centre = cv::Point2f(canvas.width / 2.0, canvas.height / 2.0);
	}

	if (type == 0) {
		dst.create(canvas.height, canvas.width, image.channels());
		rot_mat = getRotationMatrix2D(src_centre, degree, 1.0);
		cv::warpAffine(copy, dst, rot_mat, canvas);
	}
	return dst;
}
//Fin Utilidades
