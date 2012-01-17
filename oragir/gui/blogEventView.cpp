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
#include <QLayout>
#include "blogEventView.h"
#include "htmlEditor.h"

namespace gui
{

BlogEventView::BlogEventView(QWidget *parent, Qt::WindowFlags f)
    :
        QWidget(parent, f)
{
    m_editSubject = new QLineEdit(this);
    m_htmlEditor = new HtmlEditor(this);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_editSubject);
    mainLayout->addWidget(m_htmlEditor);

    setLayout(mainLayout);
}

void BlogEventView::setHtmlContent(const QString &content)
{
    m_htmlEditor->setContent(content);
}

void BlogEventView::setSubject(const QString &subject)
{
    m_editSubject->setText(subject);
}

void BlogEventView::setParagraph()
{
    m_htmlEditor->setParagraph();
}

void BlogEventView::setHeading1()
{
    m_htmlEditor->setHeading1();
}

void BlogEventView::setHeading2()
{
    m_htmlEditor->setHeading2();
}

void BlogEventView::setHeading3()
{
    m_htmlEditor->setHeading3();
}

void BlogEventView::setHeading4()
{
    m_htmlEditor->setHeading4();
}

void BlogEventView::setHeading5()
{
    m_htmlEditor->setHeading5();
}

void BlogEventView::setHeading6()
{
    m_htmlEditor->setHeading6();
}

void BlogEventView::setPreformatted()
{
    m_htmlEditor->setPreformatted();
}

void BlogEventView::setAddress()
{
    m_htmlEditor->setAddress();
}

void BlogEventView::setAlignLeft()
{
    m_htmlEditor->setAlignLeft();
}

void BlogEventView::setAlignCenter()
{
    m_htmlEditor->setAlignCenter();
}

void BlogEventView::setAlignRight()
{
    m_htmlEditor->setAlignRight();
}

void BlogEventView::setAlignJustify()
{
    m_htmlEditor->setAlignJustify();
}

void BlogEventView::setIncreaseIndent()
{
    m_htmlEditor->setIncreaseIndent();
}

void BlogEventView::setDecreaseIndent()
{
    m_htmlEditor->setDecreaseIndent();
}

void BlogEventView::setNumberedList()
{
    m_htmlEditor->setNumberedList();
}

void BlogEventView::setBulletedList()
{
    m_htmlEditor->setBulletedList();
}

void BlogEventView::insertImage()
{
    m_htmlEditor->insertImage();
}

void BlogEventView::createLink()
{
    m_htmlEditor->createLink();
}

} // namespace gui
