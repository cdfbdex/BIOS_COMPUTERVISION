#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    initializeObjects();

    connectSignalSlot();
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::initializeObjects()
{
    // Leer valores de configuración
    readSettingsFromFile();

    // Gestión de webservices
    m_apiControl = new APIController(this);
}

void SettingsWindow::connectSignalSlot()
{
    // SIGNALS Y SLOTS PARA CONEXIÓN CON WEBSERVICE
    connect(m_apiControl,SIGNAL(dataRequestedReady(QByteArray)),this,SLOT(handleResponse(QByteArray)));
    connect(m_apiControl,SIGNAL(networkError(QString)),this,SLOT(handleNetworkError(QString)));

    // SIGNALS Y SLOTS PARA OBJETOS DEL GUI
    connect(ui->listWidget_categories,SIGNAL(currentRowChanged(int)),ui->stackedWidget_categories,SLOT(setCurrentIndex(int)));

    connect(ui->toolButton_cambiarIdioma,SIGNAL(clicked()),this,SIGNAL(changeLanguage()));
    connect(ui->pushButton_calcularEspacioEnDisco,SIGNAL(clicked()),this,SLOT(calcularEspacioDisco_onClicked()));

    connect(ui->toolButton_valoresDefecto,SIGNAL(clicked()),this,SLOT(valoresDefecto_onClicked()));
    connect(ui->toolButton_aceptar,SIGNAL(clicked()),this,SLOT(aceptar_onClicked()));
    connect(ui->toolButton_aplicar,SIGNAL(clicked()),this,SLOT(aplicar_onClicked()));
    connect(ui->toolButton_cancelar,SIGNAL(clicked()),this,SLOT(cancelar_onClicked()));
}

/// REIMPLEMENTED FUNCTIONS
// Evento llamado cuando se cierra la ventana
void SettingsWindow::closeEvent (QCloseEvent *event)
{
    // Mostrar advertencia si no se han guardado los cambios
    if(!savedData)
    {
        QMessageBox messageBox(QMessageBox::Question,
                    tr("Salir sin guardar cambios"),
                    tr("No ha guardado los cambios. ¿Está seguro que desea salir?"),
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                    this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Sí"));
        messageBox.setButtonText(QMessageBox::No, tr("No"));
        messageBox.setButtonText(QMessageBox::Cancel, tr("Cancelar"));

        if (messageBox.exec() == QMessageBox::Yes)
            event->accept();
        else
            event->ignore();
    }
    else
    {
        event->accept();
    }
}

// Evento llamado cuando se muestra la ventana
void SettingsWindow::showEvent(QShowEvent *e)
{
    // Ocultar gráfico de cálculo de espacio disponible
    ui->widget_diskSpace->setVisible(false);

    // No se han guardado los datos
    savedData = false;

    e->accept();
    /*
    if (isMaximized())
    {
         if (e->spontaneous())
         {
             // the author would like to know
             // if the event is issued by the system
         }
         ; // the action for maximized
    }
    else
    {
         ; // the action for normal show
    }
    */
}

/// PUBLIC FUNCTIONS
void SettingsWindow::setPage(int index)
{
    ui->listWidget_categories->setCurrentRow(index);
}

void SettingsWindow::readSettingsFromFile()
{
    // Inicialización de Variables
    settingsFile = new QSettings(SETTINGS_FILEPATH,QSettings::IniFormat,this);

    // Cargar configuración
    settingsFile->beginGroup("System");
    ui->input_maxBufferInspections->setValue(settingsFile->value("maxBufferInspections").toInt());
    ui->input_machineID->setText(settingsFile->value("machineID").toString());
    ui->input_assemblyLineID->setText(settingsFile->value("assemblyLineID").toString());
    ui->input_timeBetweenLogin->setValue(settingsFile->value("timeBetweenLogin").toInt());
    settingsFile->endGroup();
    settingsFile->beginGroup("FIS");
    ui->input_URLwebservice->setPlainText(settingsFile->value("URLwebservice").toString());
    settingsFile->endGroup();
}

void SettingsWindow::setDefaultValues()
{
    QSettings *defaultSettings = new QSettings(SETTINGS_FILEPATH,QSettings::IniFormat);

    // Guardar configuración por defecto
    defaultSettings->setValue("language","es");

    defaultSettings->beginGroup("System");
    defaultSettings->setValue("maxBufferInspections",10);
    defaultSettings->setValue("machineID","SIV_0000");
    defaultSettings->setValue("assemblyLineID","L0");
    defaultSettings->setValue("timeBetweenLogin",3);
    defaultSettings->endGroup();

    defaultSettings->beginGroup("FIS");
    defaultSettings->setValue("URLwebservice",SERVER_URL);
    defaultSettings->endGroup();
}

///// SLOTS /////
void SettingsWindow::handleResponse(QByteArray response)
{
    serverResponse = response;

    qDebug() << "RESPUESTA SERVER: " << serverResponse;

    // Verificar cual fue el servicio solicitado
    switch (LastWebServiceRequested)
    {
    case RQ_LOGIN: // Inicio de sesión
//        validateCredentialsLogin(serverResponse);
        break;
    case RQ_LOGOUT:
//        validatePrivilegesOfUser(serverResponse);
        break;
    default:
        break;
    }
    LastWebServiceRequested = RQ_NULL;
}

void SettingsWindow::handleNetworkError(QString errorString)
{
    QMessageBox::information(0,tr("Error de red"),tr("Error al conectar con el servidor:\n")+errorString);
}

void SettingsWindow::valoresDefecto_onClicked()
{
    QMessageBox messageBox(QMessageBox::Question,
                tr("Reestablecer valores de configuración"),
                tr("¿Está seguro que desea Reestablecer los valores de configuración del sistema?"),
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                this);
    messageBox.setButtonText(QMessageBox::Yes, tr("Sí"));
    messageBox.setButtonText(QMessageBox::No, tr("No"));
    messageBox.setButtonText(QMessageBox::Cancel, tr("Cancelar"));

    if (messageBox.exec() == QMessageBox::Yes)
    {
        setDefaultValues();
        readSettingsFromFile();
    }
}

void SettingsWindow::calcularEspacioDisco_onClicked()
{
    // Mostrar  Layouts ocultos
    ui->widget_diskSpace->setVisible(true);

    // Calcular espacio en disco
    QStorageInfo storage(QDir::rootPath());
    qint64 totalSpace = storage.bytesTotal()/1024/1024/1024;
    qint64 availableSpace = storage.bytesAvailable()/1024/1024/1024;
    qint64 usedSpace = totalSpace - availableSpace;

    // Llenar información
    ui->label_usedBytes->setText(QString::number(usedSpace*1024*1024*1024)+" bytes");
    ui->label_usedGB->setText(QString::number(usedSpace)+" GB");
    ui->label_freeBytes->setText(QString::number(storage.bytesAvailable())+" bytes");
    ui->label_freeGB->setText(QString::number(availableSpace)+" GB");
    ui->label_totalBytes->setText(QString::number(storage.bytesTotal())+" bytes");
    ui->label_totalGB->setText(QString::number(totalSpace)+" GB");

    ui->label_diskInfo_value->setText(QString(storage.displayName()+" | "+storage.fileSystemType()));

    // Generar gráfico
    QPieSeries *series = new QPieSeries();
    series->append(tr("Usado"), usedSpace);
    series->append(tr("Disponible"), availableSpace);

    QList<QPieSlice*> listSlices = QList<QPieSlice*>();
    for (int i=0; i<series->count();i++)
    {
        QPieSlice *slice = series->slices().at(i);
        slice->setExploded();
        slice->setLabel(slice->label()+" - "+QString::number(int(slice->percentage()*100))+"%");
        slice->setLabelVisible(true);
        slice->setPen(QPen(Qt::black, 2));
        slice->setLabelFont(QFont("Times",10));
//        slice->setBrush(Qt::green);

        listSlices.append(slice);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTheme(QChart::ChartThemeLight);
    chart->legend()->setVisible(false);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Limpiar Layout para cálculo de espacio libre
    while(ui->layout_diskSpace->count() > 0)
    {
        QLayoutItem *item = ui->layout_diskSpace->takeAt(0);
        delete item;
    }
    // Mostrar gráfico en el layout
    ui->layout_diskSpace->addWidget(chartView);
}

void SettingsWindow::aceptar_onClicked()
{
    aplicar_onClicked();
    this->close();
}

void SettingsWindow::aplicar_onClicked()
{
    // Guardar configuración actual
    settingsFile->beginGroup("System");
    settingsFile->setValue("maxBufferInspections",ui->input_maxBufferInspections->value());
    settingsFile->setValue("machineID",ui->input_machineID->text());
    settingsFile->setValue("assemblyLineID",ui->input_assemblyLineID->text());
    settingsFile->setValue("timeBetweenLogin",ui->input_timeBetweenLogin->value());
    settingsFile->endGroup();

    settingsFile->beginGroup("FIS");
    settingsFile->setValue("URLwebservice",ui->input_URLwebservice->toPlainText());
    settingsFile->endGroup();

    // Actualizar bandera de que la información ha sido guardada
    savedData = true;
}

void SettingsWindow::cancelar_onClicked()
{
    this->close();
}
