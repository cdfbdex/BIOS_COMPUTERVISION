#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QtCore>

// Directorio de almacenamiento
const QString APP_DIRPATH = QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/MABE-SIV/");
const QString SETTINGS_FILEPATH = QString(APP_DIRPATH+"/Settings/mabeSIV.ini");

// const QString SERVER_URL = "https://appminsalud.bios.co:9000/";
// const QString SERVER_URL = "http://srvcol20:8080/wsfissiv/SivService.svc/";
const QString SERVER_URL = "http://190.0.58.94:10080/wssivservice/SivService.svc/";

/// Lectura de webservice
/*OK*/ const QString WS_LOGIN = SERVER_URL+"autenticarUsuario?usuario=%1&password=%2&linea=%3";
/*OK*/ const QString WS_LOGOUT = SERVER_URL+"cerrarSesion?usuario=%1&password=%2";
/*OK*/ const QString WS_EVENTLOG = SERVER_URL+"registrarEvento?fecha=%1&linea=%2&id_maquina=%3&id_operador=%4&id_evento=%5&detalle_logsiv=%6";
const QString WS_REFERENCELIST = SERVER_URL+"getListadoReferencias";
//const QString WS_ACCESORIESLIST = SERVER_URL+"getListadoAccesorios";
const QString WS_REFERENCEREGISTER = SERVER_URL+"registrarReferencia";

/// Lectura de JSON
//const QString WS_LOGIN = "/testJSON/autenticarUsuario.json";
//const QString WS_LOGOUT = "/testJSON/cerrarSesion.json";
//const QString WS_EVENTLOG = "/testJSON/eventLog.json";
//const QString WS_REFERENCELIST = "/testJSON/listadoReferencias.json";
const QString WS_ACCESORIESLIST = "/testJSON/listadoAccesorios.json";


/// TODO: Añadir tipos de request para cada posible tipo de request al servicio web.
enum {
    RQ_NULL = 0,
    RQ_EVENTLOG,
    RQ_LOGIN,
    RQ_LOGOUT,
    RQ_ACCESORIESLIST,
    RQ_REFERENCELIST,
    RQ_REFERENCEREGISTER
};

#endif // CONSTANTS_H
