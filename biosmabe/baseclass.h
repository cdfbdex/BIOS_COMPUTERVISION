// MODIFY pre-processor directive this class
#ifndef BASECLASS_H
#define BASECLASS_H

/// DO NOT MODIFY: Pre-processor directive to control export process between platforms WIN and GNU.
#ifdef _WIN32 || _WIN64		// WIN export directive
#  define EXPORTIT __declspec( dllexport )
#else						// GNU export directive
#  define EXPORTIT
#endif

/// External libraries
#include <biosmabe/version.h>

// DO NOT MODIFY: Namespace for library
namespace biosmabe{ 

class EXPORTIT BaseClass
{
private:
	/// Atributes
    int m_year;
    int m_month;
    int m_day;
 
public:
	/// Constructor and destructor
    BaseClass(int year, int month, int day);
 
	/// Public methods
    void SetDate(int year, int month, int day);
 
    int getYear();
    int getMonth();
    int getDay();
};

}
#endif // BASECLASS_H