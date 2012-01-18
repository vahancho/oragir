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
#include <QAction>
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

void BlogEventView::setupActions(const HtmlActions &actions)
{
    HtmlActions::iterator it = actions.begin();
    while (it != actions.end()) {
        switch (it.key()) {
        case Undo:
        case Redo:
        case Cut:
        case Copy:
        case Paste:
        case SelectAll:
        case Bold:
        case Italic:
        case Underline:
        case Srikethrough:
        case Paragraph:
            connect(it.value(), SIGNAL(triggered()), m_htmlEditor, SLOT(setParagraph()));
            break;
        case Heading1:
            connect(it.value(), SIGNAL(triggered()), m_htmlEditor, SLOT(setHeading1()));
            break;
        case Heading2:
            connect(it.value(), SIGNAL(triggered()), m_htmlEditor, SLOT(setHeading2()));
            break;
        case Heading3:
            connect(it.value(), SIGNAL(triggered()), m_htmlEditor, SLOT(setHeading3()));
            break;
        case Heading4:
            connect(it.value(), SIGNAL(triggered()), m_htmlEditor, SLOT(setHeading4()));
            break;
        case Heading5:
            connect(it.value(), SIGNAL(triggered()), m_htmlEditor, SLOT(setHeading5()));
            break;
        case Heading6:
            connect(it.value(), SIGNAL(triggered()), m_htmlEditor, SLOT(setHeading6()));
            break;
        case Address:
            connect(it.value(), SIGNAL(triggered()), m_htmlEditor, SLOT(setAddress()));
            break;
        case FontName:
        case FontSize:
        case TextColor:
        case BgColor:
        case AlignLeft:
            connect(it.value(), SIGNAL(triggered()), m_htmlEditor, SLOT(setAlignLeft()));
            break;
        case AlignRight:
            connect(it.value(), SIGNAL(triggered()), m_htmlEditor, SLOT(setAlignRight()));
            break;
        case AlignCenter:
            connect(it.value(), SIGNAL(triggered()), m_htmlEditor, SLOT(setAlignCenter()));
            break;
        case AlignJustify:
            connect(it.value(), SIGNAL(triggered()), m_htmlEditor, SLOT(setAlignJustify()));
            break;
        case DecreaseIndent:
            connect(it.value(), SIGNAL(triggered()), m_htmlEditor, SLOT(setDecreaseIndent()));
            break;
        case IncreaseIndent:
            connect(it.value(), SIGNAL(triggered()), m_htmlEditor, SLOT(setIncreaseIndent()));
            break;
        case NumberedList:
            connect(it.value(), SIGNAL(triggered()), m_htmlEditor, SLOT(setNumberedList()));
            break;
        case BulletedList:
            connect(it.value(), SIGNAL(triggered()), m_htmlEditor, SLOT(setBulletedList()));
            break;
        default:
            ;
        }

        ++it;
    }
}

} // namespace gui
