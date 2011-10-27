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
class Filter
{
public:
    Filter(const QString &name);

    enum Option {
        Ignore = 0,
        ExactMatch,
        Contains
    };

    /// Enumerate rules matching creteria.
    enum RuleMatch {
        One = 0, // At least one matches.
        All      // All non empty rules match.
    };

    bool setRule(const QString &name, const QString &value, Option opt);
    bool match(const Source &source) const;
    QString name() const;
    bool enabled() const;
    void setEnabled(bool enabled = true);
    RuleMatch ruleMatch() const;
    void setRuleMatch(RuleMatch rm = One);

    void writeXml(QXmlStreamWriter &writer);
    void readXml(QXmlStreamReader &reader);

    bool operator<(const Filter<Source> &other) const;

private:
    // Rules results.
    enum Result {
        Matched = 0,
        NotMatched,
        Undefined
    };

    /// Implements the filter's rule class.
    struct Rule
    {
        Rule()
            :
                m_option(Ignore)
        {}
        QString m_value;
        Option m_option;
    };

    /// Returns result of applying rule named 'name' on a given value.
    Result match(const QString &name, const QString &value) const;

    QMap<QString, Rule> m_rules;
    QString m_name;
    bool m_enabled;
    RuleMatch m_ruleMatch;
};

///////////////////////////////////////////////////////////////////////////////
// Implementation
//

template<class Source>
Filter<Source>::Filter(const QString &name)
    :
        m_name(name),
        m_enabled(true),
        m_ruleMatch(One)
{
    Source source;
    QMap<QString, QVariant> properties = source.propertyMap();

    QMap<QString, QVariant>::const_iterator it = properties.constBegin();
    while (it != properties.constEnd()) {
        Rule flt;
        const QString &name = it.key();
        m_rules[name] = flt;

        ++it;
    }
}

template<class Source>
bool Filter<Source>::setRule(const QString &name, const QString &value,
                             Option opt)
{
    QMap<QString, Rule>::iterator it = m_rules.find(name);
    if (it != m_rules.end()) {
        Rule &rl = it.value();
        rl.m_value = value;
        rl.m_option = opt;

        return true;
    } else {
        return false;
    }
}

template<class Source>
bool Filter<Source>::match(const Source &source) const
{
    QMap<QString, QVariant> properties = source.propertyMap();

    // Iterate over all properties and apply rule on each.
    QMap<QString, QVariant>::const_iterator it = properties.constBegin();
    while (it != properties.constEnd()) {
        Result res = match(it.key(), it.value().toString());

        // Make decision depending on rule matching criteria.
        switch (m_ruleMatch) {
            case One:
                // If at lease one rule matched, we succeed.
                if (res == Matched)
                    return true;
            break;
            case All:
                // If one of the rule does not match, we failed.
                if (NotMatched)
                    return false;
            break;
            default:
                break;
        }
        ++it;
    }

    return false;
}

template<class Source>
typename Filter<Source>::Result Filter<Source>::match(const QString &name,
                                                      const QString &value) const
{
    QMap<QString, Rule>::iterator it = m_rules.find(name);
    if (it != m_rules.end()) {
        const Rule &rule = it.value();

        // Strip the html tags before comparing strings.
        QString stripped = value;
        stripped.replace("&amp;", "&");
        stripped.replace("&quot;", "\"");
        stripped.replace("&lt;", "<");
        stripped.replace("&gt;", ">");
        stripped.replace("&apos;", "'");
        stripped.replace("&nbsp;", " ");
        stripped.replace( QRegExp("<[^>]*>"), "" );

        if (rule.m_option == Ignore) {
            return Undefined;
        } else if (rule.m_option == Contains &&
                   stripped.contains(rule.m_value, Qt::CaseInsensitive)) {
            QRegExp rx;
            QString pat = QString("\\s.{0,40}%1.{0,40}\\s").arg(rule.m_value);
            rx.setPattern(pat);
            if (rx.indexIn(stripped) != -1) {
                QString s = rx.cap(0);
                qDebug(s.toAscii().data());
            }
            return Matched;
        } else if (rule.m_option == ExactMatch &&
                   stripped == rule.m_value) {
            return Matched;
        } else {
            return NotMatched;
        }
    } else {
        // Such property not found.
        return Undefined;
    }
}

template<class Source>
QString Filter<Source>::name() const
{
    return m_name;
}

template<class Source>
bool Filter<Source>::enabled() const
{
    return m_enabled;
}

template<class Source>
void Filter<Source>::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

template<class Source>
typename Filter<Source>::RuleMatch Filter<Source>::ruleMatch() const
{
    return m_ruleMatch;
}

template<class Source>
void Filter<Source>::setRuleMatch(RuleMatch rm)
{
    m_ruleMatch = rm;
}

template<class Source>
void Filter<Source>::writeXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement(str::TagFilter);
    writer.writeAttribute(str::TagName, m_name);

    QMap<QString, Rule>::const_iterator it = m_rules.constBegin();
    while (it != m_rules.constEnd()) {
        const Rule &flt = it.value();
        const QString &name = it.key();

        writer.writeStartElement(str::TagRule);

        writer.writeAttribute(str::TagName, name);
        writer.writeTextElement(str::TagValue, flt.m_value);
        writer.writeTextElement(str::TagOption, QString::number(flt.m_option));

        writer.writeEndElement(); // rule

        ++it;
    }

    writer.writeEndElement(); // filter
}

template<class Source>
void Filter<Source>::readXml(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.isStartElement());

    reader.readNext();
    QString ruleName;
    QString ruleValue;
    Option option;
    QString elementName;

    while(!(reader.isEndElement() && reader.name() == str::TagFilter)) {
        reader.readNext();
        if(reader.isStartElement()) {
            elementName = reader.name().toString();
            if (elementName == str::TagRule)
                ruleName = reader.attributes().value(str::TagNameAttr).toString();
        } else if (reader.isEndElement()) {
            if (reader.name().toString() == str::TagRule) {
                if (option != Ignore)
                    setRule(ruleName, ruleValue, option);
            }
        } else if (reader.isCharacters() && !reader.isWhitespace()) {
            if (elementName == str::TagValue)
                ruleValue = reader.text().toString();
            else if (elementName == str::TagOption)
                option = (Option)reader.text().toString().toInt();
        }
    }
}

template<class Source>
bool Filter<Source>::operator<(const Filter<Source> &other) const
{
    return m_name < other.m_name;
}

} // namespace core

#endif // __RULE_H__
