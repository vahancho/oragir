/**************************************************************************
*   Copyright (C) 2012 by Vahan Aghajanyan                                *
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

#include "ljsyncitems.h"

namespace lj
{

SyncItems::SyncItems()
    :
        m_count(0),
        m_total(0)
{}

SyncItems::SyncItems(const QByteArray &data)
    :
        m_count(0),
        m_total(0)
{
    // Parse response data and extract events list from
    // response map in case of valid result.
    QVariant response = parse(data);
    if (isValid()) {
        QMap<QString, QVariant> responseMap = response.toMap();
        m_items = responseMap["syncitems"].toList();
        m_count = responseMap["count"].toInt();
        m_total = responseMap["total"].toInt();
    }
}

int SyncItems::count() const
{
    return m_count;
}

int SyncItems::total() const
{
    return m_total;
}

QString SyncItems::itemText(int index) const
{
    return item(index)["item"].toString();
}

QString SyncItems::time(int index) const
{
    return item(index)["time"].toString();
}

QString SyncItems::action(int index) const
{
    return item(index)["action"].toString();
}

SyncItems::Item SyncItems::item(int index) const
{
    if (index >= 0 && index < count())
        return m_items.at(index).toMap();
    else
        return Item();
}

}
