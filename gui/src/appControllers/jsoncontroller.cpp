#include "jsoncontroller.h"

JsonController::JsonController(QObject *parent): QObject(parent)
{

}

/// Funciones para manejo de imágenes

// Gets a Pixmap and converts it to JsonValue, encoded with BASE-64 and ready to be sent to web.
QJsonValue JsonController::jsonValFromPixmap(const QPixmap & p,const char* format)
{
    QByteArray data;
    QBuffer buffer { &data };
    buffer.open(QIODevice::WriteOnly);
    p.save(&buffer, format);
    //qDebug() << << data;

    auto encoded = buffer.data().toBase64();
    return QJsonValue(QString::fromLatin1(encoded));
}

// Gets a BASE-64 string encoding an image, and converts it to QPixmap with specified format
QPixmap JsonController::pixmapFromJsonValue(const QJsonValue & val, const char* format)
{
    QByteArray encoded = val.toString().toLatin1();
    QPixmap p;
    p.loadFromData(QByteArray::fromBase64(encoded), format);
    return p;
}


/// Funciones para manejo de archivos

QJsonValue JsonController::createJsonArrayFromQStringList(QStringList list)
{
    QJsonArray jsonParameters;

    //Recorre una lista de QString y genera un arreglo en estructura JSON
    for (int i=0;i<list.count();i++)
    {
        QString item = list.at(i);
        jsonParameters.append(QJsonValue(item));
        //qDebug() << item;
    }
    return QJsonValue(jsonParameters);
}

QByteArray JsonController::createJSONDocument(QJsonValue params, QString method)
{
    // Crea la estructura JSON para un servidor tipo JSON-RPC, incluyendo parámetros y la función a ejecutar

    // Parámetros de la solicitud POST para JSON-RPC
    QJsonObject jsonObject;
    jsonObject.insert("method",QJsonValue(method));
    jsonObject.insert("jsonrpc",QJsonValue("2.0"));
    jsonObject.insert("id",QJsonValue(0));
    jsonObject.insert("params",params);

    // Crear documento JSON
    QJsonDocument jsonDocument(jsonObject);
    return jsonDocument.toJson();
}

QByteArray JsonController::createJSONDocument(QHash<QString, QVariant> dataHash)
{
    // Crea un documento JSON a partir de un par <QString, QVariant>
    QJsonObject jsonObject;

    // Recorrer la estructura Hash
    QHash<QString, QVariant>::const_iterator i = dataHash.constBegin();
    while (i != dataHash.constEnd())
    {
        //qDebug() << i.key() << ": " << i.value();
        // Conversión de Hash a JSON
        jsonObject.insert(i.key(),QJsonValue::fromVariant(i.value()));
        ++i;
    }

    // Crear documento JSON
    QJsonDocument jsonDocument(jsonObject);
    return jsonDocument.toJson();
}

QJsonObject JsonController::readJSONFromFile(QString pathToFile)
{
    // Abre un archivo JSON de un directorio especificado y lo convierte en un JsonObject manipulable

    // Directorio del archivo
    QFile jsonFile(QCoreApplication::applicationDirPath() + pathToFile);
    qDebug() << jsonFile.fileName();

    // Verificar si el archivo existe y se puede abrir
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Could not open JSON file";
        qDebug() << jsonFile.errorString();
        return QJsonObject();
    }

    // Crear estructura JSON manipulable del archivo de entrada
    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(jsonFile.readAll());
    if (jsonDoc.isNull()) {
        qDebug() << "Could not read data from JSON Document";
    }

    QJsonObject jsonObj = jsonDoc.object();
    return jsonObj;
}

QJsonObject JsonController::readJSONFromByteArray(QByteArray jsonData)
{
    // Conversión de QByteArray a QJsonObject
    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull()) {
        qDebug() << "Could not read data from JSON Document";
    }

    return jsonDoc.object();
}

QJsonObject JsonController::readJSONFromQString(QString jsonObjString)
{
    QJsonObject obj;
    QByteArray objBA = jsonObjString.toUtf8();
    QJsonDocument doc = QJsonDocument::fromJson(objBA);
    // check validity of the document
    if(!doc.isNull())
    {
       if(doc.isObject())
       {
           obj = doc.object();
       }
       else
       {
           qDebug() << "Document is not an object" << endl;
       }
    }
    else
    {
       qDebug() << "Invalid JSON...\n" << jsonObjString << endl;
    }
    return obj;
}

QVariant JsonController::getKeyFromJsonDocument(QByteArray jsonDocument, QString keyName)
{
    // Obtener el valor de una llave dentro de un documento JSON
    QJsonParseError jerror;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(jsonDocument,&jerror);
    // Verificar si hay errores
    if(jerror.error != QJsonParseError::NoError)
    {
        qDebug() << "Parsing error...";
        return QJsonValue("Error");
    }

    // Extracción del valor
    QJsonObject jsonObject = jsonResponse.object();
    QJsonValue result = jsonObject.value(keyName);

    return result.toVariant();
}
