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

    if(m_defaultManager)
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
    m_mainWindow = new gui::MainWindow;

    // Read and set all defaults
    m_defaultManager->readDefaults();

    // Restore Main Window state based on saved defaults
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

} // namespace core