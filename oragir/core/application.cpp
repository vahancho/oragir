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

#include <QStatusBar>
#include <QDesktopServices>
#include "application.h"
#include "defaultManager.h"
#include "versionManager.h"
#include "credentials.h"
#include "../parser/atomParser.h"
#include "../database/streamdatabase.h"
#include "../database/blogdatabase.h"
#include "../gui/mainWindow.h"
#include "../strings/strings.h"

namespace core
{

// Default main window size and position
const int defSizeX  = 800;
const int defSizeY  = 700;
const int defPosX   = 200;
const int defPosY   = 200;

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
    saveMainWindowDefaults();
    m_mainWindow->close();
    delete m_mainWindow;

    m_atomParser->stop();
    delete m_atomParser;

    saveDatabaseDefaults();
    delete m_streamDatabase;
    delete m_blogDatabase;

    delete m_versionManager;

    m_defaultManager->setValue("Blog/Credentials", m_credentials->encode());
    delete m_credentials;

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
    // Set the application name.
    QCoreApplication::setApplicationName(str::AppName);

    // Create all application components
    m_defaultManager = new DefaultManager;

    m_credentials = new Credentials;
    m_defaultManager->addProperty("Blog/Credentials", QString(), QString());

    m_versionManager = new VersionManager;
    m_defaultManager->addProperty(str::CheckUpdates, bool(true), bool(true));

    m_streamDatabase = new StreamDatabase;
    m_blogDatabase = new BlogDatabase;
    registerDatabaseDefaults();

    m_atomParser = new AtomParser;
    m_defaultManager->addProperty(str::Reconnect, bool(true), bool(true));
    m_defaultManager->addProperty(str::ReconnectCount, int(20), int(20));
    QObject::connect(m_atomParser, SIGNAL(fetched(const Post &, const Blog &)),
                     m_streamDatabase, SLOT(onFetched(const Post &, const Blog &)));

    m_mainWindow = new gui::MainWindow;
    registerMainWindowDefaults();
    QObject::connect(m_streamDatabase, SIGNAL(recordInserted(const QString &)),
                     m_mainWindow, SLOT(onRecordInserted(const QString &)));
    QObject::connect(m_streamDatabase, SIGNAL(tableAdded(const QString &)),
                     m_mainWindow, SLOT(createFolderView(const QString &)));

    // Read and set all defaults.
    m_defaultManager->readDefaults();

    restoreDatabase();
    restoreMainWindow();
    m_mainWindow->show();

    // Check for updates if needed.
    if (m_defaultManager->value(str::CheckUpdates).toBool() == true) {
        m_versionManager->checkForUpdates();
    }

    // Restore user credentials.
    m_credentials->fromEncoded(m_defaultManager->value("Blog/Credentials").toString());
}

gui::MainWindow *Application::mainWindow() const
{
    return m_mainWindow;
}

DefaultManager *Application::defaultManager() const
{
    return m_defaultManager;
}

StreamDatabase *Application::streamDatabase() const
{
    return m_streamDatabase;
}

BlogDatabase *Application::blogDatabase() const
{
    return m_blogDatabase;
}

AtomParser *Application::streamParser() const
{
    return m_atomParser;
}

VersionManager *Application::versionManager() const
{
    return m_versionManager;
}

Credentials *Application::credentials() const
{
    return m_credentials;
}

void Application::registerDatabaseDefaults() const
{
    Q_ASSERT(m_defaultManager);
    Q_ASSERT(m_streamDatabase);

    // Get settings directory and create it if it does not exist.
    QString location = settingsDirectory();

    QString appDataFile = QString(location + "oragir.data");
    m_defaultManager->addProperty(str::Database, appDataFile, appDataFile);

    m_defaultManager->addProperty("Blog/Database", QString(), QString());

    QString filterFile = QString(location + "filters.flt");
    m_defaultManager->addProperty(str::Filters, filterFile, filterFile);
}

void Application::restoreDatabase() const
{
    Q_ASSERT(m_defaultManager);
    Q_ASSERT(m_streamDatabase);

    // Stream database
    QString database = m_defaultManager->value(str::Database).toString();
    if (m_streamDatabase->create(database)) {
        QStringList tables = m_streamDatabase->tables();
        tables.removeAll(str::BlogTableName);
        foreach(const QString &table, tables) {
            m_streamDatabase->addStreamTable(table);
        }
    }

    QString filtersFile = m_defaultManager->value(str::Filters).toString();
    m_streamDatabase->openFilters(filtersFile);

    // Blog database
    database = m_defaultManager->value("Blog/Database").toString();
    if (!database.isEmpty())
        m_blogDatabase->create(database);
}

void Application::saveDatabaseDefaults() const
{
    Q_ASSERT(m_defaultManager);
    Q_ASSERT(m_streamDatabase);

    m_defaultManager->setValue(str::Database, m_streamDatabase->databaseName());
    QString filtersFile = m_defaultManager->value(str::Filters).toString();
    if (!filtersFile.isEmpty())
        m_streamDatabase->saveFilters(filtersFile);

    m_defaultManager->setValue("Blog/Database", m_blogDatabase->databaseName());
}

void Application::registerMainWindowDefaults() const
{
    Q_ASSERT(m_defaultManager);

    m_defaultManager->addProperty(str::MainWindowSize, QSize(defSizeX, defSizeY), QSize(defSizeX, defSizeY));
    m_defaultManager->addProperty(str::MainWindowPos, QPoint(defPosX, defPosY), QPoint(defPosX, defPosY));
    m_defaultManager->addProperty(str::MainWindowMax, bool(false), bool(false));
    m_defaultManager->addProperty(str::MainWindowState, QByteArray(), QByteArray());
    m_defaultManager->addProperty(str::ShowStatusBar, bool(true), bool(true));
    m_defaultManager->addProperty(str::QuitOnClose, bool(false), bool(false));
}

void Application::restoreMainWindow() const
{
    Q_ASSERT(m_defaultManager);
    Q_ASSERT(m_mainWindow);

    m_mainWindow->setupBlogView();

    // Set maximized state
    bool max = m_defaultManager->value(str::MainWindowMax).toBool();
    if(max)
        m_mainWindow->showMaximized();

    // Set window size
    QSize size = m_defaultManager->value(str::MainWindowSize).toSize();
    m_mainWindow->resize(size);

    // Set window position
    QPoint pos = m_defaultManager->value(str::MainWindowPos).toPoint();
    m_mainWindow->move(pos);

    // Restore tool bars state
    QByteArray state = m_defaultManager->value(str::MainWindowState).toByteArray();
    m_mainWindow->restoreState(state);

    // Restore status bar state
    bool showStatus = m_defaultManager->value(str::ShowStatusBar).toBool();
    m_mainWindow->setStatusBarVisible(showStatus);
}

void Application::saveMainWindowDefaults() const
{
    Q_ASSERT(m_defaultManager);
    Q_ASSERT(m_mainWindow);

    // Save Main Window configuration to defaults
    m_defaultManager->setValue(str::MainWindowSize, m_mainWindow->size());
    m_defaultManager->setValue(str::MainWindowPos, m_mainWindow->pos());
    m_defaultManager->setValue(str::MainWindowMax, m_mainWindow->isMaximized());
    m_defaultManager->setValue(str::MainWindowState, m_mainWindow->saveState());
    m_defaultManager->setValue(str::ShowStatusBar, m_mainWindow->statusBarVisible());
}

QString Application::settingsDirectory() const
{
    // Get settings directory and create it if it does not exist.
    QString settingsDir =
       QDesktopServices::storageLocation(QDesktopServices::DataLocation) + '/';
    settingsDir = QDir::toNativeSeparators(settingsDir);
    QDir dir(settingsDir);
    if (!dir.exists())
        dir.mkpath(settingsDir);

    return settingsDir;
}

} // namespace core
