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
#include <QtSql>
#include "../strings/strings.h"
#include "database.h"

namespace core
{

Database::Database()
{}

bool Database::create(const QString &fileName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", fileName.toLatin1());
    db.setDatabaseName(fileName);
    if (!db.open()) {
        m_error = QString("Cannot open database. "
                          "Unable to establish a database connection.");
        return false;
    }

    QSqlQuery query;
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

    return true;
}

bool Database::remove(const QString &fileName)
{
    {
        QSqlDatabase db = QSqlDatabase::database(fileName);

        if(db.isValid()) {
            if (db.isOpen())
                db.close();
        } else {
            return false;
        }
    }

    QSqlDatabase::removeDatabase(fileName);
    return true;
}

void Database::onFetched(const Post &post, const Blog &blog)
{
    std::set<Rule<Post> >::iterator it = m_rules.begin();
    while (it != m_rules.end()) {
        const Rule<Post> &rule = *it;
        if (rule.match(post)) {
            addRecord(post, blog);
            break;
        }
        ++it;
    }
}

void Database::addRule(const Rule<Post> &rule)
{
    m_rules.insert(rule);
}

void Database::addRecord(const Post &post, const Blog &blog)
{
    QString sBlog = QString("%1\n%2\n%3\n%4\n%5\n%6\n")
                            .arg(blog.value(str::TagJournalId).toString())
                            .arg(blog.value(str::TagLink).toString())
                            .arg(blog.value(str::TagName).toString())
                            .arg(blog.value(str::TagJournal).toString())
                            .arg(blog.value(str::TagTitle).toString());

    QSqlQuery query;
    QString s = QString("INSERT INTO post VALUES(%1,'%2','%3','%4','%5','%6')")
                        .arg(post.value(str::TagPosterId).toString())
                        .arg(post.value(str::TagLink).toString())
                        .arg(post.value(str::TagUpdated).toString())
                        .arg(post.value(str::TagName).toString())
                        .arg(post.value(str::TagContent).toString())
                        .arg(post.value(str::TagTitle).toString());

    qDebug(post.value(str::TagLink).toString().toAscii().data());

    query.exec(s);

    query.exec("SELECT * FROM post");
    query.last();
    int numRows = query.at() + 1;
    if (numRows % 20 == 0)
        qDebug() << numRows;

    if (numRows == 20) {
        QSqlQueryModel *model1 = new QSqlQueryModel;
        model1->setQuery("SELECT posterid, name, title FROM post");
        model1->setHeaderData(0, Qt::Horizontal, tr("posterid"));
        model1->setHeaderData(1, Qt::Horizontal, tr("name"));
        model1->setHeaderData(1, Qt::Horizontal, tr("title"));

        QTableView *view = new QTableView;
        view->setModel(model1);
        view->show();

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
    }
}

bool Database::saveRules(const QString &fileName)
{
    QFile file(fileName);

    if(file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate)) {
        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);

        writer.writeStartDocument();
        writer.writeStartElement(str::TagRules);

        std::set<Rule<Post> >::iterator it = m_rules.begin();

        while (it != m_rules.end()) {
            Rule<Post> &rule = *it;
            rule.writeXml(writer);
            ++it;
        }

        writer.writeEndElement(); // rules
        writer.writeEndDocument();

        return true;
    }

    return false;
}

bool Database::openRules(const QString &fileName)
{
    QFile file(fileName);

    if(file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        QXmlStreamReader reader(&file);

        while (!reader.atEnd()) {
            reader.readNext();
            if(reader.isStartElement()) {
                if(reader.name() == str::TagRule) {
                    QString name = reader.attributes()
                                         .value(str::TagNameAttr).toString();
                    Rule<Post> rule(name);
                    rule.readXml(reader);
                    addRule(rule);
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

} // namespace core
