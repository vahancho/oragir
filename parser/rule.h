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

#include <QXmlStreamWriter>
#include "post.h"
#include "../strings/strings.h"

namespace core
{

template<class Source>
class Rule
{
public:
    Rule(const QString &name);

    enum Option {
        Ignore = 0,
        ExactMatch,
        Contains
    };

    bool setFilter(const QString &name, const QString &value, Option opt);
    bool match(const Source &source) const;
    QString name() const;
    void writeXml(QXmlStreamWriter &writer);
    void readXml(QXmlStreamReader &reader);

    bool operator<(const Rule<Source> &other) const;

private:
    struct Filter
    {
        Filter()
            :
                m_option(Ignore)
        {}
        QString m_value;
        Option m_option;
    };

    bool match(const QString &name, const QString &value) const;

    QMap<QString, Filter> m_filters;
    QString m_name;
};

///////////////////////////////////////////////////////////////////////////////
// Implementation
//

template<class Source>
Rule<Source>::Rule(const QString &name)
    :
        m_name(name)
{
    Source source;
    QMap<QString, QVariant> properties = source.propertyMap();

    QMap<QString, QVariant>::const_iterator it = properties.constBegin();
    while (it != properties.constEnd()) {
        Filter flt;
        const QString &name = it.key();
        m_filters[name] = flt;

        ++it;
    }
}

template<class Source>
bool Rule<Source>::setFilter(const QString &name, const QString &value,
                             Option opt)
{
    QMap<QString, Filter>::iterator it = m_filters.find(name);
    if (it != m_filters.end()) {
        Filter &flt = it.value();
        flt.m_value = value;
        flt.m_option = opt;

        return true;
    } else {
        return false;
    }
}

template<class Source>
bool Rule<Source>::match(const Source &source) const
{
    QMap<QString, QVariant> properties = source.propertyMap();

    // Iterate over all properties and apply filter on each.
    // If at least one filter does not passed object does not pass.
    QMap<QString, QVariant>::const_iterator it = properties.constBegin();
    while (it != properties.constEnd()) {
        if (!match(it.key(), it.value().toString())) {
            return false;
        }
        ++it;
    }

    return true;
}

template<class Source>
bool Rule<Source>::match(const QString &name, const QString &value) const
{
    QMap<QString, Filter>::iterator it = m_filters.find(name);
    if (it != m_filters.end()) {
        const Filter &flt = it.value();

        // Strip the html tags before comparing strings.
        QString stripped = value;
        stripped.replace("&amp;", "&");
        stripped.replace("&quot;", "\"");
        stripped.replace("&lt;", "<");
        stripped.replace("&gt;", ">");
        stripped.replace("&apos;", "'");
        stripped.replace("&nbsp;", " ");
        stripped.replace( QRegExp("<[^>]*>"), "" );

        if (flt.m_option == Ignore) {
            return true;
        } else if (flt.m_option == Contains &&
                   stripped.contains(flt.m_value, Qt::CaseInsensitive)) {
            QRegExp rx;
            QString pat = QString("\\s.{0,40}%1.{0,40}\\s").arg(flt.m_value);
            rx.setPattern(pat);
            if (rx.indexIn(stripped) != -1) {
                QString s = rx.cap(0);
                qDebug(s.toAscii().data());
            }
            return true;
        } else if (flt.m_option == ExactMatch &&
                   stripped == flt.m_value) {
            return true;
        } else {
            return false;
        }
    } else {
        // Such property not found.
        return false;
    }
}

template<class Source>
QString Rule<Source>::name() const
{
    return m_name;
}

template<class Source>
void Rule<Source>::writeXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("rule");
    writer.writeAttribute("name", m_name);

    QMap<QString, Filter>::const_iterator it = m_filters.constBegin();
    while (it != m_filters.constEnd()) {
        const Filter &flt = it.value();
        const QString &name = it.key();

        writer.writeStartElement("filter");

        writer.writeAttribute("name", name);
        writer.writeTextElement("value", flt.m_value);
        writer.writeTextElement("option", QString::number(flt.m_option));

        writer.writeEndElement(); // filter

        ++it;
    }

    writer.writeEndElement(); // rule
}

template<class Source>
void Rule<Source>::readXml(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.isStartElement());

    reader.readNext();
    QString filterName;
    QString filterValue;
    Option option;
    QString elementName;

    while(!(reader.isEndElement() && reader.name() == str::sTagRule)) {
        reader.readNext();
        if(reader.isStartElement()) {
            elementName = reader.name().toString();
            if (elementName == str::sTagFilter)
                filterName = reader.attributes().value(str::sTagNameAttr).toString();
        } else if (reader.isEndElement()) {
            if (reader.name().toString() == str::sTagFilter) {
                if (option != Ignore)
                    setFilter(filterName, filterValue, option);
            }
        } else if (reader.isCharacters() && !reader.isWhitespace()) {
            if (elementName == str::sTagValue)
                filterValue = reader.text().toString();
            else if (elementName == str::sTagOption)
                option = (Option)reader.text().toString().toInt();
        }
    }
}

template<class Source>
bool Rule<Source>::operator<(const Rule<Source> &other) const
{
    return m_name < other.m_name;
}

} // namespace core

#endif // __RULE_H__
