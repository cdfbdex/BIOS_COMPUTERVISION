#include "referenciaundrefrigeracion.h"

ReferenciaUndRefrigeracion::ReferenciaUndRefrigeracion(QObject *parent)
    :QAbstractTableModel(parent)
{
    headers << tr("ID") << tr("Fecha/Hora") << tr("Referencia") << tr("Activada");
}

ReferenciaUndRefrigeracion::~ReferenciaUndRefrigeracion()
{

}

void ReferenciaUndRefrigeracion::setDataToModel(QByteArray jsonData)
{
    // Convertir respuesta del servidor en QJsonObject para extraer la información fácilmente
    QJsonObject jsonReceived = JsonController::readJSONFromByteArray(jsonData);
    qDebug() << "MODEL DATA SIZE: " << jsonReceived.size();

/*
    QVector<QString> rowData;
    rowData.append("1.1");
    rowData.append("1.2");
    rowData.append("1.3");
    tableData.append(rowData);

    QVector<QString> rowData2;
    rowData2.append("2.1");
    rowData2.append("2.2");
    rowData2.append("2.3");
    tableData.append(rowData2);
*/

    foreach (const QJsonValue &referencia, jsonReceived)
    {
        QVector<QString> rowData;

        QJsonObject refObj(referencia.toObject());

        rowData.append(refObj["id"].toString());
        rowData.append(refObj["fechaHora"].toString());
        rowData.append(refObj["id_referencia"].toString());
        rowData.append(refObj["activar"].toString());

        tableData.append(rowData);

/*
        qDebug() << "VALUE: " << value;
        switch(value.type())
        {
        case QJsonValue::Object:
            foreach (const QJsonValue &value2, value.toObject())
            {
                qDebug() << "VALUE 2: " << value2;
            }
            break;
        case QJsonValue::Array:
            break;
        case QJsonValue::Double:
            break;
        case QJsonValue::String:
            break;
        case QJsonValue::Bool:
            break;
        case QJsonValue::Null:
            qDebug() << "JsonValue is NULL";
            break;
        case QJsonValue:: Undefined:
        default:
            qDebug() << "JsonValue is UNDEFINED";
            break;
        }
        qDebug() << "\n\n";
*/

    }
}

QVariant ReferenciaUndRefrigeracion::headerData(int section, Qt::Orientation orientation, int role/* = Qt::DisplayRole*/) const
{
    Q_UNUSED(orientation);

    if (role == Qt::DisplayRole)
    {
        return headers.at(section);
    }
    return QVariant();
}

Qt::ItemFlags ReferenciaUndRefrigeracion::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
}

int ReferenciaUndRefrigeracion::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
//    if(tableData.isEmpty())
//    {
//        qDebug() << "rowCount: 1";
//        return 1;
//    }
//    else
//    {
//        qDebug() << "rowCount: " << tableData.size();
//        return tableData.size();
//    }
    return 5;
}

int ReferenciaUndRefrigeracion::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

//    if(tableData.isEmpty())
//    {
//        qDebug() << "colCount: 1";
//        return 1;
//    }
//    else
//    {
//        qDebug() << "colCount: " << tableData.at(0).size();
//        return tableData.at(0).size();
//    }
    return 4;
}

QVariant ReferenciaUndRefrigeracion::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    switch(role){
    case Qt::DisplayRole:
//        if (row == 0 && col == 1) return QString("<--left");
//        if (row == 1 && col == 1) return QString("right-->");
//        return QString("Row%1, Column%2")
//                .arg(row + 1)
//                .arg(col +1);
        if(tableData.isEmpty())
            return QString();
        else
            return tableData.at(row).at(col);

        break;
    case Qt::FontRole:
        if (row == 0 && col == 0) //change font only for cell(0,0)
        {
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
        break;
    case Qt::BackgroundRole:
        if (row == 1 && col == 2)  //change background only for cell(1,2)
        {
            QBrush redBackground(Qt::red);
            return redBackground;
        }
        break;
    case Qt::TextAlignmentRole:
        if (row == 1 && col == 1) //change text alignment only for cell(1,1)
        {
            return Qt::AlignRight + Qt::AlignVCenter;
        }
        break;
    case Qt::CheckStateRole:
        if (row == 1 && col == 0) //add a checkbox to cell(1,0)
        {
            return Qt::Checked;
        }
        break;
    }
    return QVariant();
}
