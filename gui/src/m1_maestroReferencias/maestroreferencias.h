#ifndef MAESTROREFERENCIAS_H
#define MAESTROREFERENCIAS_H

#include "MABE_core"

#include "agregarreferencias/registrarnuevareferencia.h"

#include <QWidget>
#include <QCloseEvent>
#include <QMessageBox>

#include <QStandardItemModel>

namespace Ui {
class MaestroReferencias;
}

class MaestroReferencias : public QWidget
{
    Q_OBJECT

public:
    explicit MaestroReferencias(QWidget *parent = 0);
    ~MaestroReferencias();

private:
    void initializeObjects();
    void connectSignalSlot();
    void llenarTablaReferenciasRegistradas();

protected:
    virtual void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

signals:
    closeWindow();

private slots:
    // SLOTS DEL WEBSERVICE
    void handleResponse(QByteArray response);
    void handleNetworkError(QString errorString);

    // SLOTS DE OBJETOS DEL GUI
    void actualizarListaReferenciasUndRefrigeracion();

    void cerrar_onClicked();
    void actualizarDatos_onClicked();
    void registrarReferencia_onClicked();

    // Filtro de datos en tabla
    void filtrar_onClicked();
    void filterColumnChanged(int _index);

private:
    Ui::MaestroReferencias *ui;
    RegistrarNuevaReferencia *ui_nuevaRef;

    //ReferenciaUndRefrigeracion *modeloListadoRefs;
    QStandardItemModel *modeloRefs;
    MySortFilterProxyModel *proxyModeloRefs;

    // VARIABLES DE WEBSERVICE
    int LastWebServiceRequested = RQ_NULL;
    APIController *m_apiControl;
    QByteArray serverResponse;
};

#endif // MAESTROREFERENCIAS_H
