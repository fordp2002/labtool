/*
 *  Copyright 2013 Embedded Artists AB
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#ifndef UIDALIANALYZERCONFIG_H
#define UIDALIANALYZERCONFIG_H

#include <QWidget>

#include "uiDALIanalyzer.h"
#include "analyzer/uianalyzerconfig.h"
#include "capture/uicursor.h"


class UiDALIAnalyzerConfig : public UiAnalyzerConfig
{
    Q_OBJECT
public:
    explicit UiDALIAnalyzerConfig(QWidget *parent = 0);
    int signalId();
    void setSignalId(int id);

    Types::DataFormat dataFormat();
    void setDataFormat(Types::DataFormat format);

    void setBaudRate(int rate);
    int baudRate();

    void setStopBits(int bits);
    int stopBits();

    void setDataBits(int bits);
    int dataBits();

    UiCursor::CursorId syncCursor();
    void setSyncCursor(UiCursor::CursorId id);

signals:
    
public slots:

private:

    QComboBox* mSignalBox;
    QComboBox* mCursorBox;
};

#endif // UIDALIANALYZERCONFIG_H
