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

/// Implements the filtering functionality.
/*
    This template class implemente the filtering support for data
    providers such as Post and Blog. Data source difined in the
    template argument and supposed to be a type of property table.
    Usage example: Filter<Post> - defines filter for the Post
    property table. Similarly Filter<Blog> provides filtering
    for Blog properties.
*/
template<class Source>
class Filter
{
public:
    /// Constructs the named filter.
    Filter(const QString &name = QString());

    /// Defines rules values matching options.
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

     /// Implements the filter's rule class.
    class Rule
    {
    public:
        Rule(const QString &value = QString(), Option option = Ignore)
            :
                m_value(value),
                m_option(option)
        {}

        QString value() const
        {
            return m_value;
        }

        Option option() const
        {
            return m_option;
        }

    private:
        QString m_value;
        Option m_option;
    };

    /// Sets the new filtering rule.
    /*
        @param name  The property name,
        @param value The value to compare property value with,
        @param opt   Matching option.
        @return      Returns rule adding result - success or not.
    */
    bool setRule(const QString &name, const QString &value, Option opt);

    typedef QMultiMap<QString, Rule> Rules;

    /// Returns the list of rules.
    const Rules &rules() const;

    /// Returns true if data matches to the filter rules.
    bool match(const Source &source) const;

    /// Returs the name of the filter.
    QString name() const;

    /// Returns whethe filter enabled or not.
    bool enabled() const;

    /// Set filter's enable state.
    void setEnabled(bool enabled = true);

    /// Returs how rules should match.
    RuleMatch ruleMatch() const;

    /// Sets how rules should match.
    void setRuleMatch(RuleMatch rm = One);

    /// Writes filter data with the given xml writer.
    void writeXml(QXmlStreamWriter &writer);

    /// Reads xml data with the given reader.
    void readXml(QXmlStreamReader &reader);

    /// Compare two filters.
    bool operator<(const Filter<Source> &other) const;

private:
    // Rules results.
    enum Result {
        Matched = 0,
        NotMatched,
        Undefined
    };

    /// Returns result of applying rule named 'name' on a given value.
    Result match(const QString &name, const QString &value) const;

    /// Returns true if rule for the given property supported.
    bool canAddRule(const QString &propertyName) const;

    Rules m_rules;
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
{}

template<class Source>
bool Filter<Source>::canAddRule(const QString &propertyName) const
{
    Source source;
    QMap<QString, QVariant> properties = source.propertyMap();
    return properties.find(propertyName) != properties.end();
}

template<class Source>
bool Filter<Source>::setRule(const QString &name, const QString &value,
                             Option opt)
{
    if (canAddRule(name)) {
        Rule rule(value, opt);
        m_rules.insert(name, rule);

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
    typename Rules::const_iterator it = m_rules.find(name);

    // Iterate over all rules with the given name until find a match.
    while (it != m_rules.end() && it.key() == name) {
        const Rule &rule = it.value();

        // For ignored rules return immediately.
        if (rule.option() == Ignore)
            return Undefined;

        // Strip the html tags before comparing strings.
        QString stripped = value;
        stripped.replace("&amp;", "&");
        stripped.replace("&quot;", "\"");
        stripped.replace("&lt;", "<");
        stripped.replace("&gt;", ">");
        stripped.replace("&apos;", "'");
        stripped.replace("&nbsp;", " ");
        stripped.replace( QRegExp("<[^>]*>"), "" );

        if (rule.option() == Contains &&
            stripped.contains(rule.value(), Qt::CaseInsensitive)) {
            QRegExp rx;
            QString pat = QString("\\s.{0,40}%1.{0,40}\\s").arg(rule.value());
            rx.setPattern(pat);
            if (rx.indexIn(stripped) != -1) {
                QString s = rx.cap(0);
                qDebug(s.toAscii().data());
            }
            return Matched;
        } else if (rule.option() == ExactMatch &&
                   stripped == rule.value()) {
            return Matched;
        }

        ++it;
    }

    // Such property not found or not matched.
    return NotMatched;
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
    writer.writeAttribute(str::TagEnabled,
                          QVariant::fromValue(m_enabled).toString());
    writer.writeAttribute(str::TagRuleMatch,
                          QString::number(m_ruleMatch));

    typename Rules::const_iterator it = m_rules.constBegin();
    while (it != m_rules.constEnd()) {
        const Rule &rule = it.value();
        const QString &name = it.key();

        writer.writeStartElement(str::TagRule);

        writer.writeAttribute(str::TagName, name);
        writer.writeTextElement(str::TagValue, rule.value());
        writer.writeTextElement(str::TagOption, QString::number(rule.option()));

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
    Option option = Ignore;
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
const typename Filter<Source>::Rules &Filter<Source>::rules() const
{
    return m_rules;
}

template<class Source>
bool Filter<Source>::operator<(const Filter<Source> &other) const
{
    return m_name < other.m_name;
}

} // namespace core

#endif // __RULE_H__
