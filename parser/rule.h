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

#ifndef __RULE_H__
#define __RULE_H__

#include "post.h"

namespace core
{

template<class Source>
class Rule
{
public:
    Rule();

    enum Option {
        None = 0,
        ExactMatch,
        Contains
    };

    bool setFilter(const QString &name, const QString &value, Option opt);
    bool match(const Source &source) const;

private:
    struct Filter
    {
        Filter()
            :
                m_option(None)
        {}
        QString m_value;
        Option m_option;
    };

    bool match(const QString &name, const QString &value) const;

    QMap<QString, Filter> m_filters;
};

} // namespace core

#endif // __RULE_H__
