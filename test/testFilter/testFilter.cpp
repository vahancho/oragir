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

#include <QtTest/QtTest>
#include "../../parser/filter.h"

using namespace core;

class testFilter : public QObject
{
    Q_OBJECT

private slots:
    void testName();
    void testEnabled();
    void testRuleMatch();
    void testSetRule_data();
    void testSetRule();
};

void testFilter::testName()
{
    Filter<Post> filter1("Test filter 1");
    QCOMPARE(filter1.name(), QString("Test filter 1"));

    Filter<Post> filter2("Test filter 2");
    QCOMPARE(filter2.name(), QString("Test filter 2"));

    // Test operator <
    QVERIFY(filter1 < filter2);
}

void testFilter::testEnabled()
{
    Filter<Post> filter("Test filter");

    // Test default enabled value.
    QCOMPARE(filter.enabled(), true);

    filter.setEnabled(false);
    QCOMPARE(filter.enabled(), false);

    filter.setEnabled(true);
    QCOMPARE(filter.enabled(), true);
}

void testFilter::testRuleMatch()
{
    Filter<Post> filter("Test filter");

    // Test default value.
    QCOMPARE(filter.ruleMatch(), Filter<Post>::One);

    filter.setRuleMatch(Filter<Post>::All);
    QCOMPARE(filter.ruleMatch(), Filter<Post>::All);

    filter.setRuleMatch(Filter<Post>::One);
    QCOMPARE(filter.ruleMatch(), Filter<Post>::One);
}

void testFilter::testSetRule_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("value");
    QTest::addColumn<int>("option");
    QTest::addColumn<bool>("result");

    // Non existent properties' rules.
    QTest::newRow("non-existent0") << "foo" << "foo" << 0 << false;
    QTest::newRow("non-existent1") << "" << "foo" << 0 << false;
    QTest::newRow("non-existent2") << "" << "" << 0 << false;
    QTest::newRow("non-existent3") << "foo" << "" << 0 << false;

    // Existing properties, 'Ignore' option.
    QTest::newRow("existent") << "posterid" << "foo" << 0 << true;
    QTest::newRow("existent") << "link" << "foo" << 0 << true;
    QTest::newRow("existent") << "updated" << "foo" << 0 << true;
    QTest::newRow("existent") << "name" << "foo" << 0 << true;
    QTest::newRow("existent") << "content" << "foo" << 0 << true;
    QTest::newRow("existent") << "title" << "foo" << 0 << true;
    QTest::newRow("existent") << "userpic" << "foo" << 0 << true;
    QTest::newRow("existent") << "category" << "foo" << 0 << true;

    // Existing properties, 'ExactMatch' option.
    QTest::newRow("existent") << "posterid" << "foo" << 1 << true;
    QTest::newRow("existent") << "link" << "foo" << 1 << true;
    QTest::newRow("existent") << "updated" << "foo" << 1 << true;
    QTest::newRow("existent") << "name" << "foo" << 1 << true;
    QTest::newRow("existent") << "content" << "foo" << 1 << true;
    QTest::newRow("existent") << "title" << "foo" << 1 << true;
    QTest::newRow("existent") << "userpic" << "foo" << 1 << true;
    QTest::newRow("existent") << "category" << "foo" << 1 << true;

    // Existing properties, 'Contains' option.
    QTest::newRow("existent") << "posterid" << "foo" << 2 << true;
    QTest::newRow("existent") << "link" << "foo" << 2 << true;
    QTest::newRow("existent") << "updated" << "foo" << 2 << true;
    QTest::newRow("existent") << "name" << "foo" << 2 << true;
    QTest::newRow("existent") << "content" << "foo" << 2 << true;
    QTest::newRow("existent") << "title" << "foo" << 2 << true;
    QTest::newRow("existent") << "userpic" << "foo" << 2 << true;
    QTest::newRow("existent") << "category" << "foo" << 2 << true;

    // Existing properties, empty value.
    QTest::newRow("existent") << "posterid" << "" << 2 << true;
    QTest::newRow("existent") << "link" << "" << 2 << true;
    QTest::newRow("existent") << "updated" << "" << 2 << true;
    QTest::newRow("existent") << "name" << "" << 2 << true;
    QTest::newRow("existent") << "content" << "" << 2 << true;
    QTest::newRow("existent") << "title" << "" << 2 << true;
    QTest::newRow("existent") << "userpic" << "" << 2 << true;
    QTest::newRow("existent") << "category" << "" << 2 << true;

    // Existing properties, multy word value.
    QTest::newRow("existent") << "posterid" << "foo goo hoo" << 2 << true;
    QTest::newRow("existent") << "link" << "foo goo hoo" << 2 << true;
    QTest::newRow("existent") << "updated" << "foo goo hoo" << 2 << true;
    QTest::newRow("existent") << "name" << "foo goo hoo" << 2 << true;
    QTest::newRow("existent") << "content" << "foo goo hoo" << 2 << true;
    QTest::newRow("existent") << "title" << "foo goo hoo" << 2 << true;
    QTest::newRow("existent") << "userpic" << "foo goo hoo" << 2 << true;
    QTest::newRow("existent") << "category" << "foo goo hoo" << 2 << true;
}

void testFilter::testSetRule()
{
    Filter<Post> filter("Test filter");

    QFETCH(QString, name);
    QFETCH(QString, value);
    QFETCH(int, option);
    QFETCH(bool, result);

    QCOMPARE(filter.setRule(name, value, (Filter<Post>::Option)option), result);
}

QTEST_MAIN(testFilter)
#include "testFilter.moc"
