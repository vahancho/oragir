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
#include "dlgFilters.h"

namespace gui
{

FiltersDialog::FiltersDialog(QWidget *parent, Qt::WindowFlags f)
    :
        QDialog(parent, f)
{
    QTableWidget *tblFilters = new QTableWidget(this);

    QPushButton *btnAdd = new QPushButton("Add", this);
    QPushButton *btnRemove = new QPushButton("Remove", this);
    QVBoxLayout *addRemoveLayout = new QVBoxLayout;
    addRemoveLayout->addWidget(btnAdd);
    addRemoveLayout->addWidget(btnRemove);
    addRemoveLayout->addStretch();

    QHBoxLayout *tblWithBtns = new QHBoxLayout;
    tblWithBtns->addWidget(tblFilters);
    tblWithBtns->addLayout(addRemoveLayout);

    QPushButton *btnOk = new QPushButton("&OK", this);
    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
    QPushButton *btnCancel = new QPushButton("&Cancel", this);
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

} // namespace gui
