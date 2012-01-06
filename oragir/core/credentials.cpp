/**************************************************************************
*   Copyright (C) 2010 by Vahan Aghajanyan                                *
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

#include <QNetworkInterface>
#include <QStringList>
#include "credentials.h"

namespace core
{

const char separator = ' ';

QString Credentials::user() const
{
    return m_userName;
}

void Credentials::setUser(const QString &userName)
{
    m_userName = userName;
}

QString Credentials::password() const
{
    return m_password;
}

void Credentials::setPassword(const QString &password)
{
    m_password = password;
}

QString Credentials::encode() const
{
    if (m_userName.isEmpty() || m_password.isEmpty())
        return QString();

    QString login = macAddress()
                            + separator
                            + m_userName
                            + separator
                            + m_password;

    QByteArray ba = login.toAscii().toBase64();

    return QString(ba);
}

void Credentials::fromEncoded(const QString &encoded)
{
    if (!encoded.isEmpty()) {
        QString loginDecoded = QString(QByteArray::fromBase64(encoded.toAscii()));
        QStringList items = loginDecoded.split(separator);

        if (items.size() != 3)
            return;

        if (items.at(0) == macAddress()) {
            m_userName = items.at(1);
            m_password = items.at(2);
        }
    }
}

QString Credentials::macAddress() const
{
    // Discover network interfaces and addressEntries
    foreach(QNetworkInterface inter, QNetworkInterface::allInterfaces())
    {
        QString mac = inter.hardwareAddress();

        if (!mac.isEmpty())
            return mac;
    }

    return QString();
}

} // namespace core
