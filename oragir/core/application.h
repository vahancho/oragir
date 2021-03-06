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

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

// Forward declarations
namespace gui
{
    class MainWindow;
}

namespace core
{

class DefaultManager;
class AtomParser;
class StreamDatabase;
class BlogDatabase;
class VersionManager;
class Credentials;

/// Application class implements the core of the program.
/*!
    The only one instance of Application allowed in this program.
    Application class is responsible for Main Window, DefaultManager
    and other components creation. Application instance accessible from
    anywhere in the program, so sub systems can always use it.
*/
class Application
{

public:
    /// Creates the only instance of Application and returns the pointer to it.
    /*!
        This member function is the only way for creating the Application
        instance. Function returns the pointer to Application instance after its
        creation. If Application already exists, function just returns that
        pointer without creating new one.
    */
    static Application *create();

    /// Destroys the Application.
    /*!
        This function destroys the Application's instance.
    */
    static void destroy();

    /// Return the pointer to Application object.
    static Application *theApp();

    /// Returns the Main Window pointer.
    gui::MainWindow *mainWindow() const;

    /// Returns the Default Manager.
    DefaultManager *defaultManager() const;

    /// Returns the streaming database manager.
    StreamDatabase *streamDatabase() const;

    /// Returns the blog database manager.
    BlogDatabase *blogDatabase() const;

    /// Returns the Sream parser.
    AtomParser *streamParser() const;

    /// Returns the version manager.
    VersionManager *versionManager() const;

    Credentials *credentials() const;

    /// Creates settings directory and returns its path.
    QString settingsDirectory() const;

private:
    /// Default constructor.
    /*!
        Default constructor declared as private to allow controlling creation
        of the instance of this class through the static function. Note, that
        the only one instance of application allowed.
    */
    Application();

    /// Destructor.
    /*!
        Instances of this class supposed to be created and destroyed through
        the static member functions. Therefore both constructor and
        destructor declared as private member functions of the class.
    */
    ~Application();

    /// Initialize the Application's components.
    /*!
        Creates the Application components such as MainWindow and DefaultManager.
    */
    void init();

    /// Create and initializes the database defaults.
    void registerDatabaseDefaults() const;

    /// Restores the database state.
    void restoreDatabase() const;

    /// Saves database defaults.
    void saveDatabaseDefaults() const;

    /// Create and initializes main window defaults.
    void registerMainWindowDefaults() const;

    /// Restores the window's state based on defaults values.
    /*!
        Window's state is saved on closing the Application and restore on
        next session. State includes window's size, position, dock windows
        geometries and tool bars state.
    */
    void restoreMainWindow() const;

    /// Saves database defaults.
    void saveMainWindowDefaults() const;

    /// Pointer to the only existing object of Application.
    static Application *m_theApp;

    /// The Default manager.
    DefaultManager *m_defaultManager;

    /// The Main Window.
    gui::MainWindow *m_mainWindow;

    /// The stream database.
    StreamDatabase *m_streamDatabase;

    /// The blog database.
    BlogDatabase *m_blogDatabase;

    /// The atom parser.
    AtomParser *m_atomParser;

    /// The version manager.
    VersionManager *m_versionManager;

    /// Store credentials.
    Credentials *m_credentials;
};

} // namespace core

#endif // __APPLICATION_H__
