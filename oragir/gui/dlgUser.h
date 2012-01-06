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

#ifndef __DLGEXPORT_H__
#define __DLGEXPORT_H__

#include <QDialog>

class QLineEdit;

namespace gui
{

class UserAccount : public QDialog
{
    Q_OBJECT

public:
    UserAccount(QWidget *parent = 0, Qt::WindowFlags f = 0);

private:
    QLineEdit *m_leUser;
    QLineEdit *m_lePassword;
};

} // namespace gui

#endif // __DLGEXPORT_H__
