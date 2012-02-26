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

#ifndef __LJSECURITY_H__
#define __LJSECURITY_H__

#include <QString>
#include <QMap>
#include "export.h"

namespace lj
{

/// Handles security settings of LJ events.
/*!
    Security devided by two parts: major and minor. Major part
    consists of public, private and friends items. Only one of them
    can be selected at a time.
    Minor part can consist of up to 30 items (security levels) defined
    by user. Any combination of them can be selected.
*/
class LJCOMMUNICATOR_EXPORT Security
{
public:
    Security();

    enum Type {
        Public,
        Private,
        Friends,
        UseMask
    };

    /// Returns predefined major names.
    QStringList majorNames() const;

    /// Returns minor (custome) security levels names.
    QStringList minorNames() const;

    /// Returns the mask that corresponds to selected security levels.
    /*!
        The mask makes sense only if type is UseMask and Friends.
    */
    quint32 mask() const;

    /// Returns the type of the security selection.
    Type type() const;

    /// Returns the name of currently selected security level.
    QString selectedMajorName() const;

    /// Returns the LJ specific name of major security.
    QString selectedMajorLJName() const;

    /// Returns the name of currently selected minor levels.
    QStringList selectedMinorNames() const;

    /// Sets the friend groups: id-name comma separated strings.
    void setFriendGroups(const QStringList &friendGroups);

    /// Sets the current major security.
    void setMajorSecurity(const QString &security);

    /// Sets the currently selected minor security levels.
    void setMinorSecurity(const QStringList &security);

    /// Sets the current mask. Makes sence after setting friend groups.
    void setMask(quint32 mask);

private:
    Type m_type;
    quint32 m_mask;
    QMap<int, QString> m_friendGroups;
    QStringList m_selectedMinorNames;
};

} // namespace lj

#endif // __LJSECURITY_H__


