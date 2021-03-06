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

#include "../strings/strings.h"
#include "streamdatabase.h"

namespace core
{

StreamDatabase::StreamDatabase()
{}

void StreamDatabase::onFetched(const Post &post, const Blog &blog)
{
    Filters::iterator it = m_filters.begin();
    while (it != m_filters.end()) {
        const Filter<Post> &filter = *it;
        if (filter.enabled() && filter.match(post)) {
            addRecord(post, blog, filter.table());
            break;
        }
        ++it;
    }
}

void StreamDatabase::addFilter(const Filter<Post> &filter)
{
    // If this filter already exists, delete it from container
    // and replace with a new one. This will guarantee that
    // filters will be updated, otherwise the new filter
    // will not add, and the old filter would remain.
    Filters::iterator it = m_filters.find(filter);
    if (it != m_filters.end())
        m_filters.erase(it);

    m_filters.insert(filter);
}

void StreamDatabase::addRecord(const Post &post, const Blog &/*blog*/,
                         const QString &table)
{
    QSqlQuery q = query();

    /* There is no usage for this table, so do nothing yet.
    // Insert blog record into blogs table.
    QString queryStr = QString(str::SqlInsertBlogToTable).arg(str::BlogTableName);
    q.prepare(queryStr);
    q.bindValue(":journalid", blog.value(str::TagJournalId).toInt());
    q.bindValue(":link", blog.value(str::TagLink).toString());
    q.bindValue(":name", blog.value(str::TagName).toString());
    q.bindValue(":journal", blog.value(str::TagJournal).toString());
    q.bindValue(":title", blog.value(str::TagTitle).toString());
    q.bindValue(":flag", 0);
    bool inserted = query.exec();
    if (!inserted)
        m_error = q.lastError().text();
    */

    // Insert post record into the target table.
    QString queryStr = QString(str::SqlInsertPostToTable).arg(table);
    q.prepare(queryStr);
    q.bindValue(":flag", 0);
    q.bindValue(":title", post.value(str::TagTitle).toString());
    q.bindValue(":read", false);
    q.bindValue(":name", post.value(str::TagName).toString());
    q.bindValue(":updated", post.value(str::TagUpdated).toString());
    q.bindValue(":posterid", post.value(str::TagPosterId).toInt());
    q.bindValue(":link", post.value(str::TagLink).toString());
    q.bindValue(":content", post.value(str::TagContent).toString());
    q.bindValue(":userpic", post.value(str::TagUserPic).toString());
    q.bindValue(":category", post.value(str::TagCategory).toStringList().join(",")); 

    bool inserted = q.exec();

    // If record inserted inform the world.
    if (inserted) {
        emit recordInserted(table);
    } else {
        m_error = q.lastError().text();
    }
}

bool StreamDatabase::saveFilters(const QString &fileName)
{
    QFile file(fileName);

    if(file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate)) {
        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);

        writer.writeStartDocument();
        writer.writeStartElement(str::TagFilters);
        writer.writeAttribute(str::TagVersion, str::FilterVersion);

        Filters::iterator it = m_filters.begin();

        while (it != m_filters.end()) {
            Filter<Post> filter = *it;
            filter.writeXml(writer);
            ++it;
        }

        writer.writeEndElement(); // filters
        writer.writeEndDocument();

        return true;
    }

    return false;
}

bool StreamDatabase::openFilters(const QString &fileName)
{
    QFile file(fileName);

    if(file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        QXmlStreamReader reader(&file);
        // Clear all existing filters before loading new ones.
        clearFilters();

        while (!reader.atEnd()) {
            reader.readNext();
            if(reader.isStartElement()) {
                if (reader.name() == str::TagFilters) {
                    QXmlStreamAttributes attr = reader.attributes();
                    QString version =
                        QVariant::fromValue(attr.value(str::TagVersion).toString())
                                           .toString();
                } else if(reader.name() == str::TagFilter) {
                    Filter<Post> filter;
                    filter.readXml(reader);
                    addFilter(filter);
                }
            }
        }

        if (!reader.hasError()) {
            return true;
        } else {
            m_error = QString("Failed to load filter file. Error: %1 Line %2, column %3")
                              .arg(reader.errorString())
                              .arg(reader.lineNumber())
                              .arg(reader.columnNumber());
            return false;
        }
    }

    return false;
}

const StreamDatabase::Filters &StreamDatabase::filters() const
{
    return m_filters;
}

void StreamDatabase::clearFilters()
{
    m_filters.clear();
}

bool StreamDatabase::addStreamTable(const QString &table)
{
    QString queryStr = QString(str::SqlCreatePostTable).arg(table);
    bool added = addTable(queryStr);
    if (added)
        emit tableAdded(table);

    return added;
}

int StreamDatabase::unreadCount(const QString &table) const
{
    QSqlQuery q = query();
    QString str = QString("SELECT COUNT(*) FROM %1 WHERE read='false'").arg(table);
    if (q.exec(str) && q.next())
        return q.value(0).toInt();
    else
        return -1;
}

} // namespace core
