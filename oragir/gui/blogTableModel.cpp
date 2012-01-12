/**************************************************************************
*   Copyright (C) 2012 by Vahan Aghajanyan                                *
*   vahancho@gmail.com                                                    *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include <QIcon>
#include <QFont>
#include <QSqlRecord>
#include <QDateTime>
#include "blogTableModel.h"

namespace gui
{

BlogTableModel::BlogTableModel(QObject *parent, QSqlDatabase db)
    :
        QSqlTableModel(parent, db)
{
    // Initialize visible columns title names.
    m_columnName[Subject] = QString("Subject");
    m_columnName[Time] = QString("Date");

    m_iconBlog = QIcon(":/icons/ball_green");
}

QVariant BlogTableModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return m_columnName[(ColumnNumber)section];
    }
    return QSqlTableModel::headerData(section, orientation, role);
}

QVariant BlogTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
        if (index.column() == Time) {
            QSqlRecord rec = record(index.row());
            QDateTime dt = rec.value(Time).toDateTime();
            // If the date is today, show only time, otherwise
            // show date and time.
            if (dt.date() == QDate::currentDate())
                return dt.time();
            else
                return dt;
        }
        break;
    case Qt::DecorationRole:
        if (index.column() == Subject) {
            return m_iconBlog;
        }
        break;
    case Qt::FontRole:
        {
        QFont f;
        return f;
        }
    default:
        break;
    }
    return QSqlTableModel::data(index, role);
}

} // namespace gui
