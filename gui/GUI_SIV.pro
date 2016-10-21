#-------------------------------------------------
#
# Project created by QtCreator 2016-08-16T09:25:14
#
#-------------------------------------------------

QT       += core gui network multimediawidgets charts widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUI_SIV
TEMPLATE = app


SOURCES += main.cpp\
    src/appControllers/apicontroller.cpp \
    src/appControllers/jsoncontroller.cpp \
    src/loginwindow/loginwindow.cpp \
    src/mainwindow.cpp \
    src/m1_maestroReferencias/maestroreferencias.cpp \
    src/m2_entrenadorModelos/entrenamientomodelos.cpp \
    src/m3_inspectorVisual/inspeccionvisual.cpp \
    src/appControllers/mvc/referenciaundrefrigeracion.cpp \
    src/appControllers/mvc/mysortfilterproxymodel.cpp \
    src/m1_maestroReferencias/agregarreferencias/registrarnuevareferencia.cpp \
    src/m1_maestroReferencias/agregarreferencias/seleccionarroi.cpp \
    src/m1_maestroReferencias/agregarreferencias/agregarroi.cpp \
    src/appControllers/eventloggercontroller.cpp \
    src/settingsWindow/settingswindow.cpp

HEADERS  += \
    MABE_core \
    src/appControllers/apicontroller.h \
    src/appControllers/jsoncontroller.h \
    src/loginwindow/loginwindow.h \
    src/mainwindow.h \
    src/m1_maestroReferencias/maestroreferencias.h \
    src/constants.h \
    src/m2_entrenadorModelos/entrenamientomodelos.h \
    src/m3_inspectorVisual/inspeccionvisual.h \
    src/appControllers/mvc/referenciaundrefrigeracion.h \
    src/appControllers/mvc/mysortfilterproxymodel.h \
    src/m1_maestroReferencias/agregarreferencias/registrarnuevareferencia.h \
    src/m1_maestroReferencias/agregarreferencias/seleccionarroi.h \
    src/m1_maestroReferencias/agregarreferencias/agregarroi.h \
    src/appControllers/eventloggercontroller.h \
    src/settingsWindow/settingswindow.h


FORMS    += \
    src/loginwindow/loginwindow.ui \
    src/mainwindow.ui \
    src/m1_maestroReferencias/maestroreferencias.ui \
    src/m2_entrenadorModelos/entrenamientomodelos.ui \
    src/m3_inspectorVisual/inspeccionvisual.ui \
    src/m1_maestroReferencias/agregarreferencias/registrarnuevareferencia.ui \
    src/m1_maestroReferencias/agregarreferencias/seleccionarroi.ui \
    src/m1_maestroReferencias/agregarreferencias/agregarroi.ui \
    src/settingsWindow/settingswindow.ui \

RESOURCES += \
    resources.qrc

TRANSLATIONS += ./resources/lang/mabeSIV_en.ts \
                ./resources/lang/mabeSIV_pt.ts
