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
#include <QHeaderView>
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
    toolBar->setIconSize(QSize(16, 16));
    toolBar->addAction(QIcon(":icons/filter_add"), "New Filter...",
                       this, SLOT(onNewFilter()));
    toolBar->addAction(QIcon(":icons/filter_edit"), "Change Filter...",
                       this, SLOT(onFilterEdit()));
    toolBar->addAction(QIcon(":icons/filter_delete"), "Delete",
                       this, SLOT(onFilterDelete()));

    m_filtersTree = new QTreeWidget(this);
    m_filtersTree->setColumnCount(2);
    m_filtersTree->setRootIsDecorated(false);
    QStringList headerLabels;
    headerLabels << "Filter" << "Enable";
    m_filtersTree->setHeaderLabels(headerLabels);
    m_filtersTree->header()->resizeSection(Name, 250);
    m_filtersTree->header()->setResizeMode(Enable, QHeaderView::Fixed);
    m_filtersTree->header()->setStretchLastSection(false);

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
    resize(400, 300);
}

void FiltersDialog::setFilters(const core::StreamDatabase::Filters &filters)
{
    core::StreamDatabase::Filters::const_iterator it = filters.begin();
    while (it != filters.end()) {
        const core::Filter<core::Post> &filter = *it;
        addFilterNode(filter);
        ++it;
    }

    // Finally select the first item if any.
    if (QTreeWidgetItem *node = m_filtersTree->topLevelItem(0)) {
        m_filtersTree->setCurrentItem(node);
    }
}

void FiltersDialog::onFilterEdit()
{
    if (QTreeWidgetItem *currentItem = m_filtersTree->currentItem()) {
        FilterEditor editor;
        editor.setFilter(m_filters[currentItem]);
        if (editor.exec() == QDialog::Accepted) {
            // Update the filter.
            core::Filter<core::Post> filter = editor.filter();
            m_filters[currentItem] = filter;
            currentItem->setText(Name, filter.name());
        }
    }
}

void FiltersDialog::onNewFilter()
{
    QString newName = QString("New Filter %1").arg(qrand());
    core::Filter<core::Post> filter(newName);
    addFilterNode(filter, true);
}

core::StreamDatabase::Filters FiltersDialog::filters() const
{
    core::StreamDatabase::Filters filters;
    std::map<QTreeWidgetItem *, core::Filter<core::Post> >::const_iterator it =
                                            m_filters.begin();
    while(it != m_filters.end()) {
        QTreeWidgetItem *node = (*it).first;
        core::Filter<core::Post> filter = (*it).second;
        filter.setEnabled(node->checkState(Enable) == Qt::Checked);
        filter.setName(node->text(Name));
        filters.insert(filter);
        ++it;
    }

    return filters;
}

void FiltersDialog::addFilterNode(const core::Filter<core::Post> &filter,
                                  bool edit)
{
    QTreeWidgetItem *node = new QTreeWidgetItem;
    node->setFlags(Qt::ItemIsSelectable |
                   Qt::ItemIsUserCheckable |
                   Qt::ItemIsEnabled |
                   Qt::ItemIsEditable);
    if (filter.enabled())
        node->setCheckState(Enable, Qt::Checked);
    else
        node->setCheckState(Enable, Qt::Unchecked);
    node->setText(Name, filter.name());
    node->setToolTip(Name, filter.name());
    m_filtersTree->addTopLevelItem(node);
    m_filters[node] = filter;

    if (edit)
        m_filtersTree->editItem(node, Name);
}

void FiltersDialog::onFilterDelete()
{
    if (QTreeWidgetItem *currentItem = m_filtersTree->currentItem()) {
        m_filters.erase(currentItem);
        int index = m_filtersTree->indexOfTopLevelItem(currentItem);
        m_filtersTree->takeTopLevelItem(index);
    }
}

} // namespace gui
