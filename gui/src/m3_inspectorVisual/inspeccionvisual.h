#ifndef INSPECCIONVISUAL_H
#define INSPECCIONVISUAL_H

#include <QWidget>
#include <QCloseEvent>

#include "MABE_core"

namespace Ui {
class InspeccionVisual;
}

class InspeccionVisual : public QWidget
{
    Q_OBJECT



public:
    explicit InspeccionVisual(QWidget *parent = 0);
    ~InspeccionVisual();

private:
    void connectSignalSlot();
    void initializeObjects();

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
    void close_onClicked();
    void accesoriesList_onClicked();
    void errorDetails_onClicked();
    //Visualización de imágenes
    void resumenImg_onClicked();
    void rgbImg_onClicked();
    void depthImg_onClicked();
    void irImg_onClicked();

private:
    Ui::InspeccionVisual *ui;

    // VARIABLES DE WEBSERVICE
    int LastWebServiceRequested = RQ_NULL;
    APIController *m_apiControl;
    QByteArray serverResponse;
};

#endif // INSPECCIONVISUAL_H
