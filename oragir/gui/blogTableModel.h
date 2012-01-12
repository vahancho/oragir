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

#ifndef __BLOGTABLEMODEL_H__
#define __BLOGTABLEMODEL_H__

#include <QSqlTableModel>

namespace gui
{

class BlogTableModel : public QSqlTableModel
{
public:
    BlogTableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());

    /// Stores the visible columns numbers (indexes).
    enum ColumnNumber
    {
        ItemId = 0,
        Subject,
        Event,
        PublicId,
        CommentCount,
        Time,
        Url,
        Tags,
        Security,
        Flag
    };

    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    virtual QVariant data(const QModelIndex &index,
                          int role = Qt::DisplayRole) const;

private:
    QMap<ColumnNumber, QString> m_columnName;
    QIcon m_iconBlog;
};

} // namespace gui

#endif // __BLOGTABLEMODEL_H__
