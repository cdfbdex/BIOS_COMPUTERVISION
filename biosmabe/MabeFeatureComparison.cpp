#include "MabeFeatureComparison.h"

//Implementaci�n Clase MabeFeatureComparison
biosmabe::MabeFeatureComparison::MabeFeatureComparison(cv::Mat Imagen){
	ImagenEntrada = Imagen.clone();
}

biosmabe::MabeFeatureComparison::~MabeFeatureComparison()
{

}

biosmabe::MabeFeatureComparison::MabeFeatureComparison(cv::Mat I1, cv::Mat I2){
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

cv::Mat biosmabe::MabeFeatureComparison::getImagenSalida(){
	return ImagenSalida;
}

cv::Mat biosmabe::MabeFeatureComparison::getImagenEntrada(){
	return ImagenEntrada;
}

bool biosmabe::MabeFeatureComparison::FlannMatcher()
{
	if (!Image_Matches1.data || !Image_Matches2.data)
		return false;

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 10;

	cv::SurfFeatureDetector detector(minHessian);

	detector.detect(Image_Matches1, keypoints_1);
	detector.detect(Image_Matches2, keypoints_2);

	//-- Step 2: Calculate descriptors (feature vectors)
	cv::SurfDescriptorExtractor extractor;

	cv::Mat descriptors_1, descriptors_2;

	extractor.compute(Image_Matches1, keypoints_1, descriptors_1);
	extractor.compute(Image_Matches2, keypoints_2, descriptors_2);

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	cv::FlannBasedMatcher matcher;
	std::vector< cv::DMatch > matches;
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

cv::Mat biosmabe::MabeFeatureComparison::getImageMatches(){
	//-- Draw only "good" matches
	cv::Mat img_matches;
	drawMatches(Image_Matches1, keypoints_1, Image_Matches2, keypoints_2,
		good_matches, Image_Matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
		vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	for (int i = 0; i < (int)good_matches.size(); i++)
	{
		printf("-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx);
	}

	return Image_Matches;
}

//Fin Implementaci�n Clase MabeFeatureComparison