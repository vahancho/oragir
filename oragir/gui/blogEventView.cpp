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
            bindWebAction(action, QWebPage::Undo);
            break;
        case Redo:
            bindWebAction(action, QWebPage::Redo);
            break;
        case Cut:
            bindWebAction(action, QWebPage::Cut);
            break;
        case Copy:
            bindWebAction(action, QWebPage::Copy);
            break;
        case Paste:
            bindWebAction(action, QWebPage::Paste);
            break;
        case SelectAll:
            bindWebAction(action, QWebPage::SelectAll);
            break;
        case Bold:
            bindWebAction(action, QWebPage::ToggleBold);
            break;
        case Italic:
            bindWebAction(action, QWebPage::ToggleItalic);
            break;
        case Underline:
            bindWebAction(action, QWebPage::ToggleUnderline);
            break;
        case Srikethrough:
            bindWebAction(action, QWebPage::ToggleStrikethrough);
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
            bindWebAction(action, QWebPage::AlignLeft);
            break;
        case AlignRight:
            bindWebAction(action, QWebPage::AlignRight);
            break;
        case AlignCenter:
            bindWebAction(action, QWebPage::AlignCenter);
            break;
        case AlignJustify:
            bindWebAction(action, QWebPage::AlignJustified);
            break;
        case DecreaseIndent:
            bindWebAction(action, QWebPage::Outdent);
            break;
        case IncreaseIndent:
            bindWebAction(action, QWebPage::Indent);
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

void BlogEventView::bindWebAction(QAction *guiAction,
                                  QWebPage::WebAction webAction)
{
    connect(guiAction, SIGNAL(triggered()),
	    m_htmlEditor->viewAction(webAction),
	    SLOT(trigger()));
}

} // namespace gui
