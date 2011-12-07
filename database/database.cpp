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
    // Temporary do not add blogs table.
    QString queryStr = QString(str::SqlCreateBlogTable)
                              .arg(str::BlogTableName);
    if (!query.exec(queryStr)) {
        m_error = query.lastError().text();
        remove();
        return false;
    }

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
    QSqlDatabase db = database();
    QSqlQuery query(db);

    // Insert blog record into blogs table.
    QString queryStr = QString(str::SqlInsertBlogToTable).arg(str::BlogTableName);
    query.prepare(queryStr);
    query.bindValue(":journalid", blog.value(str::TagJournalId).toInt());
    query.bindValue(":link", blog.value(str::TagLink).toString());
    query.bindValue(":name", blog.value(str::TagName).toString());
    query.bindValue(":journal", blog.value(str::TagJournal).toString());
    query.bindValue(":title", blog.value(str::TagTitle).toString());
    query.bindValue(":flag", 0);
    bool inserted = query.exec();
    if (!inserted)
        m_error = query.lastError().text();

    // Insert post record into the target table.
    queryStr = QString(str::SqlInsertPostToTable).arg(table);
    query.prepare(queryStr);
    query.bindValue(":flag", 0);
    query.bindValue(":title", post.value(str::TagTitle).toString());
    query.bindValue(":read", false);
    query.bindValue(":name", post.value(str::TagName).toString());
    query.bindValue(":updated", post.value(str::TagUpdated).toString());
    query.bindValue(":posterid", post.value(str::TagPosterId).toInt());
    query.bindValue(":link", post.value(str::TagLink).toString());
    query.bindValue(":content", post.value(str::TagContent).toString());
    query.bindValue(":userpic", post.value(str::TagUserPic).toString());
    query.bindValue(":category", post.value(str::TagCategory).toStringList().join(",")); 

    inserted = query.exec();

    // If record inserted inform the world.
    if (inserted) {
        emit recordInserted(table);
    } else {
        m_error = query.lastError().text();
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

bool Database::openFilters(const QString &fileName)
{
    QFile file(fileName);

    if(file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        QXmlStreamReader reader(&file);

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
    QStringList tables = db.tables(QSql::Tables);
    tables.removeAll(str::BlogTableName);
    return tables;
}

bool Database::renameTable(const QString &oldName, const QString &newName)
{
    QSqlDatabase db = database();
    QSqlQuery query(db);
    QString queryStr = QString("ALTER TABLE %1 RENAME TO %2")
                               .arg(oldName).arg(newName);
    if (query.exec(queryStr)) {
        return true;
    } else {
        m_error = query.lastError().text();
        return false;
    }
}

int Database::unreadCount(const QString &table) const
{
    QSqlDatabase db = database();
    QSqlQuery query(db);
    QString str = QString("SELECT COUNT(*) FROM %1 WHERE read='false'").arg(table);
    query.exec(str);
    query.next();
    return query.value(0).toInt();
}

int Database::totalCount(const QString &table) const
{
    QSqlDatabase db = database();
    QSqlQuery query(db);
    QString str = QString("SELECT COUNT(*) FROM %1").arg(table);
    query.exec(str);
    query.next();
    return query.value(0).toInt();
}

} // namespace core
