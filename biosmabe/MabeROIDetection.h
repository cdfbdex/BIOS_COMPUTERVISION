#ifndef MABEROIDETECTION_H
#define MABEROIDETECTION_H

/// DO NOT MODIFY: Pre-processor directive to control export process between platforms WIN and GNU.
#ifdef _WIN32 || _WIN64		// WIN export directive
#  define EXPORTIT __declspec( dllexport )
#else						// GNU export directive
#  define EXPORTIT
#endif

/// External libraries
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

// DO NOT MODIFY: Namespace for library
namespace biosmabe{ 

class EXPORTIT MabeROIDetection
{
private:
	/// Atributes
    Mat ImagenEntrada;
	Mat ImagenSalida;
 
public:
	/// Constructor and destructor
	MabeROIDetection(Mat);
	~MabeROIDetection();
 
	/// Public methods
    void SetDate(int year, int month, int day);
 
    bool Tecnica1();
	bool Tecnica2();
	Mat getImagenSalida();
	Mat getImagenEntrada();
};

}
#endif // MABEROIDETECTION_H