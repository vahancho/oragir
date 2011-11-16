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

#include <QLineEdit>
#include <QGroupBox>
#include <QLayout>
#include <QRadioButton>
#include <QLabel>
#include <QTreeWidget>
#include <QPushButton>
#include "dlgFilterEditor.h"
#include "../strings/guiStrings.h"

namespace gui
{

FilterEditor::FilterEditor(QWidget *parent, Qt::WindowFlags f)
    :
        QDialog(parent, f)
{
    QLabel *lblName = new QLabel("Name:", this);
    QLineEdit *editName = new QLineEdit(this);
    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->addWidget(lblName);
    nameLayout->addWidget(editName);

    QGroupBox *groupBox = new QGroupBox("Matching Criterion", this);
    QRadioButton *radio1 = new QRadioButton("&All rules matched");
    QRadioButton *radio2 = new QRadioButton("One of the rules matched");
    radio1->setChecked(true);

    QHBoxLayout *radioLayout = new QHBoxLayout;
    radioLayout->addWidget(radio1);
    radioLayout->addWidget(radio2);
    radioLayout->addStretch(1);
    groupBox->setLayout(radioLayout);

    QTreeWidget *rules = new QTreeWidget;
    rules->setColumnCount(3);
    rules->setRootIsDecorated(false);
    QStringList headerLabels;
    headerLabels << "Property" << "Option" << "Value";
    rules->setHeaderLabels(headerLabels);

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
    mainLayout->addLayout(nameLayout);
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(rules);
    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);
}

} // namespace gui
