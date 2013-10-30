/***************************************************************************
**                                                                        **
**  Polyphone, a soundfont editor                                         **
**  Copyright (C) 2013 Davy Triponney                                     **
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
**  Website/Contact: http://www.polyphone.fr/                             **
**             Date: 01.01.2013                                           **
***************************************************************************/

#ifndef TABLEKEY_H
#define TABLEKEY_H

#include <QTableWidget>
#include <QSignalMapper>

class KeyMapper;

class TableKey : public QTableWidget
{
    Q_OBJECT

public:
    TableKey(QWidget * parent = NULL);
    void setMapper(KeyMapper * mapper);
    void setOctave(int octave);

signals:
    void combinaisonChanged(int numKey, QString combinaison);

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void rowChanged(int row);

private:
    KeyMapper * _mapper;
    int _rootKey;
    QString getName(int note);
    QSignalMapper * _signalMapper;
};


#endif // TABLEKEY_H
