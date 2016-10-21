option(BUILD_MAINGUI "Build Main GUI" OFF)

if(BUILD_MAINGUI)
	set(GUI_PROJECT_NAME GUI_SIV)

	# Set SOURCES variable
	file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
	# Set HEADERS variable
	file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
	# Set UI variable
	file(GLOB_RECURSE UIS ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)
	# Set QRC variable
	file(GLOB_RECURSE RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc)

	find_package(Qt5Widgets)
	#set(CMAKE_AUTOMOC ON)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)

	QT5_WRAP_CPP(MainGUI_MOC ${HEADERS})
	QT5_WRAP_UI(MainGUI_UIS ${UIS})
	QT5_ADD_RESOURCES(MainGUI_RESOURCES ${RESOURCES})

	add_executable(MainGUI MABE_core ${SOURCES} ${MainGUI_MOC} ${MainGUI_UIS} ${MainGUI_RESOURCES} mainwindow.rc)
	qt5_use_modules(MainGUI Core Gui Widgets Network Multimediawidgets Charts)
	target_link_libraries(MainGUI ${QT_LIBRARIES})
	
	set_target_properties( MainGUI
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
	)
	
endif(BUILD_MAINGUI)