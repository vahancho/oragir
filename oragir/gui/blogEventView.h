/**************************************************************************
*   Copyright (C) 2012 by Vahan Aghajanyan                                *
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

#ifndef __BLOGEVENTVIEW_H__
#define __BLOGEVENTVIEW_H__

#include <QWidget>
#include <QWebPage>
#include <QDateTime>
#include <QMap>
#include <QTimer>

class QLineEdit;
class QCheckBox;
class QLabel;
class QComboBox;
class QDateTimeEdit;

namespace gui
{

class HtmlEditor;

class BlogEventView : public QWidget
{
    Q_OBJECT

public:
    BlogEventView(QWidget *parent = 0, Qt::WindowFlags f = 0);

    /// Sets the html view content.
    void setHtmlContent(const QString &content);
    QString htmlContent() const;

    /// Sets the subject text.
    void setSubject(const QString &subject);
    QString subject() const;

    void setDateTime(const QDateTime &dt);
    QDateTime dateTime() const;

    void setDateOutOrder(bool outorder);
    bool dateOutOrder() const;

    void setPostTo(const QStringList &postto);
    QString postTo() const;

    void setUserPics(const QStringList &userpics);
    void setUserPic(const QString &userPic);
    QString userPic() const;

    void setEventId(int eventId);
    int eventId() const;

    /// Sets the tag list.
    void setEventTags(const QString &tags);
    void setUserTags(const QStringList &tags);
    QString eventTags() const;

    /// Sets the list of user moods.
    /*!
        Moods expected as a string list that contains mood data:
        id, name, parent.
    */
    void setMoods(const QStringList &moods);
    QString moods() const;

    /// Defines the index of html editor related actions.
    enum HtmlAction
    {
        Undo = 0,
        Redo,
        Cut,
        Copy,
        Paste,
        SelectAll,
        Bold,
        Italic,
        Underline,
        Srikethrough,
        Paragraph,
        Heading1,
        Heading2,
        Heading3,
        Heading4,
        Heading5,
        Heading6,
        Address,
        FontName,
        FontSize,
        TextColor,
        BgColor,
        AlignLeft,
        AlignRight,
        AlignCenter,
        AlignJustify,
        DecreaseIndent,
        IncreaseIndent,
        NumberedList,
        BulletedList
    };

    /// Defines the new type of mapping between index and action.
    typedef QMap<BlogEventView::HtmlAction, QAction *> HtmlActions;

    /// Set up the actions connections to appropriate slots.
    void setupActions(const HtmlActions &actions);

private slots:
    /// Updates the gui actions state based on web page actions state.
    void updateActionState();

    void onTimer();

private:
    void bindWebAction(QAction *guiAction, QWebPage::WebAction webAction);

    QLineEdit *m_editSubject;
    HtmlEditor *m_htmlEditor;
    HtmlActions m_htmlActions;
    QCheckBox *m_chkOutOrder;
    QComboBox *m_cmbPostTo;
    QComboBox *m_cmbUserPic;
    QComboBox *m_cmbTags;
    QComboBox *m_cmbMoods;
    QLabel *m_lblUserpic;
    QDateTimeEdit *m_dtEdit;

    QTimer m_timer;

    /// Stores the event id. For new event id is -1.
    int m_eventId;
};

} // namespace gui

#endif // __BLOGEVENTVIEW_H__
