#ifndef EVENTLOGGERCONTROLLER_H
#define EVENTLOGGERCONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QMessageBox>

#include "MABE_core"
#include "apicontroller.h"

class EventLoggerController : public QObject
{
    Q_OBJECT

public:
    explicit EventLoggerController(QObject *parent = 0);

    void generateEvent(QString user, QString codeEvent, QString detailEvent);

signals:
    void eventLogResponseReady(bool eventProcessed);

private:
    void initializeObjects();
    void connectSignalSlot();

protected:


private slots:
    // SLOTS DEL WEBSERVICE
    void handleResponse(QByteArray response);
    void handleNetworkError(QString errorString);

private:
    int LastWebServiceRequested;
    APIController *m_apiControl;
    QByteArray serverResponse;

};

#endif // EVENTLOGGERCONTROLLER_H
