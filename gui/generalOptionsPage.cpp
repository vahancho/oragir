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
#include "../core/defaultManager.h"
#include "../core/application.h"
#include "../strings/guiStrings.h"
#include "../strings/strings.h"
#include "generalOptionsPage.h"

namespace gui
{

GeneralOptionsPage::GeneralOptionsPage(QWidget *parent, Qt::WFlags flags)
    :
        AbstractOptionsPage(parent, flags)
{
    core::DefaultManager *defaultMngr = core::Application::theApp()->defaultManager();

    m_chkQuitOnClose = new QCheckBox(str::QuitOnCloseTitle);
    bool checked = defaultMngr->value(str::QuitOnClose).toBool();
    if(checked)
        m_chkQuitOnClose->setCheckState(Qt::Checked);
    else
        m_chkQuitOnClose->setCheckState(Qt::Unchecked);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_chkQuitOnClose);
    mainLayout->addStretch(1);

    setLayout(mainLayout);
}

// Destructor
GeneralOptionsPage::~GeneralOptionsPage()
{}

QString GeneralOptionsPage::name() const
{
    return str::GeneralPageTitle;
}

void GeneralOptionsPage::saveDefaults()
{
    core::DefaultManager *defaultMngr = core::Application::theApp()->defaultManager();
    bool quit = bool(m_chkQuitOnClose->checkState() == Qt::Checked);
    defaultMngr->setValue(str::QuitOnClose, quit);
}

} // namespace gui
