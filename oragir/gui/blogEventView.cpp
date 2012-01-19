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
        // Handle the html actions' connection.
        QAction *action = it.value();
        action->disconnect();

        switch (it.key()) {
        case Undo:
            break;
        case Redo:
            break;
        case Cut:
            break;
        case Copy:
            break;
        case Paste:
            break;
        case SelectAll:
            break;
        case Bold:
            break;
        case Italic:
            break;
        case Underline:
            break;
        case Srikethrough:
            break;
        case Paragraph:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setParagraph()));
            break;
        case Heading1:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setHeading1()));
            break;
        case Heading2:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setHeading2()));
            break;
        case Heading3:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setHeading3()));
            break;
        case Heading4:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setHeading4()));
            break;
        case Heading5:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setHeading5()));
            break;
        case Heading6:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setHeading6()));
            break;
        case Address:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setAddress()));
            break;
        case FontName:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setFontName()));
            break;
        case FontSize:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setFontSize()));
            break;
        case TextColor:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setTextColor()));
            break;
        case BgColor:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setBackgroundColor()));
            break;
        case AlignLeft:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setAlignLeft()));
            break;
        case AlignRight:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setAlignRight()));
            break;
        case AlignCenter:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setAlignCenter()));
            break;
        case AlignJustify:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setAlignJustify()));
            break;
        case DecreaseIndent:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setDecreaseIndent()));
            break;
        case IncreaseIndent:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setIncreaseIndent()));
            break;
        case NumberedList:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setNumberedList()));
            break;
        case BulletedList:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(setBulletedList()));
            break;
        default:
            ;
        }

        ++it;
    }
}

} // namespace gui
