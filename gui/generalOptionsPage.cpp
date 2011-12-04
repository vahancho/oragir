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
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QGridLayout>
#include "core/defaultManager.h"
#include "core/application.h"
#include "strings/guiStrings.h"
#include "strings/strings.h"
#include "generalOptionsPage.h"

namespace gui
{

GeneralOptionsPage::GeneralOptionsPage(QWidget *parent, Qt::WFlags flags)
    :
        AbstractOptionsPage(parent, flags)
{
    QLabel *lblData = new QLabel("Data file:", this);
    QLineEdit *editData = new QLineEdit(this);
    QPushButton *btnDataBrowse = new QPushButton("...", this);

    QLabel *lblFilter = new QLabel("Filters file:", this);
    QLineEdit *editFilter = new QLineEdit(this);
    QPushButton *btnFilterBrowse = new QPushButton("...", this);

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(lblData, 0, 0);
    grid->addWidget(editData, 0, 1);
    grid->addWidget(btnDataBrowse, 0, 2);
    grid->addWidget(lblFilter, 1, 0);
    grid->addWidget(editFilter, 1, 1);
    grid->addWidget(btnFilterBrowse, 1, 2);

    QGroupBox *groupBox = new QGroupBox("Files", this);
    groupBox->setLayout(grid);

    m_chkQuitOnClose = new QCheckBox(str::QuitOnCloseTitle);
    core::DefaultManager *defaultMngr = core::Application::theApp()->defaultManager();
    bool checked = defaultMngr->value(str::QuitOnClose).toBool();
    if(checked)
        m_chkQuitOnClose->setCheckState(Qt::Checked);
    else
        m_chkQuitOnClose->setCheckState(Qt::Unchecked);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(groupBox);
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

    if(m_chkQuitOnClose->checkState() == Qt::Checked)
        defaultMngr->setValue(str::QuitOnClose, true);
    else
        defaultMngr->setValue(str::QuitOnClose, false);
}

} // namespace gui
