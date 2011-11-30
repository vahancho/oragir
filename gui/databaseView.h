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

class QTableView;
class QItemSelection;

namespace gui
{

class PostTableModel;
class PreviewWindow;

class DatabaseView : public QWidget
{
    Q_OBJECT

public:
    /// The constructor.
    DatabaseView(const QSqlDatabase &db, const QString &table,
                 QWidget *parent = 0, Qt::WindowFlags f = 0);

    /// Destructor.
    ~DatabaseView();

    void updateTable();

    /// Returns true if view shows the given database table.
    bool hasTable(const QSqlDatabase &db, const QString &table) const;

private slots:
    void onSelectionChanged(const QItemSelection &selected,
                            const QItemSelection &deselected);
    void onDatabaseContextMenu(const QPoint &);

    /// Open selected rows with web browser.
    void onOpenSelectedInBrowser();

    void onRemoveSelected();

    void onRemoveAll();

private:

    /// Stores the visible columns numbers (indexes).
    enum ColumnNumbers
    {
        Star = 0,
        Title,
        Read,
        Name,
        Updated
    };
    void init(const QSqlDatabase &db, const QString &table);

    /// Call this function before refreshing the table model.
    /*!
        This function will backup selection for the table rows
        and store them in the list before table model updates.
        Call afterUpdate() function after updating the model.
    */
    void beforeUpdate();

    /// Call this function after updating the model.
    /*!
        This function will restore previous selection and selection
        handling signals connections.
    */
    void afterUpdate();

    PostTableModel *m_model;
    QTableView *m_view;
    PreviewWindow *m_preview;

    QAction *m_openSelected;
    QAction *m_removeSelected;

    /// Temporary stores slected rows indexes before model updates.
    QList<int> m_selectedRows;
};

} // namespace gui

#endif // __DATABASEVIEW_H__
