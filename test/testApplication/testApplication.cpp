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
#include "../../core/application.h"

using namespace core;

class testApplicaton : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testCreateDestroy();
    void cleanupTestCase();
};

void testApplicaton::initTestCase()
{
}

void testApplicaton::testCreateDestroy()
{
    core::Application *app = core::Application::create();
    QVERIFY(app);
    QVERIFY(app->theApp());
    QVERIFY(core::Application::theApp() == app);

    app->destroy();
    QVERIFY(core::Application::theApp() == 0);
}

void testApplicaton::cleanupTestCase()
{
    // Find and delete temporary files in the directory that up by two levels
    // from the working directory of this application.
    QString strWd = QCoreApplication::applicationDirPath();
    QDir wd(strWd);
    wd.cdUp();
    wd.cdUp();
    strWd = wd.absolutePath();

    if (QFile::exists(strWd + "/settings.ini"))
        QFile::remove(strWd + "/settings.ini");

    if (QFile::exists(strWd + "/filters.xml"))
        QFile::remove(strWd + "/filters.xml");

    if (QFile::exists(strWd + "/posts.db"))
        QFile::remove(strWd + "/posts.db");
}

QTEST_MAIN(testApplicaton)
#include "testApplication.moc"