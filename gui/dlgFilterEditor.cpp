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

#include <QLineEdit>
#include <QGroupBox>
#include <QLayout>
#include <QRadioButton>
#include <QLabel>
#include <QTreeWidget>
#include <QPushButton>
#include <QComboBox>
#include <QGridLayout>
#include "dlgFilterEditor.h"
#include "../strings/guiStrings.h"
#include "../core/application.h"
#include "../database/database.h"

namespace gui
{

FilterEditor::FilterEditor(QWidget *parent, Qt::WindowFlags f)
    :
        QDialog(parent, f)
{
    QLabel *lblName = new QLabel("Name:", this);
    m_editName = new QLineEdit(this);
    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->addWidget(lblName);
    nameLayout->addWidget(m_editName);

    QGroupBox *groupBox = new QGroupBox("Matching Criterion", this);
    m_radAll = new QRadioButton("&All of below matched");
    m_radOne = new QRadioButton("Any &of below matched");
    m_radAll->setChecked(true);

    QHBoxLayout *radioLayout = new QHBoxLayout;
    radioLayout->addWidget(m_radAll);
    radioLayout->addWidget(m_radOne);
    radioLayout->addStretch(1);
    groupBox->setLayout(radioLayout);

    m_rulesTree = new QTreeWidget(this);
    m_rulesTree->setColumnCount(4);
    m_rulesTree->setRootIsDecorated(false);
    QStringList headerLabels;
    headerLabels << "Property" << "Option" << "Value" << "";
    m_rulesTree->setHeaderLabels(headerLabels);

    QPushButton *btnAdd = new QPushButton("New", this);
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(onAddRule()));
    QPushButton *btnRemove = new QPushButton("Remove", this);
    connect(btnRemove, SIGNAL(clicked()), this, SLOT(onRemoveRule()));

    QVBoxLayout *addRemoveLayout = new QVBoxLayout;
    addRemoveLayout->addWidget(btnAdd);
    addRemoveLayout->addWidget(btnRemove);
    addRemoveLayout->addStretch();

    m_tableCombo = new QComboBox(this);

    QPushButton *btnOk = new QPushButton(str::Ok, this);
    connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
    btnOk->setDefault(true);
    QPushButton *btnCancel = new QPushButton(str::Cancel, this);
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(btnOk);
    btnLayout->addWidget(btnCancel);

    QGridLayout *grid = new QGridLayout;
    grid->addLayout(nameLayout, 0, 0);
    grid->addWidget(groupBox, 1, 0);
    grid->addWidget(m_rulesTree, 2, 0);
    grid->addLayout(addRemoveLayout, 2, 1);
    grid->addWidget(m_tableCombo, 3, 0);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(grid);
    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);
}

void FilterEditor::setFilter(const core::Filter<core::Post> &filter)
{
    if (filter.ruleMatch() == core::Filter<core::Post>::All)
        m_radAll->setChecked(true);
    else if (filter.ruleMatch() == core::Filter<core::Post>::One)
        m_radOne->setChecked(true);
    m_editName->setText(filter.name());

    // Set the target folders (tables) combo box and select
    // the target folder name for the given filter.
    core::Database *db = core::Application::theApp()->database();
    QStringList tables = db->tables();
    m_tableCombo->addItems(tables);
    int tblIndex = tables.indexOf(filter.table());
    m_tableCombo->setCurrentIndex(tblIndex);

    const core::Filter<core::Post>::Rules &rules = filter.rules();
    core::Filter<core::Post>::Rules::const_iterator it = rules.constBegin();
    while (it != rules.constEnd()) {
        const core::Filter<core::Post>::Rule &rule = it.value();
        const QString name = it.key();
        QTreeWidgetItem *node = new QTreeWidgetItem;
        node->setFlags(Qt::ItemIsSelectable |
                       Qt::ItemIsUserCheckable |
                       Qt::ItemIsEnabled |
                       Qt::ItemIsEditable);
        node->setText(Value, rule.value());

        m_rulesTree->addTopLevelItem(node);
        QComboBox *combo = propertiesCombo(filter, name);
        m_rulesTree->setItemWidget(node, Property, combo);

        QComboBox *comboOpt = optionsCombo(filter, rule.option());
        m_rulesTree->setItemWidget(node, Option, comboOpt);

        QWidget *addRemoveBtn = addRemoveButton();
        m_rulesTree->setItemWidget(node, AddRemove, addRemoveBtn);

        ++it;
    }
}

QComboBox *FilterEditor::propertiesCombo(const core::Filter<core::Post> &filter,
                                         const QString &currentText)
{
    QComboBox *combo = new QComboBox(this);
    combo->setEditable(false);

    QStringList propNames = filter.propertyNames();
    foreach(const QString &name, propNames) {
        combo->addItem(name, name);
    }

    int index = propNames.indexOf(currentText);
    combo->setCurrentIndex(index);
    return combo;
}

QComboBox *FilterEditor::optionsCombo(const core::Filter<core::Post> &filter,
                                      int currentOption)
{
    QComboBox *combo = new QComboBox(this);
    combo->setEditable(false);

    combo->addItem("Ignore");
    combo->addItem("Match exactly");
    combo->addItem("Contains");
    combo->setCurrentIndex(currentOption);
    return combo;
}

QWidget *FilterEditor::addRemoveButton()
{
    QPushButton *btnAdd = new QPushButton("+", this);
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(onAddRule()));
    QPushButton *btnRemove = new QPushButton("-", this);
    connect(btnRemove, SIGNAL(clicked()), this, SLOT(onRemoveRule()));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(btnAdd);
    layout->addWidget(btnRemove);
    layout->addStretch();

    QWidget *widget = new QWidget(this);
    widget->setLayout(layout);

    return widget;
}

core::Filter<core::Post> FilterEditor::filter() const
{
    core::Filter<core::Post> filter(m_editName->text());
    filter.setTable(m_tableCombo->currentText());

    if (m_radAll->isChecked())
        filter.setRuleMatch(core::Filter<core::Post>::All);
    else if (m_radOne->isChecked())
        filter.setRuleMatch(core::Filter<core::Post>::One);
    for (int i = 0; i < m_rulesTree->topLevelItemCount(); ++i) {
        QTreeWidgetItem *node = m_rulesTree->topLevelItem(i);
        QComboBox *combo =
            qobject_cast<QComboBox *>(m_rulesTree->itemWidget(node, Property));
        QComboBox *comboOpt =
            qobject_cast<QComboBox *>(m_rulesTree->itemWidget(node, Option));
        filter.setRule(combo->itemData(combo->currentIndex()).toString(),
                       node->text(Value),
                       (core::Filter<core::Post>::Option)comboOpt->currentIndex());
    }

    return filter;
}

} // namespace gui
