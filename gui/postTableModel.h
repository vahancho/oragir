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

#ifndef __POSTTABLEMODEL_H__
#define __POSTTABLEMODEL_H__

#include <QSqlTableModel>

namespace gui
{

class PostTableModel : public QSqlTableModel
{
public:
    PostTableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());

    /// Stores the visible columns numbers (indexes).
    enum ColumnNumbers
    {
        Star = 0,
        Title,
        Read,
        Name,
        Updated,
        PosterId,
        Link,
        Content,
        UserPic,
        Category
    };

    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    virtual QVariant data(const QModelIndex &index,
                          int role = Qt::DisplayRole) const;

private:
    QIcon m_titleStar;
    QIcon m_emptyStar;
    QIcon m_titleRead;
};

} // namespace gui

#endif // __POSTTABLEMODEL_H__
