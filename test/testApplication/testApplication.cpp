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
#include "../../gui/mainWindow.cpp"
#include "../../core/application.h"

using namespace core;

class testApplicaton : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testCreateDestroy();
    void testMainWindowState();
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

void testApplicaton::testMainWindowState()
{
    // Clean up the state and start main window.
    // Change main window size and position, close it and check whether its
    // state saved.
    cleanupTestCase();
    core::Application *app = core::Application::create();
    QVERIFY(app);

    gui::MainWindow *mw = app->mainWindow();
    QVERIFY(mw);
    mw->resize(333, 333);
    mw->move(333, 333);
    app->destroy();
    app = 0;
    mw = 0;

    // Create the application again and verify main window size and position.
    app = core::Application::create();
    QVERIFY(app);

    mw = app->mainWindow();
    QVERIFY(mw);

    QVERIFY(mw->size() == QSize(333, 333));
    QVERIFY(mw->pos() == QPoint(333, 333));
    app->destroy();
}

void testApplicaton::cleanupTestCase()
{
    // Find and delete temporary files in the directory that up by two levels
    // from the working directory of this application.
    QString strWd = QCoreApplication::applicationDirPath();

    if (QFile::exists(strWd + "/settings.ini"))
        QFile::remove(strWd + "/settings.ini");

    QDir wd(strWd);
    wd.cdUp();
    wd.cdUp();
    QString path = wd.absolutePath();

    if (QFile::exists(path + "/filters.xml"))
        QFile::remove(path + "/filters.xml");

    if (QFile::exists(path + "/posts.db"))
        QFile::remove(path + "/posts.db");
}

QTEST_MAIN(testApplicaton)
#include "testApplication.moc"
