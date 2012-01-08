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
#include "../../ljcommunicator/ljevents.h"
#include "blogdatabase.h"

namespace core
{

BlogDatabase::BlogDatabase()
{}

void BlogDatabase::addEvent(const lj::Event &event)
{
    QSqlDatabase db = database();
    QSqlQuery query(db);

    QString queryStr = QString(str::SqlInsertMyEntry).arg(str::MyBlogTableName);
    query.prepare(queryStr);
    query.bindValue(":itemid", event.m_itemId);
    query.bindValue(":publicid", event.m_publicId);
    query.bindValue(":commentcount", event.m_commentCount);
    query.bindValue(":time", event.m_time);
    query.bindValue(":subject", event.m_time);
    query.bindValue(":event", event.m_event);
    query.bindValue(":url", event.m_url);
    query.bindValue(":tags", event.m_tags.join(","));
    query.bindValue(":security", event.m_security);
    query.bindValue(":flag", 0);

    query.exec();
}

} // namespace core
