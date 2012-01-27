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

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <QtSql>

namespace core
{

class Database
{

public:
    Database();

    ~Database();

    /// Creates database and returns result of creation.
    /*!
        If there is no active connection, the new connection
        become the active one, otherwise new connection does
        not replace existing active connection.
    */
    virtual bool create(const QString &fileName);

    bool addTable(const QString &queryStr);

    void removeTable(const QString &table);

    /// Renames table oldName with the newName and returns renaming result.
    bool renameTable(const QString &oldName, const QString &newName);

    QStringList tables() const;

    /// Reports the last error if any.
    QString errorMessage() const;

    QSqlDatabase database() const;

    QString databaseName() const;

    /// Returns the total number of records in the given table.
    int totalCount(const QString &table) const;

protected:
     /// Stores the error message.
    QString m_error;

private:
    void remove();

    QString m_connection;
};

} // namespace core

#endif // __DATABASE_H__
