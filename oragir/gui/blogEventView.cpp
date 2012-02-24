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
#include <QPixmap>
#include <QPushButton>
#include <QActionGroup>
#include "blogEventView.h"
#include "htmlEditor.h"
#include "comboBox.h"
#include "staticMenu.h"

namespace gui
{

BlogEventView::BlogEventView(QWidget *parent, Qt::WindowFlags f)
    :
        QWidget(parent, f),
        m_eventId(-1)
{
    m_editSubject = new QLineEdit(this);
    m_editLocation = new QLineEdit(this);
    m_dtEdit = new QDateTimeEdit(this);
    m_dtEdit->setDisplayFormat("MMMM d, yyyy, h:mm AP");
    m_dtEdit->setCalendarPopup(true);
    m_chkOutOrder = new QCheckBox("Date Out of Order", this);
    m_cmbPostTo = new QComboBox(this);
    m_cmbUserPic = new QComboBox(this);
    connect(m_cmbUserPic, SIGNAL(activated(const QString &)),
            this, SLOT(onPicChanged(const QString &)));
    m_cmbTags = new ComboBox(this);
    m_cmbTags->setEditable(true);
    m_cmbMoods = new QComboBox(this);
    m_cmbMoods->setEditable(true);

    m_cmbSecurity = new QComboBox(this);
    QPushButton *btnSecurity = new QPushButton("Public", this);
    m_menuSecurity = new StaticMenu(this);
    connect(m_menuSecurity, SIGNAL(triggered(QAction *)),
            this, SLOT(onSecurityActionTrigerred(QAction *)));
    m_menuCustomSecurity = new StaticMenu("Custom", this);
    connect(m_menuCustomSecurity, SIGNAL(triggered(QAction *)),
            this, SLOT(onCustomSecurityActionTrigerred(QAction *)));
    btnSecurity->setMenu(m_menuSecurity);

    m_lblUserpic = new QLabel(this);
    m_lblUserpic->setMinimumSize(QSize(64, 64));
    m_lblUserpic->setAlignment(Qt::AlignTop);

    QHBoxLayout *timeLayout = new QHBoxLayout;
    timeLayout->addWidget(m_dtEdit);
    timeLayout->addWidget(m_chkOutOrder);

    QFormLayout *formLayout1 = new QFormLayout;
    formLayout1->addRow("Subject:", m_editSubject);
    formLayout1->addRow("Time:", timeLayout);
    formLayout1->addRow("Tags:", m_cmbTags);
    formLayout1->addRow("Location:", m_editLocation);

    QFormLayout *formLayout2 = new QFormLayout;
    formLayout2->addRow("Post to:", m_cmbPostTo);
    formLayout2->addRow("Userpic:", m_cmbUserPic);
    formLayout2->addRow("Access:", btnSecurity);
    formLayout2->addRow("Moods:", m_cmbMoods);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->addLayout(formLayout1);
    headerLayout->addLayout(formLayout2);
    headerLayout->addWidget(m_lblUserpic);

    m_htmlEditor = new HtmlEditor(this);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(m_htmlEditor);

    setLayout(mainLayout);

    m_timer.setInterval(10000);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    m_timer.start();
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

void BlogEventView::setUserPics(const QStringList &userpics,
                                const QString &picLocation)
{
    m_cmbUserPic->addItem("(default)");
    m_cmbUserPic->addItems(userpics);
    m_picLocation = picLocation;
}

void BlogEventView::setUserPic(const QString &userPic)
{
    for (int i = 0; i < m_cmbUserPic->count(); ++i) {
        QString s = m_cmbUserPic->itemData(i, Qt::DisplayRole).toString();
        if (s == userPic) {
            m_cmbUserPic->setCurrentIndex(i);
            break;
        }
    }
    onPicChanged(m_cmbUserPic->currentText());
}

QString BlogEventView::htmlContent() const
{
    return m_htmlEditor->content();
}

QString BlogEventView::subject() const
{
    return m_editSubject->text();
}

QDateTime BlogEventView::dateTime() const
{
    return m_dtEdit->dateTime();
}

bool BlogEventView::dateOutOrder() const
{
    return m_chkOutOrder->isChecked();
}

QString BlogEventView::postTo() const
{
    return m_cmbPostTo->currentText();
}

QString BlogEventView::userPic() const
{
    QString pic = m_cmbUserPic->currentText();
    if (pic == "(default)")
        return QString();
    else
        return pic;
}

void BlogEventView::setEventId(int eventId)
{
    m_eventId = eventId;
}

int BlogEventView::eventId() const
{
    return m_eventId;
}

void BlogEventView::setEventTags(const QString &tags)
{
    m_cmbTags->setEditText(tags);
}

void BlogEventView::setUserTags(const QStringList &tags)
{
    m_cmbTags->addItems(tags);
}

QString BlogEventView::eventTags() const
{
    return m_cmbTags->currentText();
}

void BlogEventView::setMoods(const QStringList &moods)
{
    // The empty mood item.
    m_cmbMoods->addItem("");

    // Verify that moods follow expected format.
    if (moods.size() % 3 == 0) {
        // Extract mood names only and ignor id and parent properties.
        for (int i = 1; i < moods.size(); i += 3) {
            m_cmbMoods->addItem(moods.at(i));
        }
    }
}

void BlogEventView::setMood(const QString &mood)
{
    for (int i = 0; i < m_cmbMoods->count(); ++i) {
        if (m_cmbMoods->itemText(i) == mood) {
            m_cmbMoods->setCurrentIndex(i);
            break;
        }
    }
}

QString BlogEventView::mood() const
{
    return m_cmbMoods->currentText();
}

void BlogEventView::setLocation(const QString &location)
{
    m_editLocation->setText(location);
}

QString BlogEventView::location() const
{
    return m_editLocation->text();
}

void BlogEventView::setSecurityNames(const QStringList &security)
{
    QActionGroup *ag = new QActionGroup(this);
    for (int i = 0; i < 3; ++i) {
        QAction *act = m_menuSecurity->addAction(security.at(i));
        act->setCheckable(true);
        ag->addAction(act);
    }
    m_menuSecurity->addSeparator();
    m_menuSecurity->addMenu(m_menuCustomSecurity);
    for (int i = 3; i < security.count(); ++i) {
        QAction *act = m_menuCustomSecurity->addAction(security.at(i));
        act->setCheckable(true);
    }
}

void BlogEventView::setSecurity(const QStringList &security)
{
    foreach (const QString &s, security) {
        m_menuSecurity->check(s);
        m_menuCustomSecurity->check(s);
    }
}

QStringList BlogEventView::security() const
{
    QStringList ret;
    QList<QAction *> actions = m_menuSecurity->actions();
    actions << m_menuCustomSecurity->actions();
    foreach (QAction *act, actions) {
        if (act->isCheckable() && act->isChecked())
            ret.append(act->text());
    }
    return ret;
}

void BlogEventView::setupActions(const HtmlActions &actions)
{
    HtmlActions::const_iterator it = actions.constBegin();
    while (it != actions.constEnd()) {
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
        case InsertImage:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(insertImage()));
            break;
        case CreateLink:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(createLink()));
            break;
        case LJCut:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(insertLjCut()));
            break;
        case InsertHtml:
            connect(action, SIGNAL(triggered()), m_htmlEditor, SLOT(insertHtml()));
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
    m_htmlActions[Bold]->setChecked(m_htmlEditor->viewAction(QWebPage::ToggleBold)->isChecked());
    m_htmlActions[Italic]->setChecked(m_htmlEditor->viewAction(QWebPage::ToggleItalic)->isChecked());
    m_htmlActions[Underline]->setChecked(m_htmlEditor->viewAction(QWebPage::ToggleUnderline)->isChecked());
}

void BlogEventView::onTimer()
{
    if (!m_chkOutOrder->isChecked())
        m_dtEdit->setDateTime(QDateTime::currentDateTime());
}

void BlogEventView::onPicChanged(const QString &pic)
{
    QPixmap pix(m_picLocation + '/' + pic + ".png");
    if (!pix.isNull()) {
        m_lblUserpic->setPixmap(pix.scaled(m_lblUserpic->size(),
                                           Qt::KeepAspectRatio));
    }

}

void BlogEventView::onSecurityActionTrigerred(QAction *action)
{
    m_menuCustomSecurity->uncheckAll();
}

void BlogEventView::onCustomSecurityActionTrigerred(QAction *action)
{
    m_menuSecurity->uncheckAll();
}

} // namespace gui
