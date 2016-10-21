/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "mysortfilterproxymodel.h"

//! [0]
MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}
//! [0]

//! [1]
void MySortFilterProxyModel::setFilterMinimumDate(const QDateTime &date)
{
    minDate = date;
    invalidateFilter();
}
//! [1]

//! [2]
void MySortFilterProxyModel::setFilterMaximumDate(const QDateTime &date)
{
    maxDate = date;
    invalidateFilter();
}

//! [2]
void MySortFilterProxyModel::setFilterDateEnable(bool _value)
{
    applyDateFilter = _value;
}

//! [2]
void MySortFilterProxyModel::setDateColumn(const int dateColumn)
{
    dateColumnIndex = dateColumn;
}
//! [2]

//! [3]
bool MySortFilterProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    QModelIndex columnIndex = sourceModel()->index(sourceRow, dateColumnIndex, sourceParent);

    if(applyDateFilter)
    {
        /// TODO: Definir formato de fecha para habilitar nuevamente el filtro
//        qDebug() << sourceModel()->data(columnIndex).toString() <<
//                    QDateTime::fromString(sourceModel()->data(columnIndex).toString(),"dd/MM/yyyy hh:mm:ss AP").toString();

        return sourceModel()->data( sourceModel()->index(sourceRow, filterKeyColumn(), sourceParent)).toString().contains(filterRegExp())
         && dateInRange(sourceModel()->data(columnIndex).toDateTime());
    }
    else
    {
        return sourceModel()->data( sourceModel()->index(sourceRow, filterKeyColumn(), sourceParent)).toString().contains(filterRegExp());
    }
}
//! [3]

//! [7]
bool MySortFilterProxyModel::dateInRange(const QDateTime &date) const
{
    return (!minDate.isValid() || date > minDate)
            && (!maxDate.isValid() || date < maxDate);
}
//! [7]
