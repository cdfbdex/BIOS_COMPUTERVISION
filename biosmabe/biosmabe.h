#ifndef BIOSMABE_H
#define BIOSMABE_H

#ifdef _WIN32 || _WIN64
#  define EXPORTIT __declspec( dllexport )
#else
#  define EXPORTIT
#endif


#include <iostream>

#include <biosmabe\version.h>

// Additional classes
#include <biosmabe\baseclass.h>
#include <biosmabe\utils.h>
#include <biosmabe\MabeSegmentation.h>
#include <biosmabe\MabeROIDetection.h>
#include <biosmabe\MabeFeatureComparison.h>

using namespace std;

namespace biosmabe{

	
}


#endif	//BIOSMABE_H
