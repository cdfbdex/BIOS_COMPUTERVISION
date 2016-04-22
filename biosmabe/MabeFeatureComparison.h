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

using namespace cv;
using namespace std;

// DO NOT MODIFY: Namespace for library
namespace biosmabe{ 


class EXPORTIT MabeFeatureComparison
{

public:
	/// Constructor and destructor
	MabeFeatureComparison(Mat);
	MabeFeatureComparison(Mat, Mat);
	~MabeFeatureComparison();

	/// Public methods
	bool Tecnica1();
	bool Tecnica2();
	bool FlannMatcher();
	Mat getImageMatches();
	Mat getImagenSalida();
	Mat getImagenEntrada();
	
private:
	/// Atributes
	Mat ImagenEntrada;
	Mat ImagenSalida;

	Mat Image_Matches;
	std::vector<KeyPoint> keypoints_1, keypoints_2;
	Mat Image_Matches1, Image_Matches2;
	std::vector< DMatch > good_matches;
};

}
#endif // MABEFEATURECOMPARISON_H