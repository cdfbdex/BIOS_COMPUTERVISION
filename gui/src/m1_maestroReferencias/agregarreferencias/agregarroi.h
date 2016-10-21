#ifndef AGREGARROI_H
#define AGREGARROI_H

#include <QDialog>
#include <QCompleter>
#include <QStringListModel>
#include <QDebug>

namespace Ui {
class AgregarROI;
}

class AgregarROI : public QDialog
{
    Q_OBJECT

public:
    explicit AgregarROI(QWidget *parent = 0);
    ~AgregarROI();

    void asignarListasParaCompletar(QStringList _codeList, QStringList _descripList);

private:
    void initializeObjects();
    void connectSignalSlot();

protected:
    virtual void accept() Q_DECL_OVERRIDE;

signals:
    void enviarCodigoAccesorio(QString code);

private slots:
    void tipoDeBusqueda();
    void obtenerCodigoDeDescripcion(int index);

private:
    Ui::AgregarROI *ui;
    QStringList codeList, descriptionList;
    QCompleter *searchCompleter;
    QString accesoryCode;
};

#endif // AGREGARROI_H
