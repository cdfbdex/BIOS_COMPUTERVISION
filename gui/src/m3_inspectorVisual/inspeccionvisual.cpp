#include "inspeccionvisual.h"
#include "ui_inspeccionvisual.h"

InspeccionVisual::InspeccionVisual(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InspeccionVisual)
{
    ui->setupUi(this);

    initializeObjects();

    connectSignalSlot();
}

InspeccionVisual::~InspeccionVisual()
{
    delete ui;
}

void InspeccionVisual::connectSignalSlot()
{
    // SIGNALS Y SLOTS PARA CONEXIÓN CON WEBSERVICE
    connect(m_apiControl,SIGNAL(dataRequestedReady(QByteArray)),this,SLOT(handleResponse(QByteArray)));
    connect(m_apiControl,SIGNAL(networkError(QString)),this,SLOT(handleNetworkError(QString)));

    // SIGNALS Y SLOTS DE OBJETOS DEL GUI
    connect(ui->pushButton_close,SIGNAL(clicked()),this,SLOT(close_onClicked()));
    connect(ui->toolButton_accesoriesList,SIGNAL(clicked()),this,SLOT(accesoriesList_onClicked()));
    connect(ui->toolButton_errorDetails,SIGNAL(clicked()),this,SLOT(errorDetails_onClicked()));
    //Visualización de imágenes
    connect(ui->toolButton_resumen,SIGNAL(clicked()),this,SLOT(resumenImg_onClicked()));
    connect(ui->toolButton_rgb,SIGNAL(clicked()),this,SLOT(rgbImg_onClicked()));
    connect(ui->toolButton_depth,SIGNAL(clicked()),this,SLOT(depthImg_onClicked()));
    connect(ui->toolButton_ir,SIGNAL(clicked()),this,SLOT(irImg_onClicked()));
}

void InspeccionVisual::initializeObjects()
{
    // Gestión de webservices
    m_apiControl = new APIController(this);

    // Mostrar imágenes tipo resumen al iniciar
    ui->stackedWidget_imgs->setCurrentIndex(0);
}

void InspeccionVisual::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
    QWidget::changeEvent(event);
}

void InspeccionVisual::closeEvent(QCloseEvent *event)
{
    emit closeWindow();
    event->accept();
}

///// SLOTS /////
// Respuesta del webservice
void InspeccionVisual::handleResponse(QByteArray response)
{
    serverResponse = response;
    qDebug() << serverResponse;

    // Verificar cual fue el servicio solicitado
    switch (LastWebServiceRequested)
    {
    case RQ_REFERENCELIST: // Listado de referencias
//        llenarTablaReferenciasRegistradas();
        break;
    default:
        qDebug() << "Request not valid";
        break;
    }
    LastWebServiceRequested = RQ_NULL;
}

// Control de errores del webservice
void InspeccionVisual::handleNetworkError(QString errorString)
{
    QMessageBox::information(0,tr("Error de red"),tr("Error al conectar con el servidor: ")+errorString);
}

// Cerrar ventana
void InspeccionVisual::close_onClicked()
{
    this->close();
}

void InspeccionVisual::accesoriesList_onClicked()
{

}

void InspeccionVisual::errorDetails_onClicked()
{

}

// Visualizar tipos de imágenes
void InspeccionVisual::resumenImg_onClicked()
{
    ui->stackedWidget_imgs->setCurrentIndex(0);
}

void InspeccionVisual::rgbImg_onClicked()
{
    ui->stackedWidget_imgs->setCurrentIndex(1);
}

void InspeccionVisual::depthImg_onClicked()
{
    ui->stackedWidget_imgs->setCurrentIndex(2);
}

void InspeccionVisual::irImg_onClicked()
{
    ui->stackedWidget_imgs->setCurrentIndex(3);
}
