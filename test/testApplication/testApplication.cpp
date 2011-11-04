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
#include "../../database/database.h"

using namespace core;

class testApplicaton : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testCreateDestroy();
    void testMainWindowState();
    void cleanupTestCase();
private:
    core::Application *createApplication() const;
};

core::Application *testApplicaton::createApplication() const
{
    core::Application *app = core::Application::create();
    core::Database *db = app->database();
    QString appPath = QCoreApplication::applicationDirPath();
    db->create(appPath + "/posts.db");

    core::Filter<core::Post> filter("Test filter");
    filter.setRule(str::TagContent, "test", core::Filter<core::Post>::Contains);
    db->addFilter(filter);

    return app;
}

void testApplicaton::initTestCase()
{}

void testApplicaton::testCreateDestroy()
{
    core::Application *app = core::Application::create();
    QVERIFY(app);
    QVERIFY(app->theApp());
    QVERIFY(core::Application::theApp() == app);
    QVERIFY(app->database());
    QVERIFY(app->mainWindow());
    QVERIFY(app->streamParser());

    app->destroy();
    QVERIFY(core::Application::theApp() == 0);
}

void testApplicaton::testMainWindowState()
{
    // Clean up the state and start main window.
    // Change main window size and position, close it and check whether its
    // state saved.
    cleanupTestCase();
    core::Application *app = createApplication();
    QVERIFY(app);

    gui::MainWindow *mw = app->mainWindow();
    QVERIFY(mw);
    mw->resize(333, 333);
    mw->move(333, 333);
    app->destroy();
    app = 0;
    mw = 0;

    // Create the application again and verify main window size and position.
    app = createApplication();
    QVERIFY(app);

    mw = app->mainWindow();
    QVERIFY(mw);

    QVERIFY(mw->size() == QSize(333, 333));
    QVERIFY(mw->pos() == QPoint(333, 333));
    app->destroy();
}

void testApplicaton::cleanupTestCase()
{
    // Find and delete temporary files from the current directory. 
    QString path = QCoreApplication::applicationDirPath();

    if (QFile::exists(path + "/settings.ini"))
        QFile::remove(path + "/settings.ini");

    if (QFile::exists(path + "/filters.xml"))
        QFile::remove(path + "/filters.xml");

    if (QFile::exists(path + "/posts.db"))
        QFile::remove(path + "/posts.db");
}

QTEST_MAIN(testApplicaton)
#include "testApplication.moc"
