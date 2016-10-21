#include "apicontroller.h"

APIController::APIController(QObject* parent):
    QObject(parent)
{
    // Conectar la respuesta del servidor para verificar que no hayan errores
    QObject::connect(&m_nam,SIGNAL(finished(QNetworkReply*)),
                     this,SLOT(parseNetworkResponse(QNetworkReply*)));
}


////////////// MÉTODOS PARA CONTROLAR LOS SERVICIOS WEB

void APIController::getRequest(const QString &urlString)
{
    // Realizar solicitud GET al servidor usando la variable m_nam tipo NetworkAccessManager
    QUrl url(urlString);
    QNetworkRequest req(url);
    m_nam.get(req);
}

void APIController::postRequest(const QString &urlString, const QByteArray &parameters)
{
    // Realizar solicitud POST al servidor usando la variable m_nam tipo NetworkAccessManager
    QUrl url(urlString);
    QNetworkRequest req(url);

    // Agregar encabezados especificando el envío de documento tipo JSON
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
//    req.setHeader(QNetworkRequest::ContentLengthHeader,parameters.size());

    m_nam.post(req,parameters);
}

void APIController::postRequest(const QString &urlString, const QHttpPart &httpPart)
{
    // Realizar solicitud POST al servidor usando la variable m_nam tipo NetworkAccessManager
    QUrl url(urlString);
    QNetworkRequest req(url);
    QHttpMultiPart *part = new QHttpMultiPart();
    part->append(httpPart);

    m_nam.post(req,part);
}

void APIController::parseNetworkResponse(QNetworkReply *finished)
{
    // Verificar si existió un eror en la respuesta del servidor
    if (finished->error() != QNetworkReply::NoError)
    {
        //Un error de comunicación ha ocurrido, enviar la señal
        qDebug() << "ERROR: Network error: " << finished->errorString();
        emit networkError(finished->errorString());
        return;
    }

    // Si no ocurrió error, leer la respuesta del servidor, enviar señal con respuesta
    QByteArray data = finished->readAll();
    //qDebug() << "DATA received from webservice: " << QString(data);
    emit dataRequestedReady(data);
}
