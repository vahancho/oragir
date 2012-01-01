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
#include <QTreeWidget>
#include <QHeaderView>
#include "dlgExport.h"
#include "../ljcommunicator/communicator.h"

namespace gui
{

ExportDialog::ExportDialog(QWidget *parent, Qt::WindowFlags f)
    :
        QDialog(parent, f)
{
    m_leUser = new QLineEdit(this);
    m_lePassword = new QLineEdit(this);
    m_lePassword->setEchoMode(QLineEdit::Password);
    QPushButton *btnVerify = new QPushButton("Verify user", this);
    connect(btnVerify, SIGNAL(clicked()), this, SLOT(onUserVerify()));
    QPushButton *btnGetRecords = new QPushButton("Get records", this);
    connect(btnGetRecords, SIGNAL(clicked()), this, SLOT(onGetRecords()));

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("&User:"), m_leUser);   
    formLayout->addRow(tr("&Password:"), m_lePassword);
    formLayout->addRow("", btnVerify);
    formLayout->addRow("", btnGetRecords);

    // Posts
    m_recordsTree = new QTreeWidget(this);
    m_recordsTree->setColumnCount(2);
    m_recordsTree->setRootIsDecorated(false);
    QStringList headerLabels;
    headerLabels << "Selected" << "Subject" << "Date";
    m_recordsTree->setHeaderLabels(headerLabels);
    m_recordsTree->header()->setStretchLastSection(false);

    QPushButton *btnOk = new QPushButton("OK", this);
    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
    btnOk->setDefault(true);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(btnOk);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(m_recordsTree);
    mainLayout->addLayout(btnLayout);    

    setLayout(mainLayout);
    setWindowTitle("Export");
}

void ExportDialog::onUserVerify()
{
    lj::Communicator com;
    com.setUser(m_leUser->text(), m_lePassword->text());
    QMap<QString, QVariant> loginResult = com.login();
    if (loginResult.size() > 0) {
        QString s = QString("Verified: %1")
                .arg(loginResult.value("fullname").toString());
        setWindowTitle(s);
    }
}

void ExportDialog::onGetRecords()
{
    lj::Communicator com;
    com.setUser(m_leUser->text(), m_lePassword->text());
    QMap<QString, QVariant> result = com.getEvents(true);
    if (result.size() > 0) {
        QVariantList events = result["events"].toList();
        foreach (const QVariant &var, events) {
            QMap<QString, QVariant> event = var.toMap();

            QTreeWidgetItem *node = new QTreeWidgetItem;
            node->setFlags(Qt::ItemIsSelectable |
                           Qt::ItemIsUserCheckable |
                           Qt::ItemIsEnabled);
            node->setCheckState(0, Qt::Unchecked);

            node->setText(1, event["event"].toString());
            node->setText(2, event["eventtime"].toString());
            m_recordsTree->addTopLevelItem(node);
        }
    }
}

} // namespace gui
