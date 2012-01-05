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

#include <QWebView>
#include <QWebFrame>
#include <QPlainTextEdit>
#include <QDesktopServices>
#include "htmlEditor.h"

namespace gui
{

HtmlEditor::HtmlEditor(QWidget *parent)
    :
        QTabWidget(parent)
{
    setTabShape(QTabWidget::Rounded);
    setTabPosition(QTabWidget::South);

    m_webView = new QWebView(this);
    m_webView->page()->setContentEditable(true);
    m_webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    m_webView->setHtml("<p></p>");
    connect(m_webView, SIGNAL(linkClicked(QUrl)), SLOT(onLinkClicked(QUrl)));
    addTab(m_webView, "Preview");

    m_htmlView = new QPlainTextEdit(this);
    addTab(m_htmlView, "Html");

    connect(this, SIGNAL(currentChanged(int)), SLOT(onTabChanged(int)));
}

void HtmlEditor::onTabChanged(int tab)
{
    if (tab == Html) {
        QString content = m_webView->page()->mainFrame()->toHtml();
        m_htmlView->setPlainText(content);
    } else if (tab == Preview) {
        QString html = m_htmlView->toPlainText();
        m_webView->setContent(html.toUtf8(), "text/html;charset=utf-8");
    }
}

void HtmlEditor::setContent(const QString &html)
{
    QByteArray data = html.toUtf8();
    m_webView->setContent(data, "text/html;charset=utf-8");
}

void HtmlEditor::onLinkClicked(const QUrl &url)
{
    QDesktopServices::openUrl(url);
}

QString HtmlEditor::content() const
{
    return m_webView->page()->mainFrame()->toHtml();
}

} // namespace gui
