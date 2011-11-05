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
    void testMatch_data();
    void testMatch();
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

void testFilter::testMatch_data()
{
    QTest::addColumn<QString>("title");
    QTest::addColumn<QString>("link");
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("userpic");
    QTest::addColumn<QString>("content");
    QTest::addColumn<QString>("category");
    QTest::addColumn<int>("posterid");

    QTest::addColumn<QString>("rule_name");
    QTest::addColumn<QString>("rule_value");
    QTest::addColumn<int>("rule_option");

    QTest::addColumn<bool>("result");

    QTest::newRow("0") << "This is title" << "link" << "name" << "userpic"
                       << "content" << "category" << 0
                       << "title" << "title" << 2 << true;
    QTest::newRow("1") << "This is title" << "link" << "name" << "userpic"
                       << "content" << "category" << 0
                       << "link" << "link" << 1 << true;
    QTest::newRow("2") << "This is title" << "link" << "name" << "userpic"
                       << "content&amp; &quot;&lt; &gt;&apos;&nbsp;<tag>"
                       << "category" << 1234567890
                       << "content" << "content& \"' " << 1 << true;
    QTest::newRow("3") << "This is title" << "link" << "name" << "userpic"
                       << "content&amp; &quot;&lt; &gt;&apos;&nbsp;<tag>"
                       << "category" << 1234567890
                       << "content" << "content" << 2 << true;
    QTest::newRow("4") << "" << "link" << "name" << "userpic"
                       << "content" << "category" << 0
                       << "title" << "" << 2 << true;
    QTest::newRow("5") << "" << "link" << "name" << "userpic"
                       << "content" << "category" << 0
                       << "title" << "" << 1 << true;

    // For ignored rules filter always should mismatch.
    QTest::newRow("6") << "Title" << "link" << "name" << "userpic"
                       << "content" << "category" << 42342
                       << "title" << "Title" << 0 << false;
    QTest::newRow("7") << "Title" << "link" << "name" << "userpic"
                       << "content" << "category" << 54553
                       << "title" << "tITlE" << 2 << true;
    QTest::newRow("8") << "Title" << "link" << "name" << "userpic"
                       << "content" << "category" << 233455
                       << "title" << "tITlE" << 1 << false;
    QTest::newRow("9") << " Title of\t this post \t " << "link" << "name" << "userpic"
                       << "content" << "category" << 233455
                       << "title" << "tITlE" << 2 << true;
}

void testFilter::testMatch()
{
    // Fetch the post and filter data.
    QFETCH(QString, title);
    QFETCH(QString, link);
    QFETCH(QString, name);
    QFETCH(QString, userpic);
    QFETCH(QString, content);
    QFETCH(QString, category);
    QFETCH(int, posterid);
    QFETCH(QString, rule_name);
    QFETCH(QString, rule_value);
    QFETCH(int, rule_option);
    QFETCH(bool, result);

    // Construct post.
    Post post;
    post.setValue("title", title);
    post.setValue("link", link);
    post.setValue("name", name);
    post.setValue("userpic", userpic);
    post.setValue("content", content);
    post.addTag(category);
    post.setValue("posterid", posterid);

    // Verify post properties values.
    QCOMPARE(post.value("title").toString(), title);
    QCOMPARE(post.value("link").toString(), link);
    QCOMPARE(post.value("name").toString(), name);
    QCOMPARE(post.value("userpic").toString(), userpic);
    QCOMPARE(post.value("content").toString(), content);
    QCOMPARE(post.value("posterid").toInt(), posterid);

    // Create a filter with rules and verify match function.
    Filter<Post> filter("Test filter");
    filter.setRule(rule_name, rule_value, (Filter<Post>::Option)rule_option);
    QCOMPARE(filter.match(post), result);
}

QTEST_MAIN(testFilter)
#include "testFilter.moc"
