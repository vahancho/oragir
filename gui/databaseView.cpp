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

#include <QtGui>
#include <QSqlTableModel>
#include "databaseView.h"

namespace gui
{

DatabaseView::DatabaseView(QWidget *parent, Qt::WindowFlags f)
    :
        QWidget(parent, f)
{
    m_view = new QTableView(this);
}

DatabaseView::~DatabaseView()
{}

void DatabaseView::init(const QSqlDatabase &db, const QString &table)
{
    Q_ASSERT(db.isValid());

    m_model = new QSqlTableModel(this, db);
    m_model->setTable(table);
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_model->select();

    m_model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("URL"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("Time"));
    m_model->setHeaderData(3, Qt::Horizontal, tr("Name"));
    m_model->setHeaderData(4, Qt::Horizontal, tr("Content"));
    m_model->setHeaderData(5, Qt::Horizontal, tr("Title"));

    Q_ASSERT(m_view);
    m_view->setModel(m_model);
    m_view->setAlternatingRowColors(true);
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);

    // The uniformed rows height.
    QFontMetrics fm = fontMetrics();
    m_view->verticalHeader()->setDefaultSectionSize(fm.height() + 6);

    // Hide content column for now.
    m_view->hideColumn(4);
}

} // namespace gui
