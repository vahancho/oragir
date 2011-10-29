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
#include <QTableView>
#include "../strings/strings.h"
#include "database.h"

namespace core
{

Database::Database()
{}

bool Database::create(const QString &fileName)
{
    if (QSqlDatabase::contains(fileName)) {
        // Such connection already exists, so just do nothing.
        return true;
    } else {
        // We want to create new connection, so close existing one.
        remove();
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", fileName.toLatin1());
    db.setDatabaseName(fileName);
    if (!db.open()) {
        m_error = QString("Cannot open database. "
                          "Unable to establish a database connection.");
        return false;
    }

    QSqlQuery query(db);
    if (!query.exec(str::SqlCreateBlogTable) ||
        !query.exec(str::SqlCreatePostTable))
    {
        m_error = query.lastError().text();
        return false;
    }

    query.exec("PRAGMA page_size = 4096");
    query.exec("PRAGMA cache_size = 16384");
    query.exec("PRAGMA temp_store = MEMORY");
    query.exec("PRAGMA journal_mode = OFF");
    query.exec("PRAGMA locking_mode = EXCLUSIVE");
    query.exec("PRAGMA synchronous = OFF");

    m_dbConnectionName = fileName;

    /// Test
    m_model = new QSqlTableModel(0, db);
    m_model->setTable("post");
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_model->select();

    m_model->setHeaderData(0, Qt::Horizontal, tr("posterid"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("name"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("title"));
    m_model->removeColumns(2, 3);

    m_view = new QTableView;
    m_view->setModel(m_model);
    m_view->show();
    ///

    return true;
}

void Database::remove()
{
    {
        QSqlDatabase db = QSqlDatabase::database(m_dbConnectionName);
        if(db.isValid() && db.isOpen()) {
            db.close();
        }
    }

    QSqlDatabase::removeDatabase(m_dbConnectionName);
    m_dbConnectionName = QString();
}

void Database::onFetched(const Post &post, const Blog &blog)
{
    std::set<Filter<Post> >::iterator it = m_filters.begin();
    while (it != m_filters.end()) {
        const Filter<Post> &filter = *it;
        if (filter.enabled() && filter.match(post)) {
            addRecord(post, blog);
            break;
        }
        ++it;
    }
}

void Database::addFilter(const Filter<Post> &filter)
{
    m_filters.insert(filter);
}

void Database::addRecord(const Post &post, const Blog &blog)
{
    QString sBlog = QString("%1\n%2\n%3\n%4\n%5\n%6\n")
                            .arg(blog.value(str::TagJournalId).toInt())
                            .arg(blog.value(str::TagLink).toString())
                            .arg(blog.value(str::TagName).toString())
                            .arg(blog.value(str::TagJournal).toString())
                            .arg(blog.value(str::TagTitle).toString());

    QSqlDatabase db = QSqlDatabase::database(m_dbConnectionName);
    QSqlQuery query(db);

    query.prepare("INSERT INTO post (posterid, link, updated, name, content, title) "
                  "VALUES (:posterid, :link, :updated, :name, :content, :title)");

    query.bindValue(":posterid", post.value(str::TagPosterId).toInt());
    query.bindValue(":link", post.value(str::TagLink).toString());
    query.bindValue(":updated", post.value(str::TagUpdated).toString());
    query.bindValue(":name", post.value(str::TagName).toString());
    query.bindValue(":content", post.value(str::TagContent).toString());
    query.bindValue(":title", post.value(str::TagTitle).toString());
    bool inserted = query.exec();

    query.exec("SELECT * FROM post");
    query.last();
    int numRows = query.at() + 1;
    if (numRows % 20 == 0)
        qDebug() << numRows;

    m_model->select();
    m_model->insertRows(numRows, 1);
    m_view->update();

    /*
    QSqlQueryModel model;
    model.setQuery("SELECT * FROM post");
    int a = model.rowCount();

    for (int i = 0; i < model.rowCount(); ++i) {
        int id = model.record(i).value("posterid").toInt();
        QString name = model.record(i).value("name").toString();
        QString title = model.record(i).value("title").toString();
        qDebug() << id << name << title;
        QString s = QString("%1\t%2\t%3\t%4\t%5")
                    .arg(id)
                    .arg(model.record(i).value("updated").toString())
                    .arg(name)
                    .arg(title)
                    .arg(model.record(i).value("link").toString());
    }
    */
}

bool Database::saveFilters(const QString &fileName)
{
    QFile file(fileName);

    if(file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate)) {
        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);

        writer.writeStartDocument();
        writer.writeStartElement(str::TagFilters);

        std::set<Filter<Post> >::iterator it = m_filters.begin();

        while (it != m_filters.end()) {
            Filter<Post> &filter = *it;
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
                    QString name = reader.attributes()
                                         .value(str::TagNameAttr).toString();
                    Filter<Post> filter(name);
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

QSqlDatabase Database::database() const
{
     return QSqlDatabase::database(m_dbConnectionName);
}

} // namespace core
