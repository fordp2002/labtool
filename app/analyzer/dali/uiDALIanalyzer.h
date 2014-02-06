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
#ifndef UIDALIANALYZER_H
#define UIDALIANALYZER_H

#include <QWidget>

#include "analyzer/uianalyzer.h"
#include "capture/uicursor.h"
#include "common/types.h"

/*!
    \class DALIItem
    \brief Container class for DALI items.

    \ingroup Analyzer

    \internal

*/
class DALIItem {
public:

    /*!
        DALI item type
    */
    enum ItemType {
        TYPE_DATA,
        TYPE_FRAME_ERROR,
        TYPE_PARITY_ERROR
    };

    // default constructor needed in order to add this to QVector
    /*! Default constructor */
    DALIItem() {
    }

    /*! Constructs a new container */
    DALIItem(ItemType type, int value, int startIdx, int stopIdx) {
        this->type = type;
        this->value = value;
        this->startIdx = startIdx;
        this->stopIdx = stopIdx;
    }

    /*! type */
    ItemType type;
    /*! value */
    int value;
    /*! item start index */
    int startIdx;
    /*! item stop index */
    int stopIdx;    

};

class UiDALIAnalyzer : public UiAnalyzer
{
    Q_OBJECT
public:
    static const QString name;


    explicit UiDALIAnalyzer(QWidget *parent = 0);

    void setSignalId(int signalId);
    int signalId() const {return mSignalId;}

    void setBaudRate(int rate) {if (rate > 0) mBaudRate = rate;}
    int baudRate() const {return mBaudRate;}

    void setSyncCursor(UiCursor::CursorId id) {mSyncCursor = id;}
    UiCursor::CursorId syncCursor() const {return mSyncCursor;}

    void analyze();
    void configure(QWidget* parent);

    QString toSettingsString() const;
    static UiDALIAnalyzer* fromSettingsString(const QString &settings);
    
signals:
    
public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent* event);

private:

    enum {
        SignalIdMarginRight = 10
    };

    enum DALIState
    {
        STATE_IDLE,
        STATE_START_FIRST,
        STATE_START_SECOND,
        STATE_DATA_FIRST,
        STATE_DATA_SECOND,
        STATE_STOP,
        STATE_ERROR
    };

    static int DALIAnalyzerCounter;
    int mSignalId;
    int mBaudRate;

    UiCursor::CursorId mSyncCursor;

    QLabel* mSignalLbl;

    QVector<DALIItem> mDALIItems;

    void infoWidthChanged();
    void doLayout();
    int calcMinimumWidth();

    void typeAndValueAsString(DALIItem::ItemType type,
                                 int value,
                                 QString &shortTxt,
                                 QString &longTxt);
    
};

#endif // UIDALIANALYZER_H
