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

#include <set>
#include "../strings/strings.h"
#include "database.h"

namespace core
{

Database::Database()
{}

Database::~Database()
{
    remove();
}

bool Database::create(const QString &fileName)
{
    if (QSqlDatabase::contains(fileName)) {
        // Such connection already exists, so just do nothing.
        return true;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", fileName.toLatin1());
    db.setDatabaseName(fileName);
    if (!db.open()) {
        m_error = QString("Cannot open database. "
                          "Unable to establish a database connection.");
        return false;
    }

    QSqlQuery query(db);
    /* Temporary do not add blogs table.
    if (!query.exec(str::SqlCreateBlogTable)) {
        m_error = query.lastError().text();
        remove();
        return false;
    }
    */

    query.exec("PRAGMA page_size = 4096");
    query.exec("PRAGMA cache_size = 16384");
    query.exec("PRAGMA temp_store = MEMORY");
    query.exec("PRAGMA journal_mode = OFF");
    query.exec("PRAGMA locking_mode = EXCLUSIVE");
    query.exec("PRAGMA synchronous = OFF");

    m_connection = fileName;

    return true;
}

void Database::remove()
{
    {
        QSqlDatabase db = QSqlDatabase::database(m_connection);
        if (!db.isValid())
            return;

        if(db.isOpen())
            db.close();
    }

    QSqlDatabase::removeDatabase(m_connection);
    m_connection.clear();
}

void Database::onFetched(const Post &post, const Blog &blog)
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

void Database::addFilter(const Filter<Post> &filter)
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

void Database::addRecord(const Post &post, const Blog &blog,
                         const QString &table)
{
    QString sBlog = QString("%1\n%2\n%3\n%4\n%5\n%6\n")
                            .arg(blog.value(str::TagJournalId).toInt())
                            .arg(blog.value(str::TagLink).toString())
                            .arg(blog.value(str::TagName).toString())
                            .arg(blog.value(str::TagJournal).toString())
                            .arg(blog.value(str::TagTitle).toString());

    QSqlDatabase db = database();
    QSqlQuery query(db);
    QString queryStr = QString(str::SqlInsertPostToTable).arg(table);
    query.prepare(queryStr);

    query.bindValue(":posterid", post.value(str::TagPosterId).toInt());
    query.bindValue(":link", post.value(str::TagLink).toString());
    query.bindValue(":updated", post.value(str::TagUpdated).toString());
    query.bindValue(":name", post.value(str::TagName).toString());
    query.bindValue(":content", post.value(str::TagContent).toString());
    query.bindValue(":title", post.value(str::TagTitle).toString());
    bool inserted = query.exec();

    // If record inserted inform the world.
    if (inserted) {
        emit recordInserted(db, table);
    }
}

bool Database::saveFilters(const QString &fileName)
{
    QFile file(fileName);

    if(file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate)) {
        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);

        writer.writeStartDocument();
        writer.writeStartElement(str::TagFilters);

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

bool Database::openFilters(const QString &fileName)
{
    QFile file(fileName);

    if(file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        QXmlStreamReader reader(&file);

        while (!reader.atEnd()) {
            reader.readNext();
            if(reader.isStartElement()) {
                if(reader.name() == str::TagFilter) {
                    QXmlStreamAttributes attr = reader.attributes();
                    QString name = attr.value(str::TagNameAttr).toString();
                    bool enabled = QVariant::fromValue(attr.value(str::TagEnabled)
                                                           .toString()).toBool();
                    int ruleMatch = QVariant::fromValue(attr.value(str::TagRuleMatch)
                                                           .toString()).toInt();
                    Filter<Post> filter(name);
                    filter.readXml(reader);
                    filter.setEnabled(enabled);
                    filter.setRuleMatch((Filter<Post>::RuleMatch)ruleMatch);
                    addFilter(filter);
                }
            }
        }

        if (!reader.hasError()) {
            return true;
        } else {
            m_error = QString("XML ERROR: %1 : line %2")
                              .arg(reader.errorString())
                              .arg(reader.lineNumber());
            return false;
        }
    }

    return false;
}

QString Database::errorMessage() const
{
    return m_error;
}

const Database::Filters &Database::filters() const
{
    return m_filters;
}

void Database::clearFilters()
{
    m_filters.clear();
}

QString Database::databaseName() const
{
    return m_connection;
}

QSqlDatabase Database::database() const
{
    return QSqlDatabase::database(m_connection);
}

bool Database::addTable(const QString &table)
{
    QSqlDatabase db = database();
    QSqlQuery query(db);

    QString queryStr = QString(str::SqlCreatePostTable).arg(table);
    if (query.exec(queryStr)) {
        return true;
    } else {
        m_error = query.lastError().text();
        return false;
    }
}

void Database::removeTable(const QString &table)
{
    QSqlDatabase db = database();
    QSqlQuery query(db);

    QString queryStr = QString("DROP TABLE %1").arg(table);
    query.exec(queryStr);
}

QStringList Database::tables() const
{
    QSqlDatabase db = database();
    return db.tables(QSql::Tables);
}

} // namespace core
