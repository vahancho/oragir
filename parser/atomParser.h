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

#ifndef __ATOMPARSER_H__
#define __ATOMPARSER_H__

#include <QUrl>
#include <QHttp>
#include <QXmlStreamReader>
#include <QStack>

#include "blog.h"
#include "post.h"

namespace core
{

class AtomParser : public QObject
{
    Q_OBJECT

public:

    AtomParser();

    ~AtomParser();

    QString statusMessage() const;

signals:
    void fetched(const Post &post, const Blog &blog);
    void stateChanged(int state);
    void dataReadProgress(int done, int total);
    void stopped();

public slots:
    /// Start parsing the atom stream.
    void start();

    /// Stop all activity.
    void stop();

private slots:
    /// Fetches the fetched data from HTTP.
    void fetchHttpData(const QHttpResponseHeader &resp);

    /// This function called when fetching is done.
    void onHttpDone(bool error);

    void onStateChanged(int state);

    /// Parse the XML data.
    bool parseXmlData();

private:
    bool isGroupTag() const;
    QString m_status;
    QUrl m_url;

    int m_requestId;

    /// Current XML tag.
    QStack<QString> m_tags;
    QString m_currentTag;
    Blog m_currentBlog;
    Post m_currentPost;
    
    /// The XML reader.
    QXmlStreamReader m_xml;

    /// Http
    QHttp m_http;

    /// Restart parsing on error.
    bool m_restart;
};

} // namespace core

#endif // __ATOMPARSER_H__
