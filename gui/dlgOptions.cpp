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

#include <QPushButton>
#include <QLayout>
#include <QTreeWidget>
#include <QStackedWidget>
#include <QHeaderView>

#include "dlgOptions.h"

// Options pages
#include "abstractOptionsPage.h"

#include "../strings/guiStrings.h"
#include "../core/defaultManager.h"

namespace gui
{

OptionsDialog::OptionsDialog(QWidget *parent, Qt::WindowFlags f)
    :
        QDialog(parent, f)
{
    // Create navigation tree view and pages carrier widget.
    setupPages();

    // Navigation widgets and layout
    QHBoxLayout *navigationLayout = new QHBoxLayout;
    navigationLayout->addWidget(m_navigationTree);
    navigationLayout->addWidget(m_pages);

    // Buttons and button layout

    QPushButton *btnOk = new QPushButton(str::Ok);
    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *btnCancel = new QPushButton(str::Cancel);
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch(1);
    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);

    // Just a line
    QFrame *line = new QFrame;
    line->setLineWidth(1);
    line->setFrameStyle(QFrame::Sunken);
    line->setFrameShape(QFrame::HLine);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(navigationLayout);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(line);
    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);
    setWindowTitle(str::OptionsDialogTitle);
    setSizeGripEnabled(true);
    resize(483, 300);
}

OptionsDialog::~OptionsDialog()
{}

void OptionsDialog::setupPages()
{
    // Create and configure the navigation tree view.
    m_navigationTree = new QTreeWidget;
    // Set column count
    m_navigationTree->setColumnCount(1);
    // Make header invisible
    m_navigationTree->header()->setVisible(false);
    m_navigationTree->setMaximumWidth(75);
    m_navigationTree->setMinimumWidth(75);
    m_navigationTree->setRootIsDecorated(false);

    // Set connection
    connect(m_navigationTree,
            SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),
            this,
            SLOT(onChangePage(QTreeWidgetItem *, QTreeWidgetItem *)));

    // Create pages carrier
    m_pages = new QStackedWidget;

    // Add options pages

}

void OptionsDialog::addPage(AbstractOptionsPage *page)
{
    int pageIndex = m_pages->addWidget(page);

    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, page->name());
    m_navigationTree->addTopLevelItem(item);
    m_navigationTree->resizeColumnToContents(0);

    // Map the tree view item with the page index.
    m_pageMapping[item] = pageIndex;
}

void OptionsDialog::onChangePage(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);

    // Get the page index from the map.
    int pageIndex = m_pageMapping.value(current);

    // Activate the corresponding page.
    m_pages->setCurrentIndex(pageIndex);
}

void OptionsDialog::saveDefaults()
{
    for (int i = 0; i < m_pages->count(); ++i) {
        if( AbstractOptionsPage *page =
            qobject_cast<AbstractOptionsPage *>(m_pages->widget(i))) {
            page->saveDefaults();
        }
    }
}

} // namespace gui
