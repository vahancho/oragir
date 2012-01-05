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

#include <QCheckBox>
#include <QLayout>
#include <QSpinBox>
#include "../core/defaultManager.h"
#include "../core/application.h"
#include "../strings/guiStrings.h"
#include "../strings/strings.h"
#include "connectOptionsPage.h"

namespace gui
{

ConnectOptionsPage::ConnectOptionsPage(QWidget *parent, Qt::WFlags flags)
    :
        AbstractOptionsPage(parent, flags)
{
    core::DefaultManager *defaultMngr = core::Application::theApp()->defaultManager();

    m_chkReconnect = new QCheckBox(str::ReconnectTitle, this);
    bool checked = defaultMngr->value(str::Reconnect).toBool();
    if(checked)
        m_chkReconnect->setCheckState(Qt::Checked);
    else
        m_chkReconnect->setCheckState(Qt::Unchecked);

    m_reconnectCount = new QSpinBox(this);
    m_reconnectCount->setMinimum(1);
    m_reconnectCount->setValue(defaultMngr->value(str::ReconnectCount).toInt());

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(m_chkReconnect);
    hLayout->addWidget(m_reconnectCount);
    hLayout->addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(hLayout);
    mainLayout->addStretch(1);

    setLayout(mainLayout);
}

// Destructor
ConnectOptionsPage::~ConnectOptionsPage()
{}

QString ConnectOptionsPage::name() const
{
    return str::ConnectionPageTitle;
}

void ConnectOptionsPage::saveDefaults()
{
    core::DefaultManager *defaultMngr = core::Application::theApp()->defaultManager();

    bool reconnect = bool(m_chkReconnect->checkState() == Qt::Checked);
    defaultMngr->setValue(str::Reconnect, reconnect);
    defaultMngr->setValue(str::ReconnectCount, m_reconnectCount->value());
}

} // namespace gui
