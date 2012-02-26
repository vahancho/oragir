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

#include <assert.h>
#include <vector>
#include <QDebug>
#include <QStringList>
#include "ljsecurity.h"

namespace lj
{

Security::Security()
    :
        m_type(Public)
{}

void Security::setMajorSecurity(const QString &security)
{
    if (!majorNames().contains(security)) {
        return;
    }

    if (security == "Public") {
        m_type = Public;
    } else if (security == "Private") {
        m_type = Private;
    } else if (security == "Friends") {
        m_mask = 1;
        m_type = UseMask;
    }

    m_selectedMinorNames.clear();
}

void Security::setMinorSecurity(const QStringList &security)
{
    m_type = UseMask;
    m_selectedMinorNames = security;

    // Calculate the mask.
    m_mask = 0;
    foreach (const QString &sec, security) {
        int id = m_friendGroups.key(sec);
        quint32 bitMask = 1;
        bitMask = bitMask << id;
        m_mask = m_mask | bitMask;
    }
    qDebug() << m_mask;
}

QString Security::selectedMajorName() const
{
    switch (m_type) {
    case Public:
        return "Public";
    case Private:
        return "Private";
    case Friends:
        return "Friends";
    default:
        return QString();
    }
}

QString Security::selectedMajorLJName() const
{
    switch (m_type) {
    case Public:
        return "public";
    case Private:
        return "private";
    case UseMask:
        return "usemask";
    default:
        return QString();
    }
}

QStringList Security::selectedMinorNames() const
{
    return m_selectedMinorNames;
}

QStringList Security::minorNames() const
{
    return m_friendGroups.values();
}

void Security::setFriendGroups(const QStringList &friendGroups)
{
    // Friends group should contain even number of items.
    assert(friendGroups.size() % 2 == 0);

    for (int i = 0; i < friendGroups.size() - 1; i += 2) {
        int id = friendGroups.at(i).toInt();
        QString name = friendGroups.at(i + 1);
        m_friendGroups[id] = name;
    }
}

QStringList Security::majorNames() const
{
    return QStringList() << "Public" << "Private" << "Friends";
}

quint32 Security::mask() const
{
    return m_mask;
}

Security::Type Security::type() const
{
    return m_type;
}

void Security::setMask(quint32 mask)
{
    m_mask = mask;
    if (m_mask == 1) {
        m_type = Friends;
        return;
    }

    m_type = UseMask;

    // Calculate selected group ids - the bit numbers for
    // friend groups, from 1-30.
    quint32 bitMask = 1;
    int groupId = 0;
    std::vector<int> ids;
    while (bitMask < m_mask) {
        if ((mask & bitMask) == bitMask) {
            ids.push_back(groupId);
        }
        bitMask = bitMask << 1;
        groupId++;
    }

    for (size_t i = 0; i < ids.size(); ++i) {
        m_selectedMinorNames << m_friendGroups.value(ids.at(i));
    }
}

}
