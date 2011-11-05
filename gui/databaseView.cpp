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
        QWidget(parent, f),
        m_view(0),
        m_model(0)
{
    m_view = new QTableView(this);
    m_view->setAlternatingRowColors(true);
    m_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);

    // The uniformed rows height.
    QFontMetrics fm = fontMetrics();
    m_view->verticalHeader()->setDefaultSectionSize(fm.height() + 6);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_view);
    setLayout(mainLayout);
}

DatabaseView::~DatabaseView()
{}

void DatabaseView::init(const QSqlDatabase &db, const QString &table)
{
    Q_ASSERT(db.isValid());

    // If model already exists, delete it and create new one.
    if (m_model)
        delete m_model;

    m_model = new QSqlTableModel(this, db);
    m_model->setTable(table);
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_model->select();

    Q_ASSERT(m_view);
    m_view->setModel(m_model);

    // Hide content column for now.
    m_view->hideColumn(4);
}

void DatabaseView::updateTable()
{
    if (m_model && m_model->database().isValid()) {
        m_model->select();
        // It selects the first 256 records only. In case of having
        // more records in the table table view will not update
        // properly. Therefore we need to fetch more if more
        // records available.
        while (m_model->canFetchMore()){
            m_model->fetchMore();
        }
    }
}

} // namespace gui
