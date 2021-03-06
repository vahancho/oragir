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

#include <QStringList>
#include "ljuserinfo.h"

namespace lj
{

UserInfo::UserInfo()
{}

UserInfo::UserInfo(const QByteArray &data)
{
    // Parse response data and extract events list from
    // response map in case of valid result.
    QVariant response = parse(data);
    if (isValid()) {
        m_info = response.toMap();
    }
}

QString UserInfo::fullName() const
{
    return m_info["fullname"].toString();
}

QString UserInfo::message() const
{
    return m_info["message"].toString();
}

QStringList UserInfo::moods() const
{
    QVariantList vl = m_info["moods"].toList();
    QStringList result;
    foreach (const QVariant &v, vl) {
        QMap<QString, QVariant> moodData = v.toMap();
        result.push_back(moodData["id"].toString());
        result.push_back(moodData["name"].toString());
        result.push_back(moodData["parent"].toString());
    }

    return result;
}

QString UserInfo::defaultPicUrl() const
{
    return m_info["defaultpicurl"].toString();
}

int UserInfo::id() const
{
    return m_info["userid"].toInt();
}

QStringList UserInfo::pictureKeys() const
{
    QVariantList vl = m_info["pickws"].toList();
    QStringList result;
    foreach (const QVariant &v, vl) {
        result.push_back(v.toString());
    }

    return result;
}

QStringList UserInfo::journals() const
{
    QVariantList vl = m_info["usejournals"].toList();
    QStringList result;
    foreach (const QVariant &v, vl) {
        result.push_back(v.toString());
    }

    return result;
}

QStringList UserInfo::pictureUrls() const
{
    QVariantList vl = m_info["pickwurls"].toList();
    QStringList result;
    foreach (const QVariant &v, vl) {
        result.push_back(v.toString());
    }

    return result;
}

}
