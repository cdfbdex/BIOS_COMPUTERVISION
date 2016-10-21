/**
  * @file mainwindow.h
  * @date 13/09/2016
  * @version 0.2
  * @author Centro de Bioinformática y Biología Computacional de Colombia - BIOS
  * @title Ventana principal
  * @brief Ventana que contiene todos los demás elementos (Widgets) del proyecto SIV.
  * @code
    Código de ejemplo
  * @endcode
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "MABE_core"
#include "src/loginwindow/loginwindow.h"
#include "src/settingsWindow/settingswindow.h"
#include "src/m1_maestroReferencias/maestroreferencias.h"
#include "src/m2_entrenadorModelos/entrenamientomodelos.h"
#include "src/m3_inspectorVisual/inspeccionvisual.h"

#include <QSettings>

#include <QMessageBox>
#include <QCloseEvent>
#include <QInputDialog>
#include <QDebug>

#include <QTranslator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void initializeObjects();
    void connectSignalSlot();
    /**
     * @brief changeLanguage Cambia el texto de la interfaz
     * @param langCode Código del lenguaje ("es","en","pt")
     */
    void changeLanguage(const QString langCode);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    virtual void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void actualizarGUI();

    // SIGNAL Y SLOTS DE OBJETOS DE LA INTERFAZ
    void iniciarSesion_onTriggered();
    void cerrarSesion_onTriggered();
    void salir_OnTriggered();
    void sistema_onTriggered();
    void parametrosFIS_onTriggered();
    void infoDisco_onTriggered();
    void logDeEventos_onTriggered();
    void cambiarIdioma_onTriggered();
    void acercaDe_onTriggered();

    void maestroDeReferencias_onClicked();
    void entrenadorModelos_onClicked();
    void inspeccionVisual_onClicked();

    void showMenu();

    // SLOTS PROVENIENTES DE OTRAS VENTANAS
    //Provenientes de LoginWindow
    void onInicioSesion_Success(QString username,int _nivelPermisos);
    void onCerrarSesion_Success();
    void generateEventLog(QString _codeEvent,QString _detailEvent);
    void onEventLogResponse_Success(bool eventProcessed);

private:
    Ui::MainWindow *ui;

    bool usuarioLoggedIn = false;
    QString usuarioID;
    int usuarioPrivilegios = -1;

    LoginWindow *ui_loginWnd;
    SettingsWindow *ui_settWnd;
    MaestroReferencias *ui_MR;
    EntrenamientoModelos *ui_EM;
    InspeccionVisual *ui_IV;
    EventLoggerController *eventLogger;

    QSettings *settingsFile;
    QTranslator qtTranslator;
};

#endif // MAINWINDOW_H
