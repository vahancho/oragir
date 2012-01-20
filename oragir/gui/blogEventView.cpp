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
#include <QLabel>
#include <QDateTimeEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include "blogEventView.h"
#include "htmlEditor.h"

namespace gui
{

BlogEventView::BlogEventView(QWidget *parent, Qt::WindowFlags f)
    :
        QWidget(parent, f)
{
    m_editSubject = new QLineEdit(this);
    QLabel *lblTime = new QLabel("Time:", this);
    m_dtEdit = new QDateTimeEdit(this);
    m_dtEdit->setDisplayFormat("MMMM d, yyyy, h:mm AP");
    m_dtEdit->setCalendarPopup(true);
    m_chkOutOrder = new QCheckBox("Date Out of Order", this);
    m_cmbPostTo = new QComboBox(this);
    m_cmbUserPic = new QComboBox(this);
    m_lblUserpic = new QLabel(this);
    m_lblUserpic->setMinimumWidth(64);

    QHBoxLayout *timeLayout = new QHBoxLayout;
    timeLayout->addWidget(m_dtEdit);
    timeLayout->addWidget(m_chkOutOrder);

    QFormLayout *formLayout1 = new QFormLayout;
    formLayout1->addRow("Subject", m_editSubject);
    formLayout1->addRow(lblTime, timeLayout);

    QFormLayout *formLayout2 = new QFormLayout;
    formLayout2->addRow("Post to:", m_cmbPostTo);
    formLayout2->addRow("Userpic:", m_cmbUserPic);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->addLayout(formLayout1);
    headerLayout->addLayout(formLayout2);
    headerLayout->addWidget(m_lblUserpic);

    m_htmlEditor = new HtmlEditor(this);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(headerLayout);
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

void BlogEventView::setDateTime(const QDateTime &dt)
{
    m_dtEdit->setDateTime(dt);
}

void BlogEventView::setDateOutOrder(bool outorder)
{
    m_chkOutOrder->setChecked(outorder);
}

void BlogEventView::setPostTo(const QStringList &postto)
{
    m_cmbPostTo->addItems(postto);
}

void BlogEventView::setUserPics(const QStringList &userpics)
{
    m_cmbUserPic->addItems(userpics);
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
            bindWebAction(action, QWebPage::InsertOrderedList);
            break;
        case BulletedList:
            bindWebAction(action, QWebPage::InsertUnorderedList);
            break;
        default:
            ;
        }

        ++it;
    }

    // Store the copy of actions map.
    m_htmlActions = actions;
}

void BlogEventView::bindWebAction(QAction *guiAction,
                                  QWebPage::WebAction webAction)
{
    connect(guiAction, SIGNAL(triggered()),
	    m_htmlEditor->viewAction(webAction),
	    SLOT(trigger()));
    connect(m_htmlEditor->viewAction(webAction),
            SIGNAL(changed()), SLOT(updateActionState()));
}

void BlogEventView::updateActionState()
{
    // Handle enabled state.
    m_htmlActions[Undo]->setEnabled(m_htmlEditor->viewAction(QWebPage::Undo)->isEnabled());
    m_htmlActions[Redo]->setEnabled(m_htmlEditor->viewAction(QWebPage::Redo)->isEnabled());
    m_htmlActions[Cut]->setEnabled(m_htmlEditor->viewAction(QWebPage::Cut)->isEnabled());
    m_htmlActions[Copy]->setEnabled(m_htmlEditor->viewAction(QWebPage::Copy)->isEnabled());
    m_htmlActions[Paste]->setEnabled(m_htmlEditor->viewAction(QWebPage::Paste)->isEnabled());
    m_htmlActions[Bold]->setEnabled(m_htmlEditor->viewAction(QWebPage::ToggleBold)->isEnabled());
    m_htmlActions[Italic]->setEnabled(m_htmlEditor->viewAction(QWebPage::ToggleItalic)->isEnabled());
    m_htmlActions[Underline]->setEnabled(m_htmlEditor->viewAction(QWebPage::ToggleUnderline)->isEnabled());

    // Handle check state
    m_htmlActions[Srikethrough]->setChecked(m_htmlEditor->viewAction(QWebPage::ToggleStrikethrough)->isChecked());
    m_htmlActions[NumberedList]->setChecked(m_htmlEditor->viewAction(QWebPage::InsertOrderedList)->isChecked());
    m_htmlActions[BulletedList]->setChecked(m_htmlEditor->viewAction(QWebPage::InsertUnorderedList)->isChecked());
}

} // namespace gui
