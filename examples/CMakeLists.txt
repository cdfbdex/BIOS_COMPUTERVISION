# (uncomment these lines for an external project)
#
# cmake_minimum_required(VERSION 2.8)
# project(Example)
# find_package(BIOSMABE REQUIRED)

# include_directories(${BIOSMABE_INCLUDE_DIRS})

option(BUILD_LIB_TESTS "Build lib Test Examples" OFF)
option(BUILD_SEGMENTATIONMODULE_TESTS "Build Segmentation Module Test Examples" OFF)
option(BUILD_ROIDETECTIONMODULE_TESTS "Build ROI Detection Module Test Examples" OFF)
option(BUILD_FEATURECOMPARISONMODULE_TESTS "Build Feature Comparison Module Test Examples" OFF)


find_package(OpenCV REQUIRED)


if(BUILD_LIB_TESTS)
	#Example 1: Testing BiosMabe Library
	add_executable(Test1BiosMabeLib test1BiosMabeLib.cpp)
	target_link_libraries(Test1BiosMabeLib ${BIOSMABE_LIBS} ${OpenCV_LIBS})
	install(TARGETS Test1BiosMabeLib RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
endif(BUILD_LIB_TESTS)

if(BUILD_SEGMENTATIONMODULE_TESTS)
	#Example 1: Testing BiosMabe Segmentation Module
	add_executable(Test1ModuloSegmentation test1ModuloSegmentation.cpp)
	target_link_libraries(Test1ModuloSegmentation ${BIOSMABE_LIBS} ${OpenCV_LIBS})
	install(TARGETS Test1ModuloSegmentation RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
	
	#Example 2: Testing BiosMabe Segmentation Module
	add_executable(Test2ModuloSegmentation test2ModuloSegmentation.cpp)
	target_link_libraries(Test2ModuloSegmentation ${BIOSMABE_LIBS} ${OpenCV_LIBS})
	install(TARGETS Test2ModuloSegmentation RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
	
endif(BUILD_SEGMENTATIONMODULE_TESTS)

if(BUILD_ROIDETECTIONMODULE_TESTS)
	#Example 1: Testing BiosMabe ROI Detection Module
	add_executable(Test1ModuloRoiDetection test1ModuloRoiDetection.cpp)
	target_link_libraries(Test1ModuloRoiDetection ${BIOSMABE_LIBS} ${OpenCV_LIBS})
	install(TARGETS Test1ModuloRoiDetection RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
endif(BUILD_ROIDETECTIONMODULE_TESTS)

if(BUILD_FEATURECOMPARISONMODULE_TESTS)
	#Example 1: Testing BiosMabe Feature Comparison Module
	add_executable(Test1ModuloFeatureComparison test1ModuloFeatureComparison.cpp)
	target_link_libraries(Test1ModuloFeatureComparison ${BIOSMABE_LIBS} ${OpenCV_LIBS})
	install(TARGETS Test1ModuloFeatureComparison RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin)
endif(BUILD_FEATURECOMPARISONMODULE_TESTS)