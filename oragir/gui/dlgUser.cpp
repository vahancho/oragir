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
#include <QFormLayout>
#include <QPushButton>
#include "dlgUser.h"
#include "../strings/guiStrings.h"

namespace gui
{

UserAccount::UserAccount(QWidget *parent, Qt::WindowFlags f)
    :
        QDialog(parent, f)
{
    m_leUser = new QLineEdit(this);
    m_lePassword = new QLineEdit(this);
    m_lePassword->setEchoMode(QLineEdit::Password);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("&User:"), m_leUser);
    formLayout->addRow(tr("&Password:"), m_lePassword);

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
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);
    setWindowTitle("User Account");
}

} // namespace gui
