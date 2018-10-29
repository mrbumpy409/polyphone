/***************************************************************************
**                                                                        **
**  Polyphone, a soundfont editor                                         **
**  Copyright (C) 2013-2017 Davy Triponney                                **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Davy Triponney                                       **
**  Website/Contact: http://polyphone-soundfonts.com                      **
**             Date: 01.01.2013                                           **
***************************************************************************/

#ifndef PAGE_INST_H
#define PAGE_INST_H

#include <QWidget>
#include "pagetable.h"

namespace Ui {
class PageInst;
}

class PageInst : public PageTable
{
    Q_OBJECT

public:
    explicit PageInst(QWidget *parent = 0);
    ~PageInst();
    void setModVisible(bool visible);
    void release();

    // Display options
    QList<DisplayOption> getDisplayOptions(IdList selectedIds) override;

protected:
    bool updateInterface(QString editingSource, IdList selectedIds, int displayOption) override;
    void keyPlayedInternal2(int key, int velocity) override;
    int getDestIndex(AttributeType type) override;
    AttributeType getDestNumber(int row) override;

private slots:
    void release(double duree36, double division, double deTune);
    void release(EltID id, double duree36, double division, double deTune);
    void onLinkClicked(EltID id);

private:
    Ui::PageInst *ui;
    QList<AttributeType> _destIndex;
};

// Classe TableWidget pour instruments
class TableWidgetInst : public TableWidget
{
    Q_OBJECT

public:
    TableWidgetInst(QWidget *parent = 0);
    ~TableWidgetInst();

    // Association champ - ligne
    AttributeType getChamp(int row);
    int getRow(quint16 champ);
};

#endif // PAGE_INST_H
