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

#ifndef __BLOGEVENTVIEW_H__
#define __BLOGEVENTVIEW_H__

#include <QWidget>

class QLineEdit;

namespace gui
{

class HtmlEditor;

class BlogEventView : public QWidget
{
    Q_OBJECT

public:
    BlogEventView(QWidget *parent = 0, Qt::WindowFlags f = 0);

    void setHtmlContent(const QString &content);
    void setSubject(const QString &subject);

public slots:
    void setParagraph();
    void setHeading1();
    void setHeading2();
    void setHeading3();
    void setHeading4();
    void setHeading5();
    void setHeading6();
    void setPreformatted();
    void setAddress();
    void setAlignLeft();
    void setAlignCenter();
    void setAlignRight();
    void setAlignJustify();
    void setIncreaseIndent();
    void setDecreaseIndent();
    void setNumberedList();
    void setBulletedList();
    void insertImage();
    void createLink();

private:
    QLineEdit *m_editSubject;
    HtmlEditor *m_htmlEditor;
};

} // namespace gui

#endif // __BLOGEVENTVIEW_H__
