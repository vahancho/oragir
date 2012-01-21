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

#ifndef __LJUSERINFO_H__
#define __LJUSERINFO_H__

#include "export.h"
#include "response.h"

namespace lj
{

/// Implements the wrapper class around response for the login request.
class LJCOMMUNICATOR_EXPORT UserInfo : public xmlrpc::Response
{
public:
    UserInfo();
    UserInfo(const QByteArray &data);

    /// Returns user's full name.
    QString fullName() const;

    /// Returns LJ message to the user.
    QString message() const;

    /// List of shared/news/community journals that the user has permission to post in.
    QStringList journals() const;

    QStringList pictureUrls() const;

    QStringList pictureKeys() const;

    QStringList moods() const;

    QString defaultPicUrl() const;

    int id() const;

private:
    QMap<QString, QVariant> m_info;
};

} // namespace lj

#endif // __LJUSERINFO_H__


