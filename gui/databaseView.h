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

#ifndef __DATABASEVIEW_H__
#define __DATABASEVIEW_H__

#include <QWidget>
#include <QSqlDatabase>

class QSqlTableModel;
class QTableView;

namespace gui
{

class DatabaseView : public QWidget
{
    Q_OBJECT

public:
    /// The constructor.
    DatabaseView(QWidget *parent = 0, Qt::WindowFlags f = 0);

    /// Destructor.
    ~DatabaseView();

    void init(const QSqlDatabase &db, const QString &table);

    void updateTable();

private:
    QSqlTableModel *m_model;
    QTableView *m_view;
};

} // namespace gui

#endif // __DATABASEVIEW_H__
