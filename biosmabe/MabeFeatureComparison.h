#ifndef MABEFEATURECOMPARISON_H
#define MABEFEATURECOMPARISON_H

/// DO NOT MODIFY: Pre-processor directive to control export process between platforms WIN and GNU.
#ifdef _WIN32 || _WIN64		// WIN export directive
#  define EXPORTIT __declspec( dllexport )
#else						// GNU export directive
#  define EXPORTIT
#endif

/// External libraries
#include "opencv2/opencv.hpp"
#include "opencv2/nonfree/features2d.hpp"

//using namespace cv;
using namespace std;

// DO NOT MODIFY: Namespace for library
namespace biosmabe{ 


class EXPORTIT MabeFeatureComparison
{

public:
	/// Constructor and destructor
	MabeFeatureComparison(cv::Mat);
	MabeFeatureComparison(cv::Mat, cv::Mat);
	~MabeFeatureComparison();

	/// Public methods
	bool Tecnica1();
	bool Tecnica2();
	bool FlannMatcher();
	cv::Mat getImageMatches();
	cv::Mat getImagenSalida();
	cv::Mat getImagenEntrada();
	
private:
	/// Atributes
	cv::Mat ImagenEntrada;
	cv::Mat ImagenSalida;

	cv::Mat Image_Matches;
	std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
	cv::Mat Image_Matches1, Image_Matches2;
	std::vector< cv::DMatch > good_matches;
};

}
#endif // MABEFEATURECOMPARISON_H