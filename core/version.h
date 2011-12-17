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

#ifndef __VERSION_H__
#define __VERSION_H__

#include <vector>

namespace core
{

/// Implements the version class.
class Version
{

public:
    /// Construct default version.
    Version();

    /// Construct version from string.
    Version(const std::string &verStr);

    ~Version();

    /// Sets the version for the version string.
    /*!
        Version string should follow the format: "major.minor.build_num (mode)"
    */
    void fromString(const std::string &verStr);

    /// Returns the current version as string.
    std::string toString() const;

    /// Resets the version.
    void reset();

    /// Compares two versions.
    bool operator>(const Version &ver) const;

private:
    /// Version numbers.
    std::vector<unsigned int> m_versions;

    /// Version mode, such as "(alpha)" or "(beta)".
    std::string m_mode;
};

} // namespace core

#endif // __LOGIN_H__