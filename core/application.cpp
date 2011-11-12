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

#include "application.h"
#include "defaultManager.h"
#include "../parser/atomParser.h"
#include "../database/database.h"
#include "../gui/mainWindow.h"

namespace core
{

// Definition of the pointer to the only application object.
Application *Application::m_theApp = 0;

Application::Application()
    :
        m_defaultManager(0),
        m_mainWindow(0)
{
    // Initialize the application pointer with this instance.
    m_theApp = this;

    // Initialize Application components.
    init();
}

Application::~Application()
{
    // Delete Application components.
    if(m_mainWindow)
        m_mainWindow->close();

    delete m_mainWindow;

    m_atomParser->stop();
    delete m_atomParser;

    saveDatabaseDefaults();
    delete m_dataBase;

    // Save defaults
    m_defaultManager->saveDefaults();
    delete m_defaultManager;
}

Application *Application::create()
{
    // If application exists - return it, otherwise return the newly created one.
    if(m_theApp)
        return m_theApp;

    return new Application;
}

Application *Application::theApp()
{
    return m_theApp;
}

void Application::destroy()
{
    delete m_theApp;
    m_theApp = 0;
}

void Application::init()
{
    // Create all application components
    m_defaultManager = new DefaultManager;
    m_dataBase = new Database;
    registerDatabaseDefaults();

    m_atomParser = new AtomParser;
    QObject::connect(m_atomParser, SIGNAL(fetched(const Post &, const Blog &)),
                     m_dataBase, SLOT(onFetched(const Post &, const Blog &)));

    m_mainWindow = new gui::MainWindow;
    QObject::connect(m_dataBase, SIGNAL(recordInserted(const QSqlDatabase &, const QString &)),
                     m_mainWindow, SLOT(onRecordInserted(const QSqlDatabase &, const QString &)));

    // Read and set all defaults.
    m_defaultManager->readDefaults();

    restoreDatabase();
    m_mainWindow->restoreWindow();
    m_mainWindow->show();
}

gui::MainWindow *Application::mainWindow() const
{
    return m_mainWindow;
}

DefaultManager *Application::defaultManager() const
{
    return m_defaultManager;
}

Database *Application::database() const
{
    return m_dataBase;
}

AtomParser *Application::streamParser() const
{
    return m_atomParser;
}

void Application::registerDatabaseDefaults() const
{
    Q_ASSERT(m_defaultManager);
    Q_ASSERT(m_dataBase);

    m_defaultManager->addProperty("Databases", QStringList(), QStringList());

    QString filterFile = QCoreApplication::applicationDirPath() + "/filters.flt";
    filterFile = QDir::toNativeSeparators(filterFile);
    m_defaultManager->addProperty("Filters", filterFile, filterFile);
}

void Application::restoreDatabase() const
{
    Q_ASSERT(m_defaultManager);
    Q_ASSERT(m_dataBase);

    QStringList databases = m_defaultManager->value("Databases").toStringList();
    foreach(const QString &db, databases) {
        if (m_dataBase->create(db)) {
            m_mainWindow->setDatabaseTable(m_dataBase->database(db), "post");
        }
    }
    QString filtersFile = m_defaultManager->value("Filters").toString();
    m_dataBase->openFilters(filtersFile);
}

void Application::saveDatabaseDefaults() const
{
    Q_ASSERT(m_defaultManager);
    Q_ASSERT(m_dataBase);

    m_defaultManager->setValue("Databases", m_dataBase->databases());
    QString filtersFile = m_defaultManager->value("Filters").toString();
    if (!filtersFile.isEmpty())
        m_dataBase->saveFilters(filtersFile);
}

} // namespace core
