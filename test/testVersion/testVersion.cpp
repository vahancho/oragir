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
#include "../../core/version.h"

using namespace core;

class testVersion : public QObject
{
    Q_OBJECT

private slots:
    void test_data();
    void test();
    void versionCompare_data();
    void versionCompare();
};

void testVersion::test_data()
{
    QTest::addColumn<QString>("versionIn");
    QTest::addColumn<QString>("versionOut");

    QTest::newRow("version1") << "1" << "1";
    QTest::newRow("version2") << "1.1" << "1.1";
    QTest::newRow("version3") << "0.1" << "0.1";
    QTest::newRow("version4") << "0.0" << "0.0";
    QTest::newRow("version5") << "00" << "0";
    QTest::newRow("version6") << "0.0a" << "0.0a";
    QTest::newRow("version7") << "0a.0a" << "0.0a";
    QTest::newRow("version8") << "1.0a" << "1.0a";
    QTest::newRow("version9") << "1.0 a" << "1.0 a";
    QTest::newRow("version10") << "1.0 (abc)" << "1.0 (abc)";
    QTest::newRow("version11") << "1.0          (abc)" <<
                                  "1.0          (abc)";
    QTest::newRow("version12") << "1.2.3.4.5.6.7.8.9 (alpha)" <<
                                  "1.2.3.4.5.6.7.8.9 (alpha)";
    QTest::newRow("version13") << "10.29.39.49.59.96.97.98.99 (alpha)" <<
                                  "10.29.39.49.59.96.97.98.99 (alpha)";
    QTest::newRow("version14") << "101.291.391.491.591.961.971.981.991 (alpha)" <<
                                  "101.291.391.491.591.961.971.981.991 (alpha)";
    QTest::newRow("version15") << "" <<
                                  "";
}

void testVersion::test()
{
    QFETCH(QString, versionIn);
    QFETCH(QString, versionOut);

    // Input version.
    QByteArray ba = versionIn.toAscii();
    const char *cc = ba.data();

    {
        // Use constructor.
        Version v(cc);
        // Read version and compare.
        std::string out = v.toString();
        QString qOut = QString::fromStdString(out);
        QCOMPARE(qOut, versionOut);
    }

    {
        // Use constructor.
        Version v;
        // Set version and compare again.
        v.fromString(cc);
        std::string out = v.toString();
        QString qOut = QString::fromStdString(out);
        QCOMPARE(qOut, versionOut);
    }

    {
        // Use constructor.
        Version v(cc);

        // Set version and compare again.
        v.fromString(cc);
        std::string out = v.toString();
        QString qOut = QString::fromStdString(out);
        QCOMPARE(qOut, versionOut);
    }
}

void testVersion::versionCompare_data()
{
    QTest::addColumn<QString>("version1");
    QTest::addColumn<QString>("version2");
    QTest::addColumn<bool>("result");

    QTest::newRow("version1") << "1" << "1" << false;
    QTest::newRow("version2") << "0.1" << "1.1" << false;
    QTest::newRow("version3") << "1.0" << "0.1" << true;
    QTest::newRow("version4") << "1.0a" << "1.0a" << false;
    QTest::newRow("version5") << "1.0b" << "1.0a" << false;
    QTest::newRow("version6") << "0.234" << "0.233" << true;
    QTest::newRow("version7") << "0.2345a" << "0.23444b" << false;
    QTest::newRow("version8") << "0.10000 a" << "0.2345a" << true;
    QTest::newRow("version9") << "1.2.3.4.5.6 beta" << "1.2.3.4.5.5 beta" << true;
}

void testVersion::versionCompare()
{
    QFETCH(QString, version1);
    QFETCH(QString, version2);
    QFETCH(bool, result);

    QByteArray ba1 = version1.toAscii();
    const char *cc1 = ba1.data();
    QByteArray ba2 = version2.toAscii();
    const char *cc2 = ba2.data();

    Version v1(cc1);
    Version v2(cc2);

    QCOMPARE(v1 > v2, result);
}

QTEST_MAIN(testVersion)
#include "testVersion.moc"
