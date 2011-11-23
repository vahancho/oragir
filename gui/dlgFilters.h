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

#ifndef __DLGFILTERS_H__
#define __DLGFILTERS_H__

#include <QDialog>
#include "../database/database.h"

class QTreeWidget;
class QTreeWidgetItem;

namespace gui
{

class FiltersDialog : public QDialog
{
    Q_OBJECT

public:
    FiltersDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);

    /// Adds new filter item to dialog.
    void setFilters(const core::Database::Filters &filters);

    /// Returns the temporary stored list of filters.
    core::Database::Filters filters() const;

private slots:
    void onFilterEdit();
    void onNewFilter();
    void onFilterDelete();

private:
    enum FiltersTreeColumn
    {
        Enable = 0,
        Name
    };

    /// Adds new top level node to the tree configured as filter.
    void addFilterNode(const core::Filter<core::Post> &filter);

    QTreeWidget *m_filtersTree;
    std::map<QTreeWidgetItem *, core::Filter<core::Post> > m_filters;
};

} // namespace gui

#endif // __DLGFILTERS_H__
