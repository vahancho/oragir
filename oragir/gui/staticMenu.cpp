/**************************************************************************
*   Copyright (C) 2012 by Vahan Aghajanyan                                *
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

#include <QEvent>
#include "staticMenu.h"

namespace gui
{

StaticMenu::StaticMenu(QWidget *parent)
    :
        QMenu(parent),
        m_currentAction(0)
{
    init();
}

StaticMenu::StaticMenu(const QString &title, QWidget *parent)
    :
        QMenu(title, parent),
        m_currentAction(0)
{
    init();
}

StaticMenu::~StaticMenu()
{}

bool StaticMenu::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        if (m_currentAction && m_currentAction->isCheckable()) {
            m_currentAction->setChecked(!m_currentAction->isChecked());
            emit triggered(m_currentAction);
        }
        return true;
    } else if (event->type() == QEvent::Hide ||
               event->type() == QEvent::HoverLeave) {
        m_currentAction = 0;
    }
    // Propagate to the parent class.
    return QObject::eventFilter(obj, event);
}

void StaticMenu::onActionHovered(QAction *act)
{
    m_currentAction = act;
}

void StaticMenu::init()
{
    installEventFilter(this);
    connect(this, SIGNAL(hovered(QAction *)),
            this, SLOT(onActionHovered(QAction *)));
}

void StaticMenu::uncheckAll()
{
    QList<QAction *> actionList = actions();
    foreach (QAction *action, actionList) {
        if (action->isCheckable())
            action->setChecked(false);
    }
}

void StaticMenu::check(const QString &actionName)
{
    QList<QAction *> actionList = actions();
    foreach (QAction *action, actionList) {
        if (action->isCheckable() && action->text() == actionName)
            action->setChecked(true);
    }
}

} // namespace gui
