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

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include <QMdiArea>
#include <QSystemTrayIcon>
#include <QtSql>

class QSignalMapper;
class QProgressBar;

namespace gui
{

/// Implements the Application's Main Window class.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /// The constructor.
    MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);

    /// Destructor.
    ~MainWindow();

    /// Restores the window's state based on defaults values.
    /*!
        Window's state is saved on closing the Application and restore on
        next session. State includes window's size, position, dock windows
        geometries and tool bars state.
    */
    void restoreWindow();

    /// Set up the table mdi child view for the given database table.
    void setDatabaseTable(const QSqlDatabase &db, const QString &table);

public slots:
    void onRecordInserted(const QSqlDatabase &db, const QString &table);

protected:
    /// Handles the window's close event.
    /*!
        Saves MainWindow geometry and settings.
    */
    void closeEvent(QCloseEvent *event);

private slots:
    /// Opens Options dialog.
    void onToolsOptions();

    /// This slot is called when Tool bars sub menu is about to show.
    void updateToolBarsMenu();

    /// This slot is called when Views sub menu is about to show.
    void updateViewsMenu();

    /// This slot is called when Window menu is about to show.
    void updateWindowMenu();

    /// This slot is called when Status bar action is triggered.
    void onStatusBarShowHide();

    /// This slot called when a MDI sub window activated.
    /*!
        @param subWindow The MDI child window that has been activated.
        \sa setActiveSubWindow(), activeSubWindow()
    */
    void onSubWindowActivated(QMdiSubWindow *subWindow);

    /// This slot activates MDI sub window.
    /*!
        @param subWindow The MDI child window that will be activated.
        \sa activeSubWindow(), onSubWindowActivated()
    */
    void setActiveSubWindow(QWidget *subWindow);

    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

    void trayIconMessageClicked();

    /// This slot called when About menu action triggered.
    void onAbout();

    void onStreamStart();

    void onStreamStop();

    void onDatabaseOpen();

private:
    /// Saves the window state.
    void saveWindow() const;

    /// Returns the active MDI sub window.
    /*!
        Returns the active MDI sub window if any, otherwise returns 0.
        \sa setActiveSubWindow(), onSubWindowActivated()
    */
    QMdiSubWindow *activeSubWindow() const;

    /// Construct and add menus.
    void createMenus();

    /// Create dockable windows.
    void createDockWindows();

    /// Registers and set Main Window defaults.
    void addDefaults() const;

    /// Creates the system tray icon.
    void createTrayIcon();

    void storeInClipboard(const QString &text) const;

    /// The system tray icon.
    QSystemTrayIcon *m_trayIcon;

    /// Store the "Status bar" action.
    QAction *m_statusBarAction;

    /// Store the "Close" action.
    QAction *m_closeAct;

    /// Store the "Close all" action.
    QAction *m_closeAllAct;

    /// Store the "Tile" action.
    QAction *m_tileAct;

    /// Store the "Cascade" action.
    QAction *m_cascadeAct;

    /// Store the "Next" action.
    QAction *m_nextAct;

    /// Store the "Previous" action.
    QAction *m_previousAct;

    /// Store the separator action.
    QAction *m_separatorAct;

    /// Store the Window menu.
    QMenu *m_windowMenu;

    /// Store the 'Toolbars' menu.
    QMenu *m_toolBarsMenu;

    /// Store the 'Views' menu.
    QMenu *m_viewsMenu;

    /// The signal mapper for MDI child windows.
    QSignalMapper *m_windowMapper;

    /// Workspace for MDI application.
    QMdiArea m_mdiArea;
};

} // namespace gui

#endif // __MAINWINDOW_H__
