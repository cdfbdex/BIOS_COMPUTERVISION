#ifndef REFERENCIAUNDREFRIGERACION_H
#define REFERENCIAUNDREFRIGERACION_H

#include "src/appControllers/jsoncontroller.h"

#include <QAbstractTableModel>
#include <QStandardItem>

#include <QDateTime>

// Clase para llenar el Widget tipo QTableWidget que contiene las referencias inscritas en el sistema.
class ReferenciaUndRefrigeracion : public QAbstractTableModel
{
    Q_OBJECT

public:
    ReferenciaUndRefrigeracion(QObject *parent);
    virtual ~ReferenciaUndRefrigeracion();

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void setDataToModel(QByteArray jsonPath);

private:
    QStringList headers;
    QVector< QVector<QString> > tableData;

//    int id;
//    QDateTime fechaHora;
//    QString lineaProduccion;
//    bool estaActiva;
//    QHash<QString,QByteArray> imgsOriginales;
//    QHash<QString,QByteArray> imgsAccesorios;
};

#endif // REFERENCIAUNDREFRIGERACION_H
