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
#include <QMessageBox>
#include "../strings/strings.h"
#include "database.h"

namespace core
{

Database::Database()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
     QMessageBox::critical(0, "Cannot open database",
                              "Unable to establish a database connection.\n"
                              "This example needs SQLite support. Please read "
                              "the Qt SQL driver documentation for information how "
                              "to build it.", QMessageBox::Cancel);
    }

    QSqlQuery query;
    query.exec("CREATE TABLE blog ("
               "journalid INT PRIMARY KEY NOT NULL,"
               "link      VARCHAR(256)    NOT NULL,"
               "name      NVARCHAR(128)   NOT NULL,"
               "journal   VARCHAR(256)    NOT NULL,"
               "title     NVARCHAR(256))");

    query.exec("CREATE TABLE post ("
               "posterid INT             NOT NULL,"
               "link     VARCHAR(256)    NOT NULL,"
               "updated  DATATIME        NOT NULL,"
               "name     NVARCHAR(128)   NOT NULL,"
               "content  NVARCHAR,"
               "title    NVARCHAR(256))");
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
                            .arg(blog.value(str::sTagJournalId).toString())
                            .arg(blog.value(str::sTagLink).toString())
                            .arg(blog.value(str::sTagName).toString())
                            .arg(blog.value(str::sTagJournal).toString())
                            .arg(blog.value(str::sTagTitle).toString());

    QSqlQuery query;
    QString s = QString("INSERT INTO post VALUES(%1,'%2','%3','%4','%5','%6')")
                        .arg(post.value(str::sTagPosterId).toString())
                        .arg(post.value(str::sTagLink).toString())
                        .arg(post.value(str::sTagUpdated).toString())
                        .arg(post.value(str::sTagName).toString())
                        .arg(post.value(str::sTagContent).toString())
                        .arg(post.value(str::sTagTitle).toString());

    qDebug(post.value(str::sTagLink).toString().toAscii().data());

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

} // namespace core
