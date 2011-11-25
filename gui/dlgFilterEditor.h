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

#ifndef __DLGFILTEREDITOR_H__
#define __DLGFILTEREDITOR_H__

#include <QDialog>
#include "../parser/filter.h"
#include "../parser/post.h"

class QLineEdit;
class QRadioButton;
class QTreeWidget;
class QComboBox;

namespace gui
{

class FilterEditor : public QDialog
{
    Q_OBJECT

public:
    FilterEditor(QWidget *parent = 0, Qt::WindowFlags f = 0);

    void setFilter(const core::Filter<core::Post> &filter);

    core::Filter<core::Post> filter() const;

private slots:
    void onAddRule();

    void onRemoveRule();

private:
    /// The rules tree column numbers.
    enum RulesTreeColumns
    {
        Property = 0,
        Option,
        Value,
        AddRemove
    };

    QComboBox *propertiesCombo(const core::Filter<core::Post> &filter,
                               const QString &currentText);

    QComboBox *optionsCombo(const core::Filter<core::Post> &filter,
                            int currentOption);

    QWidget *addRemoveButton();

    void addTreeNode(const core::Filter<core::Post> &filter,
                     const QString &currentProperty = QString(),
                     const QString &value = QString(),
                     int currentOption = -1);


    QLineEdit *m_editName;
    QRadioButton *m_radAll;
    QRadioButton *m_radOne;
    QTreeWidget *m_rulesTree;
    QComboBox *m_tableCombo;
};

} // namespace gui

#endif // __DLGFILTEREDITOR_H__
