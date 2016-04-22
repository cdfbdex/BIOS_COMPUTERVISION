#ifndef MABESEGMENTATION_H
#define MABESEGMENTATION_H

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

	class EXPORTIT MabeSegmentation
	{

	private:
		/// Atributes
		Mat ImagenEntrada;
		Mat ImagenSalida;
 
	public:
		/// Constructor and destructor
		MabeSegmentation(Mat);
		~MabeSegmentation();

		/// Public methods
		bool Tecnica1();
		bool Tecnica2();
		Mat getImagenSalida();
		Mat getImagenEntrada();
		
	};

}
#endif // MABESEGMENTATION_H