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

#ifndef __DLGOPTIONS_H__
#define __DLGOPTIONS_H__

#include <QDialog>
#include <QMap>

class QTreeWidget;
class QTreeWidgetItem;
class QStackedWidget;

namespace gui
{

class AbstractOptionsPage;

/// This class implements application's options dialog.
class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    /// Default constructor
    OptionsDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);

    /// Destructor
    ~OptionsDialog();

    /// Saves the defaults for all pages.
    void saveDefaults();

    /// Adds the given page to the pages carrier and corresponding node to navigation tree.
    void addPage(AbstractOptionsPage *page);

private slots:
    /// This slot called on tree view navigation.
    void onChangePage(QTreeWidgetItem * current, QTreeWidgetItem * previous);

private:
    /// Creates and sets up dialog's pages and corresponding nodes in navigation tree.
    void setupPages();

    /// Returns top level item with the given name if found or null.
    QTreeWidgetItem *itemExist(const QString &name, QTreeWidgetItem *parent) const;

    /// Returns child item of the given parent with the given name or null.
    QTreeWidgetItem *itemExist(const QString &name, QTreeWidget *parent) const;

    /// The dialog's navigation tree.
    QTreeWidget *m_navigationTree;

    /// Store the dialog's pages container.
    QStackedWidget *m_pages;

    /// Store the mapping between navigation items and pages indexes.
    QMap<QTreeWidgetItem *, int> m_pageMapping;
};

} // namespace gui

#endif // __DLGOPTIONS_H__
