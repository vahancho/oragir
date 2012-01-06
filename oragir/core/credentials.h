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

#ifndef __CREDENTIALS_H__
#define __CREDENTIALS_H__

namespace core
{

/// Implements the login information storage class.
/*!
    This class stores the user login data and can encode/decode it to be stored
    in the files.
*/
class Credentials
{

public:
    /// Sets the user name.
    void setUser(const QString &userName);

    /// Returns user name.
    QString user() const;

    /// Sets user password.
    void setPassword(const QString &password);

    /// Returns user password.
    QString password() const;

    /// Returns decoded user data.
    QString encode() const;

    /// Decodes the given string and stores decoded login data.
    void fromEncoded(const QString &encoded);

private:
    /// Returns the MAC address of this computer hardware.
    QString macAddress() const;

    /// Store the user name.
    QString m_userName;

    /// Store the user password.
    QString m_password;
};

} // namespace core

#endif // __CREDENTIALS_H__
