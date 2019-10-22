/***************************************************************************
**                                                                        **
**  Polyphone, a soundfont editor                                         **
**  Copyright (C) 2013-2019 Davy Triponney                                **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program. If not, see http://www.gnu.org/licenses/.    **
**                                                                        **
****************************************************************************
**           Author: Davy Triponney                                       **
**  Website/Contact: https://www.polyphone-soundfonts.com                 **
**             Date: 01.01.2013                                           **
***************************************************************************/

#include "inputparsergrandorgue.h"
#include "soundfontmanager.h"
#include "grandorguerank.h"
#include "grandorguestop.h"

InputParserGrandOrgue::InputParserGrandOrgue() : AbstractInputParser() {}

InputParserGrandOrgue::~InputParserGrandOrgue()
{
    while (!_ranks.empty())
        delete _ranks.take(_ranks.firstKey());
    while (!_stops.empty())
        delete _stops.take(_stops.firstKey());
}

void InputParserGrandOrgue::processInternal(QString fileName, SoundfontManager * sm, bool &success, QString &error, int &sf2Index, QString &tempFilePath)
{
    Q_UNUSED(sm)
    Q_UNUSED(tempFilePath)

    // Initialization
    _currentSection = SECTION_UNKNOWN;
    _currentIndex = -1;
    _ranks.clear();
    _stops.clear();

    // Parse the file
    _rootDir = QFileInfo(fileName).dir().path();
    if (_rootDir.endsWith('/'))
        _rootDir = _rootDir.left(_rootDir.size() - 1);
    parseFile(fileName, success, error);
    if (!success)
        return;

    // Create a sf2
    createSf2(sf2Index, fileName);
    success = true;
}

void InputParserGrandOrgue::parseFile(QString filename, bool &success, QString &error)
{
    // Open the file
    QFile inputFile(filename);
    if (!inputFile.open(QIODevice::ReadOnly))
    {
        success = false;
        error = trUtf8("Cannot open file \"%1\"").arg(filename);
        return;
    }

    QTextStream in(&inputFile);
    while (!in.atEnd())
    {
        // Read one line, skip empty lines or comments
        QString line = in.readLine();

        // Remove comments
        int tmp;
        if ((tmp = line.indexOf(';')) != -1)
        {
            line = line.left(tmp);
        }

        // Trim the line and skip it if empty
        line = line.trimmed();
        if (line.isEmpty())
            continue;

        if (line.startsWith("[") && line.endsWith("]"))
        {
            // New section
            startSection(line.mid(1, line.length() - 2).toLower());
        }
        else
        {
            // Process data
            tmp = line.indexOf('=');
            QString key = line.left(tmp).trimmed().toLower();
            QString value = line.right(line.length() - tmp - 1).trimmed().replace('\\', '/');
            if (!key.isEmpty() && !value.isEmpty())
                processData(key, value);
        }
    }
    inputFile.close();

    success = true;
}

void InputParserGrandOrgue::startSection(QString sectionName)
{
    // Track ranks and stops
    if (sectionName.startsWith("rank"))
    {
        bool ok = false;
        _currentIndex = sectionName.right(sectionName.length() - 4).toInt(&ok);
        if (!ok)
        {
            _currentSection = SECTION_UNKNOWN;
            _currentIndex = -1;
        }
        else
            _currentSection = SECTION_RANK;
    }
    else if (sectionName.startsWith("stop"))
    {
        bool ok = false;
        _currentIndex = sectionName.right(sectionName.length() - 4).toInt(&ok);
        if (!ok)
        {
            _currentSection = SECTION_UNKNOWN;
            _currentIndex = -1;
        }
        else
            _currentSection = SECTION_STOP;
    }
    else
        _currentSection = SECTION_UNKNOWN;
}

void InputParserGrandOrgue::processData(QString key, QString value)
{
    switch (_currentSection)
    {
    case SECTION_RANK:
        if (_currentIndex != -1)
        {
            if (!_ranks.contains(_currentIndex))
                _ranks[_currentIndex] = new GrandOrgueRank(_rootDir);
            _ranks[_currentIndex]->processData(key, value);
        }
        break;
    case SECTION_STOP:
        if (_currentIndex != -1)
        {
            if (!_stops.contains(_currentIndex))
                _stops[_currentIndex] = new GrandOrgueStop(_rootDir);
            _stops[_currentIndex]->processData(key, value);
        }
        break;
    case SECTION_UNKNOWN:
        // Nothing
        break;
    }
}

void InputParserGrandOrgue::createSf2(int &sf2Index, QString filename)
{
    // Create a new soundfont, store the sf2 index
    SoundfontManager * sm = SoundfontManager::getInstance();
    EltID idSf2(elementSf2, -1, -1, -1, -1);
    idSf2.indexSf2 = sm->add(idSf2);
    sf2Index = idSf2.indexSf2;

    // Title
    QFileInfo fileInfo(filename);
    sm->set(idSf2, champ_name, fileInfo.completeBaseName());

    // Comment
    sm->set(idSf2, champ_ICMT, QString("Sf2 imported from a GrandOrgue sample set by Polyphone"));

    // Ranks are instruments
    foreach (GrandOrgueRank * rank, _ranks)
        rank->createInstrument(sm, idSf2);

    // Stops are presets
    foreach (GrandOrgueStop * stop, _stops)
        stop->createPreset(sm, idSf2, &_ranks);
}