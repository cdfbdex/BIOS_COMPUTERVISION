#include "registrarnuevareferencia.h"
#include "ui_registrarnuevareferencia.h"

RegistrarNuevaReferencia::RegistrarNuevaReferencia(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::RegistrarNuevaReferencia)
{
    ui->setupUi(this);

    initializeObjects();

    connectSignalSlot();
}

RegistrarNuevaReferencia::~RegistrarNuevaReferencia()
{
    delete ui;
}

void RegistrarNuevaReferencia::initializeObjects()
{
    // Gestión de webservices
    m_apiControl = new APIController(this);

    // Captura de imágenes
    capturedPhotosList = new QList<QPixmap>();
    ///// TODO: DEBUG: Para efectos de prueba de captura de imágenes
    camera = new QCamera(this);
    imageCapture = new QCameraImageCapture(camera);
    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
    camera->setCaptureMode(QCamera::CaptureStillImage);
    ////////////

    // Cadenas de texto para guardar imágenes
    QDateTime now(QDateTime::currentDateTime());
    dateTimeStamp = now.toString("yyyyMMdd_hhmmss");
    qDebug() << "TIMESTAMP: " << dateTimeStamp;

    // Configurar tamaño máximo a cada página del Wizard
    QDesktopWidget *screen = new QDesktopWidget();
    QSize screenSize = screen->screenGeometry().size();
    for(int i=0; i< this->pageIds().size(); i++)
    {
        QWizardPage *page = this->page(i);
        page->setMaximumSize(0.8*screenSize);
    }

    //Inicializar tabla de resumen de ROI
    modeloROIs = new QStandardItemModel(this);
    ui->tableView_resumenROI->setModel(modeloROIs);
    ui->tableView_resumenROI->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    // Títulos de las columnas de la tabla de ROI
    QStringList headerList;
    headerList << tr("Imagen") << tr("Código Accesorio") << tr("X") << tr("Y") << tr("Ancho") << tr("Alto");
    modeloROIs->setHorizontalHeaderLabels(headerList);

    // Código para referenciar la imagen
    capturedImagesCodes << "FrontalSuperior" << "FrontalInferior" << "PuertaSuperior" << "PuertaInferior";

    // Agregar ROI a las imágenes
    roiDialog = new AgregarROI(this);

    // Resumen
    ui->tableView_copy_resumenROI->setModel(modeloROIs);

}

void RegistrarNuevaReferencia::connectSignalSlot()
{
    // SIGNALS Y SLOTS PARA CONEXIÓN CON WEBSERVICE
    connect(m_apiControl,SIGNAL(dataRequestedReady(QByteArray)),this,SLOT(handleResponse(QByteArray)));
    connect(m_apiControl,SIGNAL(networkError(QString)),this,SLOT(handleNetworkError(QString)));

    // SIGNALS Y SLOTS DE OBJETOS DEL GUI
    connect(ui->pushButton_analizarReferencia,SIGNAL(clicked()),this,SLOT(analizarReferencia()));
    connect(ui->pushButton_capturarImagenes,SIGNAL(clicked()),this,SLOT(capturarImagenesKinect()));
    connect(imageCapture,SIGNAL(imageCaptured(int,QImage)),this,SLOT(procesarImagenes(int,QImage)));

    connect(ui->listWidget_images,SIGNAL(itemSelectionChanged()),this,SLOT(nuevaImagenParaAsignarROI()));
    connect(ui->widget,SIGNAL(setNewROI(QRect)),this,SLOT(mostrarDialogoAgregarROI(QRect)));
    connect(roiDialog,SIGNAL(enviarCodigoAccesorio(QString)),this,SLOT(agregarROIaLista(QString)));
    connect(ui->toolButton_EliminarROI,SIGNAL(clicked()),this,SLOT(eliminarROIdeLista()));
    connect(ui->toolButton_LimpiarROI,SIGNAL(clicked()),this,SLOT(eliminarTodasLasROI()));

    connect(ui->pushButton_registrarReferenciaLocal,SIGNAL(clicked()),this,SLOT(guardarReferenciaLocal()));
    connect(ui->pushButton_registrarReferenciaEnFIS,SIGNAL(clicked()),this,SLOT(guardarReferenciaEnFIS()));
}

QJsonObject RegistrarNuevaReferencia::generateJsonFromROI()
{
    QJsonObject roiListJson;

    // Generar json para cada origen de imagen
    QJsonObject roisFrontSup;           //Imagen Frontal Superior
    QJsonObject roisFrontInf;           //Imagen Frontal Inferior
    QJsonObject roisPuertaSup;           //Imagen Puerta Superior
    QJsonObject roisPuertaInf;           //Imagen Puerta Inferior

    // Clasificar roi dentro de la categoria respectiva
    for (int row=0; row < modeloROIs->rowCount(); row++)
    {
        // Generar rectángulo de ROI
        QJsonArray roiRect;
        roiRect.append(QJsonValue(modeloROIs->data(modeloROIs->index(row,2)).toInt())); // X
        roiRect.append(QJsonValue(modeloROIs->data(modeloROIs->index(row,3)).toInt())); // Y
        roiRect.append(QJsonValue(modeloROIs->data(modeloROIs->index(row,4)).toInt())); // W
        roiRect.append(QJsonValue(modeloROIs->data(modeloROIs->index(row,5)).toInt())); // H

        // Clasificación de ROI: Extrae el índice de la categoría de la imagen basado en los índices de la variable capturedImagesCodes:
            // 0: Frontal Superior, 1: Frontal Inferior, 2: Puerta Superior, 3: Puerta Inferior
        int groupIndex = capturedImagesCodes.indexOf(modeloROIs->data(modeloROIs->index(row,0)).toString());
        QString referenceROI = modeloROIs->data(modeloROIs->index(row,1)).toString();
        switch (groupIndex)     // Agregar el JsonObject al grupo perteneciente.
        {
        case 0: // Imagen Frontal Superior
            roisFrontSup.insert(referenceROI, QJsonValue(roiRect)); // Estructura de clave --> "codigo":[ROI]
            break;
        case 1: // Imagen Frontal Inferior
            roisFrontInf.insert(referenceROI, QJsonValue(roiRect)); // Estructura de clave --> "codigo":[ROI]
            break;
        case 2: // Imagen Puerta Superior
            roisPuertaSup.insert(referenceROI, QJsonValue(roiRect)); // Estructura de clave --> "codigo":[ROI]
            break;
        case 3: // Imagen Puerta Inferior
            roisPuertaInf.insert(referenceROI, QJsonValue(roiRect)); // Estructura de clave --> "codigo":[ROI]
            break;
        default:
            break;
        }
    }

    // Recopilar todos los objetos de cada categoría
    roiListJson.insert(capturedImagesCodes.at(0),QJsonValue(roisFrontSup));
    roiListJson.insert(capturedImagesCodes.at(1),QJsonValue(roisFrontInf));
    roiListJson.insert(capturedImagesCodes.at(2),QJsonValue(roisPuertaSup));
    roiListJson.insert(capturedImagesCodes.at(3),QJsonValue(roisPuertaInf));

    return roiListJson;
}

//////// PROTECTED
void RegistrarNuevaReferencia::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
    QWidget::changeEvent(event);
}

void RegistrarNuevaReferencia::closeEvent(QCloseEvent *event)
{
    emit closeWindow();
    event->accept();
}

bool RegistrarNuevaReferencia::validateCurrentPage()
{
    if(this->currentPage() == ui->detectarReferencia)
    {
        qDebug() << "Current page: Detectar referencia";
        if(!referenceAnalized)
        {
            QMessageBox::warning(this,tr("Registro de nueva referencia"),tr("Debe analizar la referencia para continuar el proceso de registro de referencia."));
            return false;
        }
        return true;
    }
    else if(this->currentPage() == ui->capturarImagenes)
    {
        qDebug() << "Current page: Capturar imágenes";
        if(!photosCaptured)
        {
            QMessageBox::warning(this,tr("Registro de nueva referencia"),tr("Debe capturar las imágenes para continuar el proceso de registro de referencia."));
            return false;
        }
        return true;
    }
    else if(this->currentPage() == ui->analizarImagenes)
    {
        qDebug() << "Current page: Analizar imágenes";
        int roiCount = modeloROIs->rowCount();
        if(!(roiCount>0))
        {
            QMessageBox::warning(this,tr("Registro de nueva referencia"),tr("No ha seleccionado regiones de interés en las imágenes."));
            return false;
        }
        return true;
    }
    else if(this->currentPage() == ui->resumen)
    {
        qDebug() << "Current page: Resumen";
        if(!referenceSaved)
        {
            QMessageBox msgBox;
            msgBox.setText(tr("¿Está seguro que desea salir sin grabar la información?"));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

            int ret = msgBox.exec();
            switch (ret) {
            case QMessageBox::Yes:
                return true;
                break;
            case QMessageBox::No:
              case QMessageBox::Cancel:
              default:
                return false;
                break;
            }
        }
        return true;
    }
    return true;
}

///// SLOTS /////
// Respuesta del webservice
void RegistrarNuevaReferencia::handleResponse(QByteArray response)
{
    serverResponse = response;
    qDebug() << "SERVER RESPONSE: " << serverResponse;

    // Verificar cual fue el servicio solicitado
    switch (LastWebServiceRequested)
    {
    case RQ_ACCESORIESLIST: // Listado de referencias
        obtenerListadoAccesorios();
        break;
    case RQ_REFERENCEREGISTER:
        referenciaRegistrada();
        break;
    default:
        qDebug() << "Request not valid";
        break;
    }
    LastWebServiceRequested = RQ_NULL;
}

// Control de errores del webservice
void RegistrarNuevaReferencia::handleNetworkError(QString errorString)
{
    QMessageBox::information(0,tr("Error de red"),tr("Error al conectar con el servidor: ")+errorString);
}

void RegistrarNuevaReferencia::analizarReferencia()
{
    ///////////////////////////////
    // TODO: Borrar esto cuando se realice desde el servidor
    // Leer archivo json para simular el recibido desde el servidor
    QJsonDocument jsonDoc(JsonController::readJSONFromFile(WS_ACCESORIESLIST));
    serverResponse = jsonDoc.toJson();
    obtenerListadoAccesorios();

    referenceId = ui->lineEdit_referenceCode->text();
    ui->label_referenceToSave->setText(referenceId);

    return;
    ///////////////////////////////

    referenceId = ui->lineEdit_referenceCode->text();
    ui->label_referenceToSave->setText(referenceId);

    // Crear arreglo de parámetros en el JSON
    QHash<QString,QVariant> paramList;
    paramList.insert("idReferencia",referenceId);
    // Crear Json Document
    QByteArray jsonDocument = JsonController::createJSONDocument(paramList);
    // Ejecutar método HTTP POST
    m_apiControl->postRequest(WS_ACCESORIESLIST,jsonDocument);
    LastWebServiceRequested = RQ_ACCESORIESLIST;
}

void RegistrarNuevaReferencia::obtenerListadoAccesorios()
{
    ///TODO: Procesar listado como se envíe del webservice1

    // Extracción de valores del JSON
    QJsonObject jsonOriginal = JsonController::readJSONFromByteArray(serverResponse);
    QJsonObject jsonReceived = jsonOriginal["listadoAccesoriosResult"].toObject();
    QVariantMap accesories = jsonReceived.toVariantMap();

    // Crear lista de códigos de accesorios
    codesAccesories = accesories.keys();
    // Crear lista de descripciones de accesorios
    QVariantMap::Iterator it;
    for (it = accesories.begin();it != accesories.end(); ++it)
    {
        descripAccesories.append(it.value().toString());
    }

    // Label bandera de análisis de accesorios
    ui->label_flag_AnalizarReferencia->setText(tr("Analizar referencia: OK"));
    ui->label_flag_AnalizarReferencia->setStyleSheet("background-color: rgb(0, 200, 0);color: rgb(255, 255, 255);");
    referenceAnalized = true;
}

// TODO: Reemplazar la captura por Kinect en OpenCV
void RegistrarNuevaReferencia::capturarImagenesKinect()
{
    camera->start();
    imageCapture->capture();
}

// TODO: Reemplazar las imagenes que se muestran por las capturadas
// en Kinect, convirtiendo de OpenCV a pixmap
void RegistrarNuevaReferencia::procesarImagenes(int id, const QImage &imageCaptured)
{
    Q_UNUSED(id)
    camera->stop();

    // Limpiar datos anteriores
    capturedPhotosList->clear();
    modeloROIs->removeRows(0,modeloROIs->rowCount());

    // Obtener imágenes nuevas
    QPixmap imgFS(QPixmap::fromImage(imageCaptured));
    //QPixmap imgFS(QString(QCoreApplication::applicationDirPath()+"/testNevera/frontalSuperior.bmp"));
    QPixmap imgFI(QString(QCoreApplication::applicationDirPath()+"/testNevera/frontalInferior.bmp"));
    QPixmap imgPS(QString(QCoreApplication::applicationDirPath()+"/testNevera/puertaSuperior.bmp"));
    QPixmap imgPI(QString(QCoreApplication::applicationDirPath()+"/testNevera/puertaInferior.bmp"));

//    qDebug() << "ORIGINAL SIZES: ";
//    qDebug() << imgFS.size() << "\n" <<
//                imgFI.size() << "\n" <<
//                imgPS.size() << "\n" <<
//                imgPI.size();

    // Mostrar imágenes
    ui->img_frontalSuperior->setPixmap(imgFS);
    ui->img_frontalInferior->setPixmap(imgFI);
    ui->img_puertaSuperior->setPixmap(imgPS);
    ui->img_puertaInferior->setPixmap(imgPI);

    // Imágenes de la pestaña resumen.
    ui->img_frontalSuperior_2->setPixmap(imgFS);
    ui->img_frontalInferior_2->setPixmap(imgFI);
    ui->img_puertaSuperior_2->setPixmap(imgPS);
    ui->img_puertaInferior_2->setPixmap(imgPI);

    // Guardar imágenes en vector
    capturedPhotosList->push_back(imgFS);
    capturedPhotosList->push_back(imgFI);
    capturedPhotosList->push_back(imgPS);
    capturedPhotosList->push_back(imgPI);

    // Label bandera de captura de imágenes
    ui->label_flag_CapturaImagenes->setText(tr("Captura imágenes: OK"));
    ui->label_flag_CapturaImagenes->setStyleSheet("background-color: rgb(0, 200, 0);color: rgb(255, 255, 255);");
    photosCaptured = true;
}

void RegistrarNuevaReferencia::nuevaImagenParaAsignarROI()
{
    // Muestra la imagen seleccionada para extraerle las ROI de accesorios
    int selectedItem = ui->listWidget_images->row(ui->listWidget_images->selectedItems().at(0));
    QPixmap selectedPixmap(capturedPhotosList->at(selectedItem));

    // Configura el widget de selección de ROI
    ui->widget->setOriginalImageSize(selectedPixmap.size());
    ui->widget->setPixmap(selectedPixmap);
}

void RegistrarNuevaReferencia::mostrarDialogoAgregarROI(QRect _roiRectangle)
{
    selectedROI = _roiRectangle;

    ///// TODO: Genera problemas cuando se agregan varios ROI
    roiDialog->asignarListasParaCompletar(codesAccesories,descripAccesories);
    roiDialog->show();
}

void RegistrarNuevaReferencia::agregarROIaLista(QString accessoryCode)
{
    if(accessoryCode.isEmpty())
    {
        QMessageBox::warning(this,tr("No se puede agregar el ROI"),
                             tr("No se puede agregar un accesorio vacío. Por favor verifique si realizó el análisis de la referencia en el paso 1."));
        return;
    }

    // Identificar la imagen que está seleccionada
    int selectedRowImage = ui->listWidget_images->selectionModel()->selectedRows(0).at(0).row();

    // Verificar que no haya un accesorio con el mismo código en la misma imagen
    for (int i=0; i<modeloROIs->rowCount(); i++)
    {
        if( capturedImagesCodes.at(selectedRowImage) == modeloROIs->data(modeloROIs->index(i,0)).toString() && // Verificar si es la misma imagen
            accessoryCode == modeloROIs->data(modeloROIs->index(i,1)))
        {
            QMessageBox::warning(this,tr("No se puede agregar el ROI"),
                                 tr("El accesorio que está intentando agregar ya existe en la imagen especificada. Por favor verifique nuevamente el código."));
            return;
        }
    }

    // Información de fila para agregar nueva ROI
    QStringList valuesRow;
    valuesRow << capturedImagesCodes.at(selectedRowImage) <<
                 accessoryCode <<
                 QString::number(selectedROI.x()) << QString::number(selectedROI.y()) <<
                 QString::number(selectedROI.width()) << QString::number(selectedROI.height());

    QList<QStandardItem*> rowData;
    for (int i=0; i<valuesRow.size();i++)
    {
        QStandardItem *item = new QStandardItem(valuesRow.at(i));
        rowData.append(item);
    }

    modeloROIs->appendRow(rowData);
}

void RegistrarNuevaReferencia::eliminarROIdeLista()
{
    QMessageBox msgBox;
    msgBox.setText(tr("¿Está seguro que desea eliminar la fila seleccionada?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:
        modeloROIs->removeRow(ui->tableView_resumenROI->selectionModel()->selectedRows().at(0).row());
        break;
    case QMessageBox::No:
        break;
    case QMessageBox::Cancel:
        break;
    default:
        // should never be reached
        break;
    }
}

void RegistrarNuevaReferencia::eliminarTodasLasROI()
{
    QMessageBox msgBox;
    msgBox.setText(tr("¿Está seguro que desea eliminar todas las filas?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:
        modeloROIs->removeRows(0,modeloROIs->rowCount());
        break;
    case QMessageBox::No:
        break;
    case QMessageBox::Cancel:
        break;
    default:
        // should never be reached
        break;
    }
}

// TODO: Guardar también las imágenes en DEPTH y NIR
// Guardar el archivo con las selección de ROIs
void RegistrarNuevaReferencia::guardarReferenciaLocal()
{
    int numImages = 4;

    QString formatSavingImage = "PNG";
    QStringList imagesFilename;
    imagesFilename.append("_RGB_FrontSup");
    imagesFilename.append("_RGB_FrontInf");
    imagesFilename.append("_RGB_PuertaSup");
    imagesFilename.append("_RGB_PuertaInf");

    // Variables para controlar la barra de progreso de guardadooooo
    int loadingBarSteps = numImages + 1;    // Grabar imágenes y json de ROI
    int loadingCurrent = 0;    // Valor actual de la barra de carga

    if(capturedPhotosList->size() == numImages)
    {
        // Generar y guardarlos nombres de archivos
        QString pathString = QString(APP_DIRPATH+"/References/"+referenceId+"/");
        if(!QDir(pathString).exists())
        {
            QDir().mkpath(pathString);
        }

        if(QFileInfo(pathString).isWritable())
        {

        }

        qDebug() << pathString+referenceId+"_TYPE_"+dateTimeStamp <<
                    "WRITABLE: " << QFileInfo(pathString).isWritable();

        // Guardar imágenes en disco
        for(int i=0;i < numImages;i++)
        {
            // Aumentar valor de barra de progreso
            ui->progressBar->setValue((loadingCurrent++) * 100 / loadingBarSteps);
            // Mostrar mensaje de detalle de guardado
            ui->label_loadingInfo->setText(QString(tr("Guardando imagen ")+QString::number(loadingCurrent)+"/"+QString::number(loadingBarSteps-1)));
            // Guardar imagen con nombre y formato especificado en las variables "imagesFilename" y "formatSavingImage"
            capturedPhotosList->at(i).save(QString(pathString+dateTimeStamp+imagesFilename.at(i)+"."+formatSavingImage.toLower()),formatSavingImage.toLatin1().data());
        }

        // GENERAR JSON CON TABLA DE ROI
        // Generar mapeo de la tabla
        QJsonObject roiListJson = generateJsonFromROI();

/*
        // Generar json para cada origen de imagen
        QJsonObject roisFrontSup;           //Imagen Frontal Superior
        QJsonObject roisFrontInf;           //Imagen Frontal Inferior
        QJsonObject roisPuertaSup;           //Imagen Puerta Superior
        QJsonObject roisPuertaInf;           //Imagen Puerta Inferior

        // Clasificar roi dentro de la categoria respectiva
        for (int row=0; row < modeloROIs->rowCount(); row++)
        {
            // Generar rectángulo de ROI
            QJsonArray roiRect;
            roiRect.append(QJsonValue(modeloROIs->data(modeloROIs->index(row,2)).toInt())); // X
            roiRect.append(QJsonValue(modeloROIs->data(modeloROIs->index(row,3)).toInt())); // Y
            roiRect.append(QJsonValue(modeloROIs->data(modeloROIs->index(row,4)).toInt())); // W
            roiRect.append(QJsonValue(modeloROIs->data(modeloROIs->index(row,5)).toInt())); // H

            // Clasificación de ROI: Extrae el índice de la categoría de la imagen basado en los índices de la variable capturedImagesCodes:
                // 0: Frontal Superior, 1: Frontal Inferior, 2: Puerta Superior, 3: Puerta Inferior
            int groupIndex = capturedImagesCodes.indexOf(modeloROIs->data(modeloROIs->index(row,0)).toString());
            QString referenceROI = modeloROIs->data(modeloROIs->index(row,1)).toString();
            switch (groupIndex)     // Agregar el JsonObject al grupo perteneciente.
            {
            case 0: // Imagen Frontal Superior
                roisFrontSup.insert(referenceROI, QJsonValue(roiRect)); // Estructura de clave --> "codigo":[ROI]
                break;
            case 1: // Imagen Frontal Inferior
                roisFrontInf.insert(referenceROI, QJsonValue(roiRect)); // Estructura de clave --> "codigo":[ROI]
                break;
            case 2: // Imagen Puerta Superior
                roisPuertaSup.insert(referenceROI, QJsonValue(roiRect)); // Estructura de clave --> "codigo":[ROI]
                break;
            case 3: // Imagen Puerta Inferior
                roisPuertaInf.insert(referenceROI, QJsonValue(roiRect)); // Estructura de clave --> "codigo":[ROI]
                break;
            default:
                break;
            }
        }

        // Recopilar todos los objetos de cada categoría
        roiListJson.insert(capturedImagesCodes.at(0),QJsonValue(roisFrontSup));
        roiListJson.insert(capturedImagesCodes.at(1),QJsonValue(roisFrontInf));
        roiListJson.insert(capturedImagesCodes.at(2),QJsonValue(roisPuertaSup));
        roiListJson.insert(capturedImagesCodes.at(3),QJsonValue(roisPuertaInf));
*/

        // Crear objeto raíz con nombre de referencia
        QJsonObject roisObject;
        roisObject.insert(referenceId,QJsonValue(roiListJson));
        QJsonDocument jsonDoc(roisObject);

        // Guardar archivo en disco.
        ui->progressBar->setValue((loadingCurrent++) * 100 / loadingBarSteps);
        ui->label_loadingInfo->setText(tr("Guardando regiones de interés en JSON."));

        QFile file;
        file.setFileName(pathString+dateTimeStamp+"_ROIlist.json");
        file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
        file.write(jsonDoc.toJson());
        file.close();

        // Final de grabación
        ui->progressBar->setValue((loadingCurrent++) * 100 / loadingBarSteps);
        ui->label_loadingInfo->setText(tr("Guardado."));
        referenceSaved = true;

        QMessageBox::information(this,tr("Información"),tr("La grabación de imágenes y regiones de interés ha finalizado exitosamente."));
    }
    else
    {
        QMessageBox::information(this,tr("Información"),tr("No se pueden guardar las imágenes porque no se han capturado las imágenes de la referencia a registrar."));
    }
}


void RegistrarNuevaReferencia::guardarReferenciaEnFIS()
{
    /////////TODO: Insertar los accesorios y las imagenes en el JSON para FIS

    // Archivo de configuración del sistema
    QSettings *settings = new QSettings(SETTINGS_FILEPATH,QSettings::IniFormat);

    // Crear estructura JSON para almacenar los accesorios registrados a cada imagen.
    QJsonObject roiJson = generateJsonFromROI();
    qDebug() << roiJson;
    QJsonDocument roiDoc(roiJson);
    QString roiJsonString(roiDoc.toJson(QJsonDocument::Compact));

    // Crear arreglo de parámetros en el JSON
    QJsonObject paramList;
//    QHash<QString,QVariant> paramList;
    paramList.insert("idReferencia",QJsonValue(referenceId));
    paramList.insert("fechaHora",QJsonValue(dateTimeStamp));
    paramList.insert("id_bodega",QJsonValue(settings->value("System/assemblyLineID").toString()));
    paramList.insert("activo",QJsonValue("1"));
    paramList.insert("accesorios",QJsonValue(roiJsonString));       // Estructura JSON con accesorios que se leerán posteriormente en otro servicio
//    paramList.insert("accesorios",QJsonValue(roiJson));
    paramList.insert("infoDetalle","CODIGO_BASE64_IMAGENES");
//    paramList.insert("infoDetalle",JsonController::jsonValFromPixmap(*(ui->img_frontalSuperior->pixmap()),"PNG"));

    QJsonObject rootObj;
    rootObj.insert("refer",paramList);
    QJsonDocument jsonDoc(rootObj);
    // Crear Json Document
    QByteArray jsonDocument = jsonDoc.toJson();
    qDebug() << jsonDocument;
    // Ejecutar método HTTP POST
    m_apiControl->postRequest(WS_REFERENCEREGISTER,jsonDocument);
    LastWebServiceRequested = RQ_REFERENCEREGISTER;

//    ///// TODO READ JSON FROM STRING
//    QJsonObject obj = JsonController::readJSONFromQString(roiJsonString);
//    qDebug() << obj["FrontalSuperior"].toObject().toVariantHash();

    /// TODO delete: Save request in file to test
    QString pathString = QString(APP_DIRPATH+referenceId+"/");
    if(!QDir(pathString).exists())
        QDir().mkpath(pathString);
    QFile file;
    file.setFileName(pathString+dateTimeStamp+"_saveRef.json");
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(jsonDocument);
    file.close();
}

void RegistrarNuevaReferencia::referenciaRegistrada()
{
    QString response = QString::fromUtf8(serverResponse);
    if(response == "1")
    {
        QMessageBox::information(this,tr("Información"),tr("La información se ha registrado en FIS exitosamente."));
    }
    else
    {
        QMessageBox::information(this,tr("Información"),tr("La referencia NO se ha podido registrar en FIS."));
    }
}
