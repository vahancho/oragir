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

#include <QTextEdit>
#include <QLayout>
#include <QFormLayout>
#include <QLabel>
#include "previewWindow.h"

namespace gui
{

PreviewWindow::PreviewWindow(QWidget *parent, Qt::WindowFlags f)
    :
        QWidget(parent, f)
{
    m_author = new QLabel(this);
    m_url = new QLabel(this);
    m_title = new QLabel(this);
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("&Author:"), m_author);   
    formLayout->addRow(tr("&Link URL:"), m_url);
    formLayout->addRow("&Title:", m_title);

    m_preview = new QTextEdit(this);
    m_preview->setReadOnly(true);
    
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(m_preview);

    setLayout(mainLayout);
}

PreviewWindow::~PreviewWindow()
{}

void PreviewWindow::setText(const QString &text)
{
    m_preview->setText(text);
}

void PreviewWindow::setAuthor(const QString &author)
{
    m_author->setText(author);
}

void PreviewWindow::setUrl(const QString &url)
{
    m_url->setText(url);
}

void PreviewWindow::setTitle(const QString &title)
{
    m_title->setText(title);
}

} // namespace gui
