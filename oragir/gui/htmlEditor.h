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

#ifndef __HTMLEDITOR_H__
#define __HTMLEDITOR_H__

#include <QTabWidget>

class QWebView;
class QPlainTextEdit;
class QUrl;

namespace gui
{

/// Implementes simple html editor widget.
/*!
    Editor contains two tabs one for preview and second one
    is for direct html editing/viewing.
*/
class HtmlEditor : public QTabWidget
{
    Q_OBJECT

public:
    HtmlEditor(QWidget *parent = 0);

    /// Sets the html content to be edited/viewed.
    void setContent(const QString &html);

    /// Returns html content.
    QString content() const;

public slots:
    void setParagraph();
    void setHeading1();
    void setHeading2();
    void setHeading3();
    void setHeading4();
    void setHeading5();
    void setHeading6();
    void setPreformatted();
    void setAddress();
    void setAlignLeft();
    void setAlignCenter();
    void setAlignRight();
    void setAlignJustify();
    void setIncreaseIndent();
    void setDecreaseIndent();
    void setNumberedList();
    void setBulletedList();
    void insertImage();
    void createLink();

private slots:
    void onTabChanged(int tab);
    void onLinkClicked(const QUrl &url);

private:
    enum Tab
    {
        Preview = 0,
        Html
    };

    void invokeCommand(const QString&);
    void invokeCommand(const QString &cmd, const QString &arg);

    QWebView *m_webView;
    QPlainTextEdit *m_htmlView;
};

} // namespace gui

#endif // __HTMLEDITOR_H__
