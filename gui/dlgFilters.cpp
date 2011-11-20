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

#include <QTreeWidget>
#include <QLayout>
#include <QPushButton>
#include <QToolBar>
#include "dlgFilters.h"
#include "dlgFilterEditor.h"
#include "../strings/guiStrings.h"

namespace gui
{

FiltersDialog::FiltersDialog(QWidget *parent, Qt::WindowFlags f)
    :
        QDialog(parent, f)
{
    QToolBar *toolBar = new QToolBar(this);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBar->addAction(QIcon(":icons/filter_add"), "New Filter...");
    toolBar->addAction(QIcon(":icons/filter_edit"), "Change Filter...",
                       this, SLOT(onFilterEdit()));
    toolBar->addAction(QIcon(":icons/filter_delete"), "Delete");

    m_filtersTree = new QTreeWidget;
    m_filtersTree->setColumnCount(2);
    m_filtersTree->setRootIsDecorated(false);
    QStringList headerLabels;
    headerLabels << "" << "Filter";
    m_filtersTree->setHeaderLabels(headerLabels);

    QVBoxLayout *tblWithBtns = new QVBoxLayout;
    tblWithBtns->addWidget(toolBar);
    tblWithBtns->addWidget(m_filtersTree);

    QPushButton *btnOk = new QPushButton(str::Ok, this);
    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
    btnOk->setDefault(true);
    QPushButton *btnCancel = new QPushButton(str::Cancel, this);
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(tblWithBtns);
    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);

    setWindowTitle("Filters");
}

void FiltersDialog::setFilters(const core::Database::Filters &filters)
{
    core::Database::Filters::const_iterator it = filters.begin();
    while (it != filters.end()) {
        const core::Filter<core::Post> &filter = *it;
        QTreeWidgetItem *node = new QTreeWidgetItem;
        if (filter.enabled())
            node->setCheckState(0, Qt::Checked);
        else
            node->setCheckState(0, Qt::Unchecked);
        node->setText(1, filter.name());
        node->setToolTip(1, filter.name());
        m_filtersTree->addTopLevelItem(node);
        m_filtersTree->resizeColumnToContents(0);
        m_filters[node] = filter;
        ++it;
    }
}

void FiltersDialog::onFilterEdit()
{
    if (QTreeWidgetItem *currentItem = m_filtersTree->currentItem()) {
        FilterEditor editor;
        editor.setFilter(m_filters[currentItem]);
        if (editor.exec() == QDialog::Accepted) {
            // Update the filter.
            m_filters[currentItem] = editor.filter();
        }
    }
}

core::Database::Filters FiltersDialog::filters() const
{
    core::Database::Filters filters;
    std::map<QTreeWidgetItem *, core::Filter<core::Post> >::const_iterator it =
                                            m_filters.begin();
    while(it != m_filters.end()) {
        QTreeWidgetItem *node = (*it).first;
        core::Filter<core::Post> filter = (*it).second;
        filter.setEnabled(node->checkState(0) == Qt::Checked);
        filters.insert(filter);
        ++it;
    }

    return filters;
}

} // namespace gui
