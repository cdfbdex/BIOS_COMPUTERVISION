#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Detectar eventos de las ventanas CHILD
    qApp->installEventFilter(this);

    initializeObjects();

    connectSignalSlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Función para detectar eventos en todos los child del MainWindow
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    //actualizarGUI();
    //qDebug() << "Type " << event->type() << "from" << obj;

//    if (event->type() == QEvent::KeyPress)
//    {
//        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//        qDebug() << "key " << keyEvent->key() << "from" << obj;
//    }
    return QObject::eventFilter(obj, event);
}

// Función para detectar eventos sobre el MainWindow
void MainWindow::changeEvent(QEvent *event)
{
    //actualizarGUI();

    QMainWindow::changeEvent(event);
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    ///TODO: DESCOMENTAR EL SIGUIENTE CÓDIGO PARA PERMITIR LA VALIDACIÓN DEL CIERRE DE LA APLICACIÓN
    event->accept();

/*    if(usuarioLoggedIn)
    {
        QMessageBox::information(this,tr("Cerrar sesión"),
                                 tr("Debe cerrar sesión para poder cerrar la aplicación."));
        cerrarSesion_onTriggered();
        event->ignore();
        return;
    }

    QMessageBox messageBox(QMessageBox::Question,
                tr("Salir"),
                tr("¿Está seguro que desea cerrar la aplicación?"),
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                this);
    messageBox.setButtonText(QMessageBox::Yes, tr("Sí"));
    messageBox.setButtonText(QMessageBox::No, tr("No"));
    messageBox.setButtonText(QMessageBox::Cancel, tr("Cancelar"));

    if (messageBox.exec() == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
*/

}

void MainWindow::initializeObjects()
{
    // Configuración de la aplicación, ubicación del archio de configuración general
    settingsFile = new QSettings(SETTINGS_FILEPATH,QSettings::IniFormat,this);
    QFile file(SETTINGS_FILEPATH);
    if (!file.exists())
    {
        QMessageBox::information(this,tr("Información"),tr("Esta es la primera vez que ejecuta la aplicación, por favor ingrese a configurar el sistema."));
        SettingsWindow::setDefaultValues();
    }

    // Inicialización de variables
    actualizarGUI();

    // Cargar y aplicar el último idioma seleccionado
    changeLanguage(settingsFile->value("language").toString());

    // Clase para control de Log de Eventos
    eventLogger = new EventLoggerController(this);

    ///// Agregar componentes a la interfaz
    // Inicio de sesión
    ui_loginWnd = new LoginWindow(this);
    // Iniciar la aplicación con el inicio de sesión
    ui_loginWnd->show();

    // Ventana de configuración del sistema
    ui_settWnd = new SettingsWindow(this);

    // Menú de inicio
    ui->menuPage->hide();
}

void MainWindow::connectSignalSlot()
{
    // CONEXIONES DE OBJETOS DE INTERFAZ
    connect(ui->actionIniciar_sesi_n,SIGNAL(triggered()),this,SLOT(iniciarSesion_onTriggered()));
    connect(ui->actionCerrar_sesi_n,SIGNAL(triggered()),this,SLOT(cerrarSesion_onTriggered()));
    connect(ui->actionSalir,SIGNAL(triggered()),this,SLOT(salir_OnTriggered()));
    connect(ui->actionSistema,SIGNAL(triggered()),this,SLOT(sistema_onTriggered()));
    connect(ui->actionPar_metros_FIS,SIGNAL(triggered()),this,SLOT(parametrosFIS_onTriggered()));
    connect(ui->actionInformaci_n_de_Disco,SIGNAL(triggered()),this,SLOT(infoDisco_onTriggered()));
    connect(ui->actionLog_de_Eventos,SIGNAL(triggered()),this,SLOT(logDeEventos_onTriggered()));
    connect(ui->action_Cambiar_idioma,SIGNAL(triggered()),this,SLOT(cambiarIdioma_onTriggered()));
    connect(ui->action_Acerca_de,SIGNAL(triggered()),this,SLOT(acercaDe_onTriggered()));

    connect(ui->action_1_Maestro_de_referencias,SIGNAL(triggered()),this,SLOT(maestroDeReferencias_onClicked()));
    connect(ui->action_2_Entrenamiento_de_Modelo_de_Inspecci_n_Visual,SIGNAL(triggered()),this,SLOT(entrenadorModelos_onClicked()));
    connect(ui->action_3_Inspecci_n_Visual_de_Producto,SIGNAL(triggered()),this,SLOT(inspeccionVisual_onClicked()));

    connect(ui->toolButton_CerrarSesion,SIGNAL(clicked()),this,SLOT(cerrarSesion_onTriggered()));

    // CONEXIONES ENTRE SEÑALES DE OTRAS INTERFACES
    // SIGNALS de LoginWindow
    connect(ui_loginWnd,SIGNAL(inicioSesion_Success(QString,int)),this,SLOT(onInicioSesion_Success(QString,int)));
    connect(ui_loginWnd,SIGNAL(cerrarSesion_Success()),this,SLOT(onCerrarSesion_Success()));
    connect(ui_loginWnd,SIGNAL(changeLanguage_Triggered()),this,SLOT(cambiarIdioma_onTriggered()));
    //SIGNALS de SettingsWindow
    connect(ui_settWnd,SIGNAL(changeLanguage()),this,SLOT(cambiarIdioma_onTriggered()));

    // SIGNALS de MenuWindow
    connect(ui->toolButton_MaestroRefs,SIGNAL(clicked()),this,SLOT(maestroDeReferencias_onClicked()));
    connect(ui->toolButton_EntrenaMIV,SIGNAL(clicked()),this,SLOT(entrenadorModelos_onClicked()));
    connect(ui->toolButton_InspeccionVisual,SIGNAL(clicked()),this,SLOT(inspeccionVisual_onClicked()));
    // SIGNALS de Log de Eventos
    connect(eventLogger,SIGNAL(eventLogResponseReady(bool)),this,SLOT(onEventLogResponse_Success(bool)));
}

void MainWindow::changeLanguage(const QString langCode)
{
    // Cambiar idioma a español
    if(langCode == "es")
    {
        qApp->removeTranslator(&qtTranslator);
        ui->retranslateUi(this);
        settingsFile->setValue("language","es");
        return;
    }

    // Cargar archivos de otros idiomas
    QString langPath = QApplication::applicationDirPath();
    langPath.append("/lang");

    QString langFilename("mabeSIV_"+langCode+".qm");

    // Verificar si el archivo de idioma existe
    if(!qtTranslator.load(langFilename,langPath)) {
        qApp->removeTranslator(&qtTranslator);
        ui->retranslateUi(this);
        settingsFile->setValue("language","es");
        QMessageBox::information(this,tr("Información"),tr("No se ha podido cargar el archivo del idioma seleccionado. Se cambiará a Español"));
        qDebug() << "No se ha podido cargar el archivo: " << langPath.append(langFilename);
        return;
    }

    // Verifica si se ha instalado correctamente el idioma
    if(!qApp->installTranslator(&qtTranslator)) {
        QMessageBox::information(this,tr("Información"),tr("No se puede instalar el idioma"));
        qDebug() << "Error instalando el idioma de la ruta: " << langPath.append(langFilename);
        return;
    }

    ui->retranslateUi(this);
    settingsFile->setValue("language",langCode);
}

///// SLOTS /////
void MainWindow::actualizarGUI()
{
    /// TODO: Modificar la actualización del GUI basado en permisos de usuario

    static int i=0;
    qDebug() << ++i << " usuarioLoggedIn: " << usuarioLoggedIn << "\n"
             << "usuarioPrivilegios: " << usuarioPrivilegios;

    if(!usuarioLoggedIn)
    {
        ui->actionIniciar_sesi_n->setEnabled(true);
        ui->actionCerrar_sesi_n->setEnabled(false);

        ui->action_1_Maestro_de_referencias->setEnabled(false);
        ui->action_2_Entrenamiento_de_Modelo_de_Inspecci_n_Visual->setEnabled(false);
        ui->action_3_Inspecci_n_Visual_de_Producto->setEnabled(false);

        ui->toolButton_MaestroRefs->setEnabled(false);
        ui->toolButton_EntrenaMIV->setEnabled(false);
        ui->toolButton_InspeccionVisual->setEnabled(false);

        ui->widget_datosUsuario->setVisible(false);
        return;
    }
    else
    {
        ui->actionIniciar_sesi_n->setEnabled(false);
        ui->actionCerrar_sesi_n->setEnabled(true);
        ui->widget_datosUsuario->setVisible(true);
    }

    // Si el usuario inición sesión, verificar el nivel de privilegios
    // para así mismo mostrar las opciones correspondientes
    /// TODO: Mostrar elementos correspondientes al nivel de privilegios
    switch (usuarioPrivilegios) {
    case 1:
        ui->action_1_Maestro_de_referencias->setEnabled(true);
        ui->action_2_Entrenamiento_de_Modelo_de_Inspecci_n_Visual->setEnabled(true);
        ui->action_3_Inspecci_n_Visual_de_Producto->setEnabled(true);

        ui->toolButton_MaestroRefs->setEnabled(true);
        ui->toolButton_EntrenaMIV->setEnabled(true);
        ui->toolButton_InspeccionVisual->setEnabled(true);
        break;
    case 2:
        ui->action_1_Maestro_de_referencias->setEnabled(true);
        ui->action_2_Entrenamiento_de_Modelo_de_Inspecci_n_Visual->setEnabled(false);
        ui->action_3_Inspecci_n_Visual_de_Producto->setEnabled(true);

        ui->toolButton_MaestroRefs->setEnabled(true);
        ui->toolButton_EntrenaMIV->setEnabled(false);
        ui->toolButton_InspeccionVisual->setEnabled(true);
        break;
    case 3:
        ui->action_1_Maestro_de_referencias->setEnabled(false);
        ui->action_2_Entrenamiento_de_Modelo_de_Inspecci_n_Visual->setEnabled(false);
        ui->action_3_Inspecci_n_Visual_de_Producto->setEnabled(true);

        ui->toolButton_MaestroRefs->setEnabled(false);
        ui->toolButton_EntrenaMIV->setEnabled(false);
        ui->toolButton_InspeccionVisual->setEnabled(true);
        break;
    default:
        break;
    }
}

void MainWindow::iniciarSesion_onTriggered()
{
    ui_loginWnd->show();
}

void MainWindow::cerrarSesion_onTriggered()
{
    ui_loginWnd->show();
}

void MainWindow::salir_OnTriggered()
{
    this->close();
}

void MainWindow::sistema_onTriggered()
{
//    QMessageBox::information(this,tr("Abrir módulo"),tr("Abriendo Configuración del sistema"));
    ui_settWnd->show();
    ui_settWnd->setPage(0); // Abrir página de Configuración de Sistema
}

void MainWindow::parametrosFIS_onTriggered()
{
    ui_settWnd->show();
    ui_settWnd->setPage(1); // Abrir página de Configuración de Parámetros de FIS
}

void MainWindow::infoDisco_onTriggered()
{
    ui_settWnd->show();
    ui_settWnd->setPage(2); // Abrir página de Información de espacio de disco
}

void MainWindow::logDeEventos_onTriggered()
{
    QMessageBox::information(this,tr("Abrir módulo"),tr("Abriendo Log de Eventos"));
//    eventLogger->generateEvent("1053825924");
}

void MainWindow::cambiarIdioma_onTriggered()
{
    QStringList langList;
    langList << "Español" << "English" << "Português";

    bool ok;
    const QString language = QInputDialog::getItem(this,
                                    tr("Cambiar idioma"),
                                    tr("Selecciona un idioma"),
                                    langList,0,false,&ok);

    // Configurar el idioma seleccionado solo cuando
    // se haya presionado el botón OK
    if(ok)
    {
        if(language == "English")
            changeLanguage("en");
        else if(language == "Português")
            changeLanguage("pt");
        else
            changeLanguage("es");
    }
}

void MainWindow::acercaDe_onTriggered()
{
    QMessageBox::information(this,tr("Acerca de..."),tr("Proyecto MABE-BIOS"));
}

void MainWindow::maestroDeReferencias_onClicked()
{
    //QMessageBox::information(this,tr("Abrir módulo"),tr("Abriendo maestro de referencias"));
    ////Maestro de referencias
    ui_MR = new MaestroReferencias(this);
    ui->stackedWidget->addWidget(ui_MR);
    ui->stackedWidget->setCurrentWidget(ui_MR);

    // SIGNALS de Maestro de Referencias
    connect(ui_MR,SIGNAL(closeWindow()),this,SLOT(showMenu()));
}

void MainWindow::entrenadorModelos_onClicked()
{
    QMessageBox::information(this,tr("Abrir módulo"),tr("Abriendo entrenador de modelos de inspección visual"));
    ///Entrenamiento de Modelo de Inspección Visual
    ui_EM = new EntrenamientoModelos(this);
    ui->stackedWidget->addWidget(ui_EM);
    ui->stackedWidget->setCurrentWidget(ui_EM);

    // SIGNALS de Entrenamiento de Modelo
    connect(ui_EM,SIGNAL(closeWindow()),this,SLOT(showMenu()));
}

void MainWindow::inspeccionVisual_onClicked()
{
//    QMessageBox::information(this,tr("Abrir módulo"),tr("Abriendo inspector visual de producto"));
    ///Inspección Visual de Producto
    ui_IV = new InspeccionVisual(this);
    ui->stackedWidget->addWidget(ui_IV);
    ui->stackedWidget->setCurrentWidget(ui_IV);

    // SIGNALS de Inspeccion Visual de Producto
    connect(ui_IV,SIGNAL(closeWindow()),this,SLOT(showMenu()));
}

void MainWindow::showMenu()
{
    // Índice de la ventana que contiene
    ui->menuPage->show();
    ui->stackedWidget->setCurrentIndex(0);
}

/// SLOTS PROVENIENTES DE SEÑALES EN OTRAS INTERFACES

void MainWindow::onInicioSesion_Success(QString username, int _nivelPermisos)
{
    qDebug() << "Inició sesión " << username << " permisos: " << _nivelPermisos;
    usuarioLoggedIn = true;
    usuarioPrivilegios = _nivelPermisos;
    usuarioID = username;
    ui->label_username->setText(usuarioID);

    actualizarGUI();

    showMenu();

    generateEventLog("1","InicioDeSesion");
}

void MainWindow::onCerrarSesion_Success()
{
    qDebug() << "Cerró sesión correctamente";
    usuarioLoggedIn = false;
    usuarioPrivilegios = -1;
    QMessageBox::information(this,tr("Cerrar sesión"),tr("Ha cerrado sesión correctamente"));
    actualizarGUI();


    /// TODO: Cuando se cierre sesión, crear un temporizador de tiempo configurable que cierre la aplicación
    // Limpiar StackedWid excepto el menú
    for(int i = ui->stackedWidget->count(); i >= 1; i--)
    {
        ui->stackedWidget->removeWidget(ui->stackedWidget->widget(i));
    }

    ui->menuPage->hide();
    ui_loginWnd->show();

    generateEventLog("2","Ha cerrado sesión el usuario.");
}

// Función para generar eventos en log
void MainWindow::generateEventLog(QString _codeEvent,QString _detailEvent)
{
    eventLogger->generateEvent(usuarioID,_codeEvent,_detailEvent);
}

// Función para verificar si el log de eventos se proceso correctamente
void MainWindow::onEventLogResponse_Success(bool eventProcessed)
{
    qDebug() << "Evento procesado: " << eventProcessed;
}
