#include "eventloggercontroller.h"

EventLoggerController::EventLoggerController(QObject *parent)
    : QObject(parent)
{
    initializeObjects();

    connectSignalSlot();
}

void EventLoggerController::initializeObjects()
{
    // Inicialización de Variables
    LastWebServiceRequested = RQ_NULL;

    // Gestión de webservices
    m_apiControl = new APIController(this);
}

void EventLoggerController::connectSignalSlot()
{
    // SIGNALS Y SLOTS PARA CONEXIÓN CON WEBSERVICE
    connect(m_apiControl,SIGNAL(dataRequestedReady(QByteArray)),this,SLOT(handleResponse(QByteArray)));
    connect(m_apiControl,SIGNAL(networkError(QString)),this,SLOT(handleNetworkError (QString)));
}

///// SLOTS
void EventLoggerController::handleResponse(QByteArray response)
{
    serverResponse = response;

    qDebug() << "RESPUESTA SERVIDOR: " << serverResponse;

    QJsonObject jsonServerResponse = JsonController::readJSONFromByteArray(serverResponse);
    bool eventProcessed = false;

    // Verificar cual fue el servicio solicitado
    switch (LastWebServiceRequested)
    {
    case RQ_EVENTLOG: // Crear Log de Eventos
        // Function
        if(jsonServerResponse.contains("registrarEventoResult"))
        {
            if(jsonServerResponse["registrarEventoResult"].toInt() == 1)
            {
                eventProcessed = true;
                qDebug() << "Ha generado un nuevo evento.";
            }
        }
        emit eventLogResponseReady(eventProcessed);
        break;

    default:
        qDebug() << "Solicitud desconocida a servicio web.";
        break;
    }

    LastWebServiceRequested = RQ_NULL;
}

void EventLoggerController::handleNetworkError(QString errorString)
{
    QMessageBox::information(0,tr("Error de red"),tr("Error al conectar con el servidor:\n")+errorString);
    emit eventLogResponseReady(false);
}

void EventLoggerController::generateEvent(QString user, QString codeEvent, QString detailEvent)
{
    // Archivo de configuración del sistema
    QSettings *settings = new QSettings(SETTINGS_FILEPATH,QSettings::IniFormat);

    QDateTime now(QDateTime::currentDateTime());
    QString dateTimeStamp = now.toString("yyyy-MM-dd hh:mm:ss");

//    qDebug() << WS_EVENTLOG.arg(dateTimeStamp,
//                                settings->value("System/assemblyLineID").toString(),
//                                settings->value("System/machineID").toString(),
//                                user,
//                                codeEvent,
//                                detailEvent);

    m_apiControl->getRequest(WS_EVENTLOG.arg(dateTimeStamp,
                                             settings->value("System/assemblyLineID").toString(),
                                             settings->value("System/machineID").toString(),
                                             user,
                                             codeEvent,
                                             detailEvent));

    LastWebServiceRequested = RQ_EVENTLOG;
}
