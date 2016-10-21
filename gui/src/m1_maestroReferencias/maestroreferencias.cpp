#include "maestroreferencias.h"
#include "ui_maestroreferencias.h"

MaestroReferencias::MaestroReferencias(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaestroReferencias)
{
    ui->setupUi(this);

    initializeObjects();

    connectSignalSlot();
}

MaestroReferencias::~MaestroReferencias()
{
    delete ui;
}

void MaestroReferencias::initializeObjects()
{
    // Gestión de webservices
    m_apiControl = new APIController(this);

    // Modelo-Vista-Controlador para tabla de referencias
    modeloRefs = new QStandardItemModel(this);
    ui->tableView_referenciasRefrig->verticalHeader()->hide();

    // Filtro de datos en Modelo-Vista-Controlador
    proxyModeloRefs = new MySortFilterProxyModel(this);
    proxyModeloRefs->setDateColumn(0);
    proxyModeloRefs->setSourceModel(modeloRefs);
    ui->tableView_referenciasRefrig->setModel(proxyModeloRefs);
    // Desactivar filtros por fecha
    ui->groupBox_filtroFecha->setChecked(false);

    // Comando para que las columnas ocupen el ancho de la pantalla
    //ui->tableView_referenciasRefrig->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MaestroReferencias::connectSignalSlot()
{
    // SIGNALS Y SLOTS PARA CONEXIÓN CON WEBSERVICE
    connect(m_apiControl,SIGNAL(dataRequestedReady(QByteArray)),this,SLOT(handleResponse(QByteArray)));
    connect(m_apiControl,SIGNAL(networkError(QString)),this,SLOT(handleNetworkError(QString)));

    // SIGNALS Y SLOTS DE OBJETOS DEL GUI
    connect(ui->pushButton_cerrar,SIGNAL(clicked()),this,SLOT(cerrar_onClicked()));
    connect(ui->pushButton_actualizarDatos,SIGNAL(clicked()),this,SLOT(actualizarDatos_onClicked()));
    connect(ui->pushButton_filtrar,SIGNAL(clicked()),this,SLOT(filtrar_onClicked()));

    connect(ui->lineEdit_filterPattern,SIGNAL(returnPressed()),this,SLOT(filtrar_onClicked()));
    connect(ui->comboBox_filterColumn,SIGNAL(currentIndexChanged(int)),this,SLOT(filterColumnChanged(int)));
    connect(ui->groupBox_filtroFecha,SIGNAL(toggled(bool)),ui->widget_filtroFecha,SLOT(setEnabled(bool))); // Activar o desactivar filtro de fecha

    connect(ui->pushButton_registrarNuevaRef,SIGNAL(clicked()),this,SLOT(registrarReferencia_onClicked()));
}

void MaestroReferencias::llenarTablaReferenciasRegistradas()
{
    // Limpiar modelo antes de reconfigurarlos
    modeloRefs->clear();

    // Lista de claves que se extraerán del JSON
    QStringList keysList;
    keysList << "fechaHora" << "id_referencia" << "id_bodega" << "activo";

    // Títulos de las columnas de la tabla
    QStringList headerList;
    headerList << tr("Fecha/Hora") << tr("Referencia") << tr("Línea") << tr("Activa");
    modeloRefs->setHorizontalHeaderLabels(headerList);

    // Agregar lista de títulos en el combobox
    ui->comboBox_filterColumn->clear();
    ui->comboBox_filterColumn->addItems(headerList);

    // Extracción de valores del JSON
//    QJsonObject jsonOriginal = JsonController::readJSONFromByteArray(serverResponse);
//    QJsonObject jsonReceived = jsonOriginal["listadoReferenciasResult"].toObject();
//    foreach (const QJsonValue &referencia, /*jsonReceived*/jsonOriginal)
    QJsonParseError ok;
    QJsonDocument jsonOriginal(QJsonDocument::fromJson(serverResponse,&ok));
    if(ok.error != QJsonParseError::NoError)
        qDebug() << "Error leyendo Json de referencias: " << ok.errorString();

    QJsonArray jsonReceived(jsonOriginal.array());
    foreach (const QJsonValue &referencia, jsonReceived)
    {
        QList<QStandardItem*> rowData;
        QJsonObject refObj(referencia.toObject());

        // Recorrer cada una de las llaves dentro de un Objeto Referencia
        for (int i=0;i<keysList.size();i++)
        {
            QStandardItem *item = new QStandardItem(refObj[keysList.at(i)].toString());

            // Mostrar casillas de verificación para el cambio activar
            if(keysList.at(i) == "activo")
            {
                // La llave "activo" no se puede convertir a QString porque es de tipo booleano
                item = new QStandardItem(refObj[keysList.at(i)].toBool() == true? "true" : "false");

                item->setCheckable(true);
                item->text() == "true" ? item->setCheckState(Qt::Checked) : item->setCheckState(Qt::Unchecked);
                item->setText("");
            }
            rowData.append(item);
        }
        // Agregar al listado
        modeloRefs->appendRow(rowData);

/*
        qDebug() << "VALUE: " << value;
        switch(value.type())
        {
        case QJsonValue::Object:
            foreach (const QJsonValue &value2, value.toObject())
            {
                qDebug() << "VALUE 2: " << value2;
            }
            break;
        case QJsonValue::Array:
            break;
        case QJsonValue::Double:
            break;
        case QJsonValue::String:
            break;
        case QJsonValue::Bool:
            break;
        case QJsonValue::Null:
            qDebug() << "JsonValue is NULL";
            break;
        case QJsonValue:: Undefined:
        default:
            qDebug() << "JsonValue is UNDEFINED";
            break;
        }
        qDebug() << "\n\n";*/

    }
}

void MaestroReferencias::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
    QWidget::changeEvent(event);
}

void MaestroReferencias::closeEvent(QCloseEvent *event)
{
    emit closeWindow();
    event->accept();
}

///// SLOTS /////
// Respuesta del webservice
void MaestroReferencias::handleResponse(QByteArray response)
{
    serverResponse = response;
//    qDebug() << serverResponse;

    // Verificar cual fue el servicio solicitado
    switch (LastWebServiceRequested)
    {
    case RQ_REFERENCELIST: // Listado de referencias
        llenarTablaReferenciasRegistradas();
        break;
    default:
        qDebug() << "Request not valid";
        break;
    }
    LastWebServiceRequested = RQ_NULL;
}

// Control de errores del webservice
void MaestroReferencias::handleNetworkError(QString errorString)
{
    QMessageBox::information(0,tr("Error de red"),tr("Error al conectar con el servidor: ")+errorString);
}

void MaestroReferencias::actualizarListaReferenciasUndRefrigeracion()
{
//    ///////////////////////////////
//    // TODO: Borrar esto cuando se realice desde el servidor
//    // Leer archivo json para simular el recibido desde el servidor
//    QJsonDocument jsonDoc(JsonController::readJSONFromFile(WS_REFERENCELIST));
//    serverResponse = jsonDoc.toJson();

//    llenarTablaReferenciasRegistradas();
//    return;
//    ///////////////////////////////

    // Eliminar filtros al traer la lista
    ui->lineEdit_filterPattern->setText("");
    filtrar_onClicked();

    // Archivo de configuración del sistema para petición HTTP
    QString onlyActiveReferences = ui->checkBox_onlyActiveReferences->isChecked()? "1" : "0";
    QSettings *settings = new QSettings(SETTINGS_FILEPATH,QSettings::IniFormat);
    QString assemblyLine = settings->value("System/assemblyLineID").toString();

    // Crear arreglo de parámetros en el JSON
    QHash<QString,QVariant> paramList;
    paramList.insert("soloActivas",onlyActiveReferences);
    paramList.insert("lineaProduccion",assemblyLine);

    // Create Json Document
    QByteArray jsonDocument = JsonController::createJSONDocument(paramList);
//    qDebug() << jsonDocument;

    // Ejecutar método HTTP POST
    m_apiControl->postRequest(WS_REFERENCELIST,jsonDocument);

    LastWebServiceRequested = RQ_REFERENCELIST;
}

void MaestroReferencias::cerrar_onClicked()
{
    this->close();
}


void MaestroReferencias::actualizarDatos_onClicked()
{
    actualizarListaReferenciasUndRefrigeracion();    
}

void MaestroReferencias::registrarReferencia_onClicked()
{
    ui_nuevaRef = new RegistrarNuevaReferencia();
    ui_nuevaRef->show();
}

// Filtrar datos de tabla
void MaestroReferencias::filtrar_onClicked()
{
    QRegExp::PatternSyntax syntax = QRegExp::RegExp;    //Otros tipos: RegExp, WildCard, FixedString

    Qt::CaseSensitivity caseSensitivity =
            ui->checkBox_caseSensitive->isChecked() ? Qt::CaseSensitive
                                                       : Qt::CaseInsensitive;

    QRegExp regExp(ui->lineEdit_filterPattern->text(), caseSensitivity, syntax);

    proxyModeloRefs->setFilterDateEnable(ui->groupBox_filtroFecha->isChecked());
    proxyModeloRefs->setFilterMinimumDate(ui->dateTimeEdit_inicio->dateTime());
    proxyModeloRefs->setFilterMaximumDate(ui->dateTimeEdit_fin->dateTime());

    proxyModeloRefs->setFilterRegExp(regExp);
}

void MaestroReferencias::filterColumnChanged(int _index)
{
    proxyModeloRefs->setFilterKeyColumn(_index);
}

