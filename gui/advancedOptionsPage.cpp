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
#include "advancedOptionsPage.h"

namespace gui
{

AdvancedOptionsPage::AdvancedOptionsPage(QWidget *parent, Qt::WFlags flags)
    :
        AbstractOptionsPage(parent, flags)
{
    core::DefaultManager *defaultMngr = core::Application::theApp()->defaultManager();

    m_chkCheckUpdates = new QCheckBox(str::CheckUpdatesTitle);
    bool checked = defaultMngr->value(str::CheckUpdates).toBool();
    if(checked)
        m_chkCheckUpdates->setCheckState(Qt::Checked);
    else
        m_chkCheckUpdates->setCheckState(Qt::Unchecked);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_chkCheckUpdates);
    mainLayout->addStretch(1);

    setLayout(mainLayout);
}

// Destructor
AdvancedOptionsPage::~AdvancedOptionsPage()
{}

QString AdvancedOptionsPage::name() const
{
    return str::AdvancedPageTitle;
}

void AdvancedOptionsPage::saveDefaults()
{
    core::DefaultManager *defaultMngr = core::Application::theApp()->defaultManager();
    bool check = bool(m_chkCheckUpdates->checkState() == Qt::Checked);
    defaultMngr->setValue(str::CheckUpdates, check);
}

} // namespace gui
