#include "agregarroi.h"
#include "ui_agregarroi.h"

AgregarROI::AgregarROI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AgregarROI)
{
    ui->setupUi(this);

    initializeObjects();

    connectSignalSlot();
}

AgregarROI::~AgregarROI()
{
    delete ui;
}

void AgregarROI::initializeObjects()
{

}

void AgregarROI::connectSignalSlot()
{
    connect(ui->radioButton_buscarCodigo,SIGNAL(clicked()),this,SLOT(tipoDeBusqueda()));
    connect(ui->radioButton_buscarDescripcion,SIGNAL(clicked()),this,SLOT(tipoDeBusqueda()));
    connect(ui->comboBox_busqueda,SIGNAL(currentIndexChanged(int)),this,SLOT(obtenerCodigoDeDescripcion(int)));
}

void AgregarROI::asignarListasParaCompletar(QStringList _codeList, QStringList _descripList)
{
    codeList = _codeList;
    descriptionList = _descripList;

    // Asignar completer de código
    searchCompleter = new QCompleter(codeList,this);
    searchCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    ui->comboBox_busqueda->clear();
    ui->comboBox_busqueda->addItems(codeList);
    ui->comboBox_busqueda->setCompleter(searchCompleter);
}

// REIMPLEMENTED FUNCTIONS
void AgregarROI::accept()
{
    emit enviarCodigoAccesorio(accesoryCode);
    this->close();
}

//SLOTS
void AgregarROI::tipoDeBusqueda()
{
    if(ui->radioButton_buscarCodigo->isChecked())
    {
        // Completer para búsquedas por código.
        searchCompleter = new QCompleter(codeList,this);
        searchCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        ui->comboBox_busqueda->clear();
        ui->comboBox_busqueda->addItems(codeList);
        ui->comboBox_busqueda->setCompleter(searchCompleter);
    }
    else if(ui->radioButton_buscarDescripcion->isChecked())
    {
        // Completer para búsquedas por accesorio.
        searchCompleter = new QCompleter(descriptionList,this);
        searchCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        ui->comboBox_busqueda->clear();
        ui->comboBox_busqueda->addItems(descriptionList);
        ui->comboBox_busqueda->setCompleter(searchCompleter);
    }
    else
    {
        qDebug() << "No se puede determinar el estado de los RadioButton";
    }
}

void AgregarROI::obtenerCodigoDeDescripcion(int index)
{
    if(index>=0 && index<codeList.size())
    {
        accesoryCode = codeList.at(index);
//        qDebug() << "SELECTED CODE: " << accesoryCode;
        ui->label_codigoDeAccesorio->setText(accesoryCode);
    }
    else
    {
        qDebug() << "Error obteniendo código de descripción";
    }
}
