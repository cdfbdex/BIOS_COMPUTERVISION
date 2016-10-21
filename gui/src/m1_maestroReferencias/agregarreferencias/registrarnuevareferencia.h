#ifndef REGISTRARNUEVAREFERENCIA_H
#define REGISTRARNUEVAREFERENCIA_H

#include "MABE_core"

#include "seleccionarroi.h"
#include "agregarroi.h"

#include <QWizard>
#include <QCloseEvent>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QDesktopWidget>
#include <QInputDialog>

#include <QHttpMultiPart>

#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>


namespace Ui {
class RegistrarNuevaReferencia;
}

class RegistrarNuevaReferencia : public QWizard
{
    Q_OBJECT

public:
    explicit RegistrarNuevaReferencia(QWidget *parent = 0);
    ~RegistrarNuevaReferencia();

private:
    void initializeObjects();
    void connectSignalSlot();
    ///
    /// \brief generateJsonFromROI
    /// \return Estructura json con todas las ROI seleccionadas agrupadas por origen de imagen
    ///
    QJsonObject generateJsonFromROI();

protected:
    virtual void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    virtual bool validateCurrentPage() Q_DECL_OVERRIDE;

signals:
    closeWindow();

private slots:
    // SLOTS DEL WEBSERVICE
    void handleResponse(QByteArray response);
    void handleNetworkError(QString errorString);

    // SLOTS DE OBJETOS DEL GUI
    void analizarReferencia();
    void obtenerListadoAccesorios();
    void capturarImagenesKinect();
    void procesarImagenes(int id, const QImage &buffer);
    void nuevaImagenParaAsignarROI();
    void mostrarDialogoAgregarROI(QRect _roiRectangle);
    void agregarROIaLista(QString accesoryCode);
    void eliminarROIdeLista();
    void eliminarTodasLasROI();
    void guardarReferenciaLocal();
    void guardarReferenciaEnFIS();
    void referenciaRegistrada();

private:

    Ui::RegistrarNuevaReferencia *ui;

    // VARIABLES DE WEBSERVICE
    int LastWebServiceRequested = RQ_NULL;
    APIController *m_apiControl;
    QByteArray serverResponse;

    // VARIABLES
    // Validación
    bool referenceAnalized = false, photosCaptured = false, roiSelected = false, referenceSaved = false;

    // Análisis de referencia
    QString referenceId;
    QStringList codesAccesories;
    QStringList descripAccesories;

    // Captura de imágenes
    QCamera *camera;
    QCameraImageCapture *imageCapture;

    QString dateTimeStamp;
    QList<QPixmap> *capturedPhotosList;
    QStringList capturedImagesCodes;

    // Seleccionar ROI
    QStandardItemModel *modeloROIs;
    AgregarROI *roiDialog;
    QRect selectedROI;
};

#endif // REGISTRARNUEVAREFERENCIA_H
