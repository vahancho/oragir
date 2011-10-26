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

#include "atomParser.h"
#include "../strings/strings.h"
#include "../strings/guiStrings.h"

namespace core
{

AtomParser::AtomParser()
    :
        m_url(str::FeedUrl)
{
    connect(&m_http, SIGNAL(readyRead(const QHttpResponseHeader &)), this,
            SLOT(fetchHttpData(const QHttpResponseHeader &)));

    connect(&m_http, SIGNAL(done(bool)),
            this, SLOT(onHttpDone(bool)));

    connect(&m_http, SIGNAL(stateChanged(int)),
            this, SLOT(onStateChanged(int)));

    m_http.setHost(m_url.host());
}

AtomParser::~AtomParser()
{
    if (m_http.state() != QHttp::Unconnected)
        m_http.abort();
}

void AtomParser::parse()
{
    if (m_http.state() != QHttp::Unconnected)
        m_http.abort();

    m_xml.clear();
    m_http.get(m_url.path());
}

void AtomParser::stop()
{
    m_http.abort();
}

void AtomParser::onStateChanged(int state)
{
    switch (state)
    {
    case QHttp::HostLookup :
        m_status = TRANSLATE(str::HostLookup);
        break;
    case QHttp::Connecting :
        m_status = TRANSLATE(str::Connecting);
        break;
    case QHttp::Sending :
        m_status = TRANSLATE(str::Sending);
        break;
    case QHttp::Reading :
        m_status = TRANSLATE(str::Reading);
        break;
    case QHttp::Connected :
        m_status = TRANSLATE(str::Connected);
        break;
    case QHttp::Unconnected :
    case QHttp::Closing :
    default:
        break;
    }
}

void AtomParser::fetchHttpData(const QHttpResponseHeader &resp)
{
    if (resp.statusCode() == 200)
    {
        m_xml.addData(m_http.readAll());

        // Abort HTTP if XML parsing failed.
        if(!parseXmlData())
            m_http.abort();
    }
    else
    {
        // Done with fail.
        m_status = m_http.errorString();
        m_http.abort();
    }
}

void AtomParser::onHttpDone(bool error)
{
    if (error)
        m_status = m_http.errorString();
    else
        m_status.clear();
}

bool AtomParser::parseXmlData()
{
    while (!m_xml.atEnd())
    {
        m_xml.readNext();

        if (m_xml.isStartElement()) {
            m_currentTag = m_xml.name().toString();

            if (isGroupTag()) {
                if (m_currentTag == str::TagFeed) {
                    m_currentBlog.resetToDefault();
                } else if (m_currentTag == str::TagEntry) {
                    m_currentPost.resetToDefault();
                }
                m_tags.push(m_currentTag);
            }

            if (m_currentTag == str::TagLink) {
                QXmlStreamAttributes atrs = m_xml.attributes();
                QString s = atrs.value("href").toString();

                if (m_tags.top() == str::TagFeed) {
                    m_currentBlog.setValue(str::TagLink, s);
                } else if (m_tags.top() == str::TagEntry) {
                    m_currentPost.setValue(str::TagLink, s);
                }
            } else if (m_currentTag == str::TagCategory) {
                QXmlStreamAttributes atrs = m_xml.attributes();
                QString s = atrs.value("term").toString();
                m_currentPost.addTag(s);
            }
        } else if (m_xml.isEndElement()) {
            if (isGroupTag()) {
                QString s = m_tags.pop();
                if (s == str::TagEntry)
                    emit fetched(m_currentPost, m_currentBlog);
            }
        } else if (m_xml.isCharacters() && !m_xml.isWhitespace()) {
            QString text = m_xml.text().toString();
            if (m_currentTag == str::TagTitle) {
                if (m_tags.top() == str::TagFeed) {
                    m_currentBlog.setValue(str::TagTitle, text);
                } else if (m_tags.top() == str::TagEntry) {
                    m_currentPost.setValue(str::TagTitle, text);
                }
            } else if (m_currentTag == str::TagName) {
                if (m_tags.top() == str::TagAuthor) {
                    m_currentBlog.setValue(str::TagName, text);
                } else if (m_tags.top() == str::TagEntry) {
                    m_currentPost.setValue(str::TagName, text);
                }
            } else if (m_currentTag == str::TagJournal) {
                m_currentBlog.setValue(str::TagJournal, text);
            } else if (m_currentTag == str::TagJournalId) {
                m_currentBlog.setValue(str::TagJournalId, text.toLong());
            } else if (m_currentTag == str::TagPosterId) {
                m_currentPost.setValue(str::TagPosterId, text.toLong());
            } else if (m_currentTag == str::TagUserPic) {
                m_currentPost.setValue(str::TagUserPic, text);
            } else if (m_currentTag == str::TagContent) {
                m_currentPost.setValue(str::TagContent, text);
            } else if (m_currentTag == str::TagUpdated) {
                m_currentPost.setValue(str::TagUpdated, text);
            }
        }
    }

    if (m_xml.error() && m_xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
        m_status = QString("XML ERROR: %1 : %2").arg(m_xml.lineNumber()).arg(m_xml.errorString());
        return false;
    } else {
        return true;
    }
}

bool AtomParser::isGroupTag() const
{
    return m_xml.name() == str::TagFeed ||
           m_xml.name() == str::TagAuthor ||
           m_xml.name() == str::TagEntry;
}

} // namespace core
