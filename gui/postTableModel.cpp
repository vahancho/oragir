/**************************************************************************
*   Copyright (C) 2011 by Vahan Aghajanyan                                *
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
#include "postTableModel.h"

namespace gui
{

PostTableModel::PostTableModel(QObject *parent, QSqlDatabase db)
    :
        QSqlTableModel(parent, db)
{
    m_iconTitleStar = QIcon(":/icons/star_off");
    m_iconStarOff = QIcon(":/icons/star_empty");
    m_iconStarOn = QIcon(":/icons/star_on");
    m_iconTitleRead = QIcon(":/icons/ball_green");
    m_iconUnread = QIcon(":/icons/ball_red");

    // Initialize visible columns title names.
    m_columnName[Star] = QString();
    m_columnName[Title] = QString("Title");
    m_columnName[Read] = QString();
    m_columnName[Name] = QString("Name");
    m_columnName[Updated] = QString("Date");
}

QVariant PostTableModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DecorationRole) {
            if (section == Star)
                return m_iconTitleStar;
            else if (section == Read)
                return m_iconTitleRead;
        } else if (role == Qt::DisplayRole) {
                return m_columnName[(ColumnNumber)section];
        }
    }

    return QSqlTableModel::headerData(section, orientation, role);
}

QVariant PostTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case Qt::DisplayRole:
        if (index.column() == Star || index.column() == Read)
            return QString();
        break;
    case Qt::DecorationRole:
        if (index.column() == Star) {
            if (flag(index.row()) == 0)
                return m_iconStarOff;
            else
                return m_iconStarOn;
        } else if (index.column() == Read) {
            if (isRead(index.row()))
                return m_iconTitleRead;
            else
                return m_iconUnread;
        }
        break;
    case Qt::FontRole:
        {
        QFont f;
        f.setBold(!isRead(index.row()));
        return f;
        }
    default:
        break;
    }
    return QSqlTableModel::data(index, role);
}

bool PostTableModel::isRead(int row) const
{
    QSqlRecord rec = record(row);
    return rec.value(Read).toBool();
}

int PostTableModel::flag(int row) const
{
    QSqlRecord rec = record(row);
    return rec.value(Star).toInt();
}

} // namespace gui
