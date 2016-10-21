/**
  * @file jsoncontroller.h
  * @date 13/09/2016
  * @version 0.2
  * @author Centro de Bioinformática y Biología Computacional de Colombia - BIOS
  * @title JSON Controller
  * @brief Funciones para manipulación de estructura JSON.
  * @code
    Sin código de ejemplo
  * @endcode
  */

#ifndef JSONCONTROLLER_H
#define JSONCONTROLLER_H

#include <QObject>
#include <QtCore>
#include <QPixmap>

#include <QBuffer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDir>
#include <QFile>

#include <QDebug>

class JsonController : public QObject
{
    Q_OBJECT

public:
    JsonController(QObject *parent = 0);

    // Funciones para control de imágenes
    /**
     * @brief jsonValFromPixmap Convierte QPixmap a QJsonValue tipo String con codificación base64
     * @param p Imagen de entrada a ser codificada
     * @param format Formato en el que se guardará la imagen
     * @return QJsonValue con la imagen codificada en base64
     */
    static QJsonValue jsonValFromPixmap(const QPixmap & p,const char* format);

    /**
     * @brief pixmapFromJsonValue Convierte de cadena base64 a QPixmap
     * @param val Cadena de texto con imagen codificada en base64
     * @param format Formato en el que se encuentra la imagen
     * @return Imagen tipo QPixmap
     */
    static QPixmap pixmapFromJsonValue(const QJsonValue & val, const char* format);


    // Funciones para crear el JSON
    /**
     * @brief createJsonArrayFromQStringList Recorre una lista de QString y genera un arreglo en estructura JSON
     * @param list Lista de QString que se desea convertir a JSON
     * @return Estructura JSON tipo arreglo
     */
    static QJsonValue createJsonArrayFromQStringList(QStringList list);

    /**
     * @brief createJSONDocument Crea la estructura JSON para un servidor tipo JSON-RPC, incluyendo parámetros y la función a ejecutar
     * @param params Parámetros a enviar a la función ejecutada
     * @param method Método a ejecutar en la solicitud POST
     * @return Estructura JSON lista para ser enviada a un servidor JSON-RPC
     */
    static QByteArray createJSONDocument(QJsonValue params, QString method);

    /**
     * @brief createJSONDocument Crea un documento JSON a partir de un par <QString, QVariant>
     * @param dataHash Pares <QString,QVariant> que debe mapearse en estructura JSON
     * @return Estructura JSON formateada apropiadamente con los valores del Hash
     */
    static QByteArray createJSONDocument(QHash<QString,QVariant> dataHash);

    // Funciones para leer el JSON
    /**
     * @brief readJSONFromFile Abre un archivo JSON de un directorio especificado y lo convierte en un JsonObject manipulable
     * @param pathToFile Ruta del archivo, relativa a partir del directorio de ejecución de la aplicación.
     * @return Estructura JSON de tipo Objeto manipulable fácilmente internamente
     */
    static QJsonObject readJSONFromFile(QString pathToFile);

    /**
     * @brief readJSONFromByteArray Conversión de QByteArray a QJsonObject
     * @param jsonData Estructura JSON tipo QByteArray proveniente, por ejemplo, de la respuesta de un servicio web
     * @return Estructura JSON de tipo Objecto manipulable fácilmente internamente
     */
    static QJsonObject readJSONFromByteArray(QByteArray jsonData);

    /**
     * @brief readJSONFromQString Conversión de QString a QJsonObject
     * @param jsonObjString Cadena de texto con la estructura JSON
     * @return Estructura JSON del objeto asociado a la cadena de entrada
     */
    static QJsonObject readJSONFromQString(QString jsonObjString);

    /**
     * @brief getKeyFromJsonDocument Obtener el valor de una llave dentro de un documento JSON
     * @param jsonDocument Estructura JSON completa
     * @param keyName Nombre de la clave que se desea extraer del JSON, debe estar sobre el primer nivel
     * @return Valor de la llave indicada
     */
    static QVariant getKeyFromJsonDocument(QByteArray jsonDocument, QString keyName);

};

#endif // JSONCONTROLLER_H
