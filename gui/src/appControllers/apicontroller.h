/**
  * @file apicontroller.h
  * @date 13/09/2016
  * @version 0.2
  * @author Centro de Bioinformática y Biología Computacional de Colombia - BIOS
  * @title API Controller
  * @brief Funciones para controlar las solicitudes HTTP tipo GET y tipo POST a los webservices.
  * @code
    Sin código de ejemplo
  * @endcode
  */

#ifndef APICONTROLLER_H
#define APICONTROLLER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QHttpMultiPart>

class APIController : public QObject
{
    Q_OBJECT

public:
    explicit APIController(QObject *parent = 0);

    /**
     * @brief getRequest Solicitudes HTTP tipo GET a la URL especificada
     * @param urlString Dirección web donde se encuentra el servicio web
     */
    void getRequest(const QString &urlString);

    /**
     * @brief postRequest Solicitudes HTTP tipo POST a la URL especificada
     * @param urlString Dirección web donde se encuentra el servicio web
     * @param jsonDocument Archivo JSON que contiene los parámetros a enviar al servidor
     */
    void postRequest(const QString &urlString, const QByteArray &parameters);

    void postRequest(const QString &urlString, const QHttpPart &parameters);

signals:
    /**
     * @brief dataRequestedReady Señal emitida cuando se ha recibido correctamente la respuesta del servidor
     * @param dataFromServer Respuesta del servidor en estructura JSON
     */
    void dataRequestedReady(QByteArray dataFromServer);

    /**
     * @brief networkError Señal emitida cuando se ha detectado un error de comunicación con el servidor
     * @param err Descripción del error encontrado
     */
    void networkError(QString err);

public slots:
    /**
     * @brief parseNetworkResponse Verifica si existió algún error en la respuesta del servidor
     * @param finished Respuesta obtenida de la solicitud HTTP al servidor
     */
    void parseNetworkResponse(QNetworkReply *finished);


private:
    // Network Access Manager para controlar las solicitudes al servidor
    QNetworkAccessManager m_nam;
};

#endif // APICONTROLLER_H
