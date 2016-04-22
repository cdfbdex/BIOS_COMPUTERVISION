option(BUILD_GUI_TEST "Build GUI Test Examples" OFF)
option(BUILD_MAINGUI_TEST "Build Main GUI" OFF)

if(BUILD_MAINGUI_TEST)
	find_package(Qt5Widgets)
	#set(CMAKE_AUTOMOC ON)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)

	QT5_WRAP_CPP(MainGUI_MOC mainwindow.h)
	QT5_WRAP_UI(MainGUI_UIS mainwindow.ui)

	add_executable(MainGUI main.cpp mainwindow.cpp ${MainGUI_MOC} ${MainGUI_UIS} mainwindow.rc)
	qt5_use_modules(MainGUI Core Gui Widgets)
	target_link_libraries(MainGUI ${QT_LIBRARIES})
	
	set_target_properties( MainGUI
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
	
endif(BUILD_MAINGUI_TEST)