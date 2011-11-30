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

#include <QTableView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTextEdit>
#include <QSqlRecord>
#include <QMenu>
#include <QDesktopServices>
#include <QUrl>
#include <QToolBar>
#include "databaseView.h"
#include "previewWindow.h"
#include "postTableModel.h"
#include "../strings/strings.h"

namespace gui
{

DatabaseView::DatabaseView(const QSqlDatabase &db, const QString &table,
                           QWidget *parent, Qt::WindowFlags f)
    :
        QWidget(parent, f),
        m_view(0),
        m_model(0)
{
    // Create the table view for database items.
    m_view = new QTableView(this);
    m_view->setShowGrid(false);
    m_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_view->verticalHeader()->setVisible(false);
    m_view->setContextMenuPolicy(Qt::CustomContextMenu);
    m_view->setSortingEnabled(true);
    connect(m_view, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(onDatabaseContextMenu(const QPoint &)));

    // The uniformed rows height.
    QFontMetrics fm = fontMetrics();
    m_view->verticalHeader()->setDefaultSectionSize(fm.height() + 6);

    QSplitter *splitter = new QSplitter(Qt::Vertical, this);
    splitter->addWidget(m_view);

    m_preview = new PreviewWindow(this);
    splitter->addWidget(m_preview);

    // Create and configure tool bar actions.
    m_openSelected = new QAction("&Open In Browser", this);
    m_openSelected->setIcon(QIcon(":/icons/web"));
    m_openSelected->setShortcut(QKeySequence(Qt::Key_Return));
    m_openSelected->setEnabled(false);
    connect(m_openSelected, SIGNAL(triggered()), this, SLOT(onOpenSelectedInBrowser()));

    m_removeSelected = new QAction("&Remove Selected", this);
    m_removeSelected->setIcon(QIcon(":/icons/remove_record"));
    m_removeSelected->setShortcut(QKeySequence(Qt::Key_Delete));
    m_removeSelected->setEnabled(false);
    connect(m_removeSelected, SIGNAL(triggered()), this, SLOT(onRemoveSelected()));

    QAction *act = new QAction("Remove &All", this);
    act->setIcon(QIcon(":/icons/remove_all"));
    connect(act, SIGNAL(triggered()), this, SLOT(onRemoveAll()));

    QToolBar *toolbar = new QToolBar(this);
    toolbar->setIconSize(QSize(16, 16));
    toolbar->addAction(m_openSelected);
    toolbar->addSeparator();
    toolbar->addAction(m_removeSelected);
    toolbar->addAction(act);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(toolbar);
    mainLayout->addWidget(splitter);
    setLayout(mainLayout);

    init(db, table);
}

DatabaseView::~DatabaseView()
{
    m_model->submitAll();
}

void DatabaseView::init(const QSqlDatabase &db, const QString &table)
{
    m_model = new PostTableModel(this, db);
    m_model->setTable(table);
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_model->select();

    Q_ASSERT(m_view);
    m_view->setModel(m_model);
    connect(m_view->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(onSelectionChanged(const QItemSelection &, const QItemSelection &)));

    // Hide content column for now.
    for (int i = 0; i < m_model->columnCount(); ++i) {
        QString title =
            m_model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
        if (title != str::TagTitle &&
            title != str::TagName &&
            title != str::TagUpdated &&
            title != "flag" &&
            title != "read") {
            m_view->hideColumn(i);
        }
    }

    // Add and configure first custom column in the table.
    m_model->setHeaderData(0, Qt::Horizontal, QString());
    m_model->setHeaderData(2, Qt::Horizontal, QString());
    m_view->horizontalHeader()->resizeSection(0, 24);
    m_view->horizontalHeader()->resizeSection(2, 24);
    m_view->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    m_view->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
}

void DatabaseView::updateTable()
{
    if (m_model && m_model->database().isValid()) {
        beforeUpdate();

        m_model->select();
        // It selects the first 256 records only. In case of having
        // more records in the table table view will not update
        // properly. Therefore we need to fetch more if more
        // records available.
        while (m_model->canFetchMore()){
            m_model->fetchMore();
        }

        afterUpdate();
    }
}

bool DatabaseView::hasTable(const QSqlDatabase &db, const QString &table) const
{
    return m_model &&
           m_model->database().driver() == db.driver() &&
           m_model->database().connectionName() == db.connectionName() &&
           m_model->tableName() == table;
}

void DatabaseView::onSelectionChanged(const QItemSelection &selected,
                                      const QItemSelection &deselected)
{
    QModelIndexList indexes = selected.indexes();
    // Handle the state of the actions.
    bool enable = indexes.size() > 0;
    m_openSelected->setEnabled(enable);
    m_removeSelected->setEnabled(enable);

    // Set the content of preview window.
    foreach(const QModelIndex &index, indexes) {
        QSqlRecord record = m_model->record(index.row());
        QString colName = record.fieldName(index.column());
        if (colName == str::TagContent)
            m_preview->setText(record.value(index.column()).toString());
        else if (colName == str::TagName)
            m_preview->setAuthor(record.value(index.column()).toString());
        else if (colName == str::TagLink)
            m_preview->setUrl(record.value(index.column()).toString());
        else if (colName == str::TagTitle)
            m_preview->setTitle(record.value(index.column()).toString());

        // Mark the row as read.
        record.setValue(str::TagRead, true);
        m_model->setRecord(index.row(), record);
        beforeUpdate();
        m_model->submitAll();
        afterUpdate();
    }
}

void DatabaseView::onDatabaseContextMenu(const QPoint &pos)
{
    QMenu menu;
    menu.addAction(m_openSelected);
    menu.addAction(m_removeSelected);
    menu.exec(m_view->mapToGlobal(QPoint(pos.x(), pos.y() + 20)));
}

void DatabaseView::onOpenSelectedInBrowser()
{
    QModelIndexList indexes = m_view->selectionModel()->selectedIndexes();
    foreach(const QModelIndex &index, indexes) {
        QSqlRecord record = m_model->record(index.row());
        if (record.fieldName(index.column()) == str::TagLink) {
            QDesktopServices::openUrl(QUrl(record.value(index.column()).toString(),
                                      QUrl::TolerantMode));
        }
    }
}

void DatabaseView::onRemoveSelected()
{
    QModelIndexList indexes = m_view->selectionModel()->selectedRows();
    foreach(const QModelIndex &index, indexes) {
        m_model->removeRows(index.row(), 1);
    }

    m_model->submitAll();
}

void DatabaseView::onRemoveAll()
{
    m_model->removeRows(0, m_model->rowCount());
    m_model->submitAll();
}

void DatabaseView::beforeUpdate()
{
    // Store selected row numbers in the list in order to
    // restore selection after update. We don't store selected
    // indexes because they will be invalidated after model
    // updates itself.
    QModelIndexList indexes = m_view->selectionModel()->selectedRows();
    m_selectedRows.clear();
    foreach(const QModelIndex &index, indexes) {
        m_selectedRows.push_back(index.row());
    }

    // Disconnect selection related signal to prevent selection
    // handling while model is updating. Restore this connection
    // after update.
    disconnect(m_view->selectionModel(),
               SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
               this,
               SLOT(onSelectionChanged(const QItemSelection &, const QItemSelection &)));
}

void DatabaseView::afterUpdate()
{
     foreach(int row, m_selectedRows) {
            QModelIndex index = m_model->index(row, 0);
            m_view->selectionModel()->select(index, QItemSelectionModel::Select |
                                                    QItemSelectionModel::Rows);
     }

    connect(m_view->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this,
            SLOT(onSelectionChanged(const QItemSelection &, const QItemSelection &)));
}

} // namespace gui
