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

QString Database::errorMessage() const
{
    return m_error;
}

QString Database::databaseName() const
{
    return m_connection;
}

QSqlDatabase Database::database() const
{
    return QSqlDatabase::database(m_connection);
}

bool Database::addTable(const QString &queryStr)
{
    QSqlDatabase db = database();
    QSqlQuery query(db);

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
    if (!query.exec(queryStr))
        m_error = query.lastError().text();
}

QStringList Database::tables() const
{
    QSqlDatabase db = database();
    QStringList tables = db.tables(QSql::Tables);
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
