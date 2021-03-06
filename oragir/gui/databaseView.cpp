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
	m_model(0),
        m_view(0)
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
    connect(m_view, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(onClicked(const QModelIndex &)));

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

    for (int i = 0; i < m_model->columnCount(); ++i) {
        // Hide all columns that follow the Updated column number.
        if (i > PostTableModel::Updated) {
            m_view->hideColumn(i);
        }
    }

    // Add and configure first custom column in the table.
    m_view->horizontalHeader()->resizeSection(PostTableModel::Star, 24);
    m_view->horizontalHeader()->resizeSection(PostTableModel::Read, 14);
    m_view->horizontalHeader()->setResizeMode(PostTableModel::Star, QHeaderView::Fixed);
    m_view->horizontalHeader()->setResizeMode(PostTableModel::Read, QHeaderView::Fixed);
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
        emit changed(table());
    }
}

void DatabaseView::onSelectionChanged(const QItemSelection &selected,
                                      const QItemSelection & /*deselected*/)
{
    QModelIndexList indexes = selected.indexes();
    // Handle the state of the actions.
    bool enable = indexes.size() > 0;
    m_openSelected->setEnabled(enable);
    m_removeSelected->setEnabled(enable);
}

void DatabaseView::onDatabaseContextMenu(const QPoint &pos)
{
    QMenu menu;
    menu.addAction(m_openSelected);
    menu.addAction(m_removeSelected);
    menu.addSeparator();

    bool read = false;
    bool unread = false;
    bool star = false;
    bool unstar = false;
    QModelIndexList indexes = m_view->selectionModel()->selectedRows();
    foreach(const QModelIndex &index, indexes) {
        QSqlRecord record = m_model->record(index.row());

        if (record.value(PostTableModel::Read).toBool())
            read = true;
        else
            unread = true;

        if (record.value(PostTableModel::Star).toInt() == 1)
            star = true;
        else
            unstar = true;
    }

    if (unread)
        menu.addAction("Mark As &Read", this, SLOT(onMarkAsRead()));
    if (read)
        menu.addAction("Mark As &Unread", this, SLOT(onMarkAsUnread()));
    if (unstar)
        menu.addAction("&Add Star", this, SLOT(onAddStar()));
    if (star)
        menu.addAction("Remove &Star", this, SLOT(onRemoveStar()));

    // Finally show the menu.
    menu.exec(m_view->mapToGlobal(QPoint(pos.x(), pos.y() + 20)));
}

void DatabaseView::onMarkAsRead()
{
    QModelIndexList indexes = m_view->selectionModel()->selectedRows();
    foreach(const QModelIndex &index, indexes) {
        QSqlRecord record = m_model->record(index.row());
        record.setValue(PostTableModel::Read, true);
        m_model->setRecord(index.row(), record);
    }
    beforeUpdate();
    m_model->submitAll();
    afterUpdate();

    emit changed(table());
}

void DatabaseView::onMarkAsUnread()
{
    QModelIndexList indexes = m_view->selectionModel()->selectedRows();
    foreach(const QModelIndex &index, indexes) {
        QSqlRecord record = m_model->record(index.row());
        record.setValue(PostTableModel::Read, false);
        m_model->setRecord(index.row(), record);
    }
    beforeUpdate();
    m_model->submitAll();
    afterUpdate();

    emit changed(table());
}

void DatabaseView::onAddStar()
{
    QModelIndexList indexes = m_view->selectionModel()->selectedRows();
    foreach(const QModelIndex &index, indexes) {
        QSqlRecord record = m_model->record(index.row());
        record.setValue(PostTableModel::Star, 1);
        m_model->setRecord(index.row(), record);
    }
    beforeUpdate();
    m_model->submitAll();
    afterUpdate();
}

void DatabaseView::onRemoveStar()
{
     QModelIndexList indexes = m_view->selectionModel()->selectedRows();
    foreach(const QModelIndex &index, indexes) {
        QSqlRecord record = m_model->record(index.row());
        record.setValue(PostTableModel::Star, 0);
        m_model->setRecord(index.row(), record);
    }
    beforeUpdate();
    m_model->submitAll();
    afterUpdate();
}

void DatabaseView::onOpenSelectedInBrowser()
{
    QModelIndexList indexes = m_view->selectionModel()->selectedRows();
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
    emit changed(table());
}

void DatabaseView::onRemoveAll()
{
    m_model->removeRows(0, m_model->rowCount());
    m_model->submitAll();
    m_preview->clear();
    emit changed(table());
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
}

void DatabaseView::afterUpdate()
{
     foreach(int row, m_selectedRows) {
            QModelIndex index = m_model->index(row, 0);
            m_view->selectionModel()->select(index, QItemSelectionModel::Select |
                                                    QItemSelectionModel::Rows);
     }
}

void DatabaseView::onClicked(const QModelIndex &index)
{
    QSqlRecord record = m_model->record(index.row());
    m_preview->setText(record.value(PostTableModel::Content).toString());
    m_preview->setAuthor(record.value(PostTableModel::Name).toString());
    m_preview->setUrl(record.value(PostTableModel::Link).toString());
    m_preview->setTitle(record.value(PostTableModel::Title).toString());

    if (record.value(PostTableModel::Read) == false) {
        // Mark the row as read.
        record.setValue(PostTableModel::Read, true);
    }

    if (index.column() == PostTableModel::Star) {
        // Revert the row flag.
        int flag = record.value(PostTableModel::Star).toInt();
        if (flag == 0)
            record.setValue(PostTableModel::Star, 1);
        else if (flag == 1)
            record.setValue(PostTableModel::Star, 0);
    }

    m_model->setRecord(index.row(), record);
    beforeUpdate();
    m_model->submitAll();
    afterUpdate();

    emit changed(table());
}

QString DatabaseView::table() const
{
    return m_model->tableName();
}

} // namespace gui
