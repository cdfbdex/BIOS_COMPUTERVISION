#ifndef BASECLASS_H
#define BASECLASS_H

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define WINENVIRONMENT64
#else
#define WINENVIRONMENT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENTGNU64
#else
#define ENVIRONMENTGNU32
#endif
#endif

#ifdef _WIN32 || _WIN64
#  define EXPORTIT __declspec( dllexport )
#else
#  define EXPORTIT
#endif

#include <biosmabe/version.h>
#include "opencv2/opencv.hpp"
#include "opencv2/opencv_modules.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <string>
#include <vector>
#include <map>
#include <math.h>

namespace biosmabe{ 
class EXPORTIT BaseClass
{
private:
    int m_year;
    int m_month;
    int m_day;
 
public:
    BaseClass(int year, int month, int day);
 
    void SetDate(int year, int month, int day);
 
    int getYear();
    int getMonth();
    int getDay();
};
}
#endif // BASECLASS_H