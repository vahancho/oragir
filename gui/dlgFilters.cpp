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

#include <QTableWidget>
#include <QLayout>
#include <QPushButton>
#include <QToolBar>
#include "dlgFilters.h"
#include "../strings/guiStrings.h"

namespace gui
{

FiltersDialog::FiltersDialog(QWidget *parent, Qt::WindowFlags f)
    :
        QDialog(parent, f)
{
    QToolBar *toolBar = new QToolBar(this);
    toolBar->addAction("New Filter...");
    toolBar->addAction("Change Filter...");
    toolBar->addAction("Delete");

    m_tblFilters = new QTableWidget(this);
    m_tblFilters->setColumnCount(3);
    QStringList headerLabels;
    headerLabels << QString() << "Filter" << QString();
    m_tblFilters->setHorizontalHeaderLabels(headerLabels);

    QVBoxLayout *tblWithBtns = new QVBoxLayout;
    tblWithBtns->addWidget(toolBar);
    tblWithBtns->addWidget(m_tblFilters);

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
    QTableWidgetItem *item0 = new QTableWidgetItem;
    QTableWidgetItem *item1 = new QTableWidgetItem(name);
    QTableWidgetItem *item2 = new QTableWidgetItem;
    int row = m_tblFilters->rowCount();
    m_tblFilters->insertRow(row);
    m_tblFilters->setItem(row, 0, item0);
    m_tblFilters->setItem(row, 1, item1);
    m_tblFilters->setItem(row, 2, item2);
}

} // namespace gui
