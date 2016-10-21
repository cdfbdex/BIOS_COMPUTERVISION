#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>

#include <MABE_core>

#include <QSettings>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

    void setPage(int index);
    static void setDefaultValues();

signals:
    void changeLanguage();

private:
    void connectSignalSlot();
    void initializeObjects();
    void readSettingsFromFile();

protected:
    virtual void closeEvent (QCloseEvent *event) Q_DECL_OVERRIDE;
    virtual void showEvent(QShowEvent *e) Q_DECL_OVERRIDE;

private slots:
    // SLOTS DEL WEBSERVICE
    void handleResponse(QByteArray response);
    void handleNetworkError(QString errorString);

    // SLOTS DE OBJETOS DEL GUI
    void valoresDefecto_onClicked();
    void calcularEspacioDisco_onClicked();
    void aceptar_onClicked();
    void aplicar_onClicked();
    void cancelar_onClicked();

private:
    Ui::SettingsWindow *ui;

    bool savedData = false;
    QSettings *settingsFile;

    int LastWebServiceRequested = RQ_NULL;
    APIController *m_apiControl;
    QByteArray serverResponse;
};

#endif // SETTINGSWINDOW_H
