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
    toolBar->addAction("New Filter...");
    toolBar->addAction("Change Filter...", this, SLOT(onFilterEdit()));
    toolBar->addAction("Delete");

    m_filters = new QTreeWidget;
    m_filters->setColumnCount(2);
    m_filters->setRootIsDecorated(false);
    QStringList headerLabels;
    headerLabels << "" << "Filter";
    m_filters->setHeaderLabels(headerLabels);

    QVBoxLayout *tblWithBtns = new QVBoxLayout;
    tblWithBtns->addWidget(toolBar);
    tblWithBtns->addWidget(m_filters);

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

void FiltersDialog::addFilter(const QString &name, bool enabled)
{
    QTreeWidgetItem *node = new QTreeWidgetItem;
    if (enabled)
        node->setCheckState(0, Qt::Checked);
    else
        node->setCheckState(0, Qt::Unchecked);
    node->setText(1, name);
    node->setToolTip(1, name);
    m_filters->addTopLevelItem(node);
    m_filters->resizeColumnToContents(0);
}

void FiltersDialog::onFilterEdit()
{
    FilterEditor editor;
    if (editor.exec() == QDialog::Accepted) {
    }
}

} // namespace gui
