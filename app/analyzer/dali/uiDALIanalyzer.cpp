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
#include "common/inputhelper.h"
#include "uiDALIanalyzer.h"

#include <QDebug>

#include "uiDALIanalyzerconfig.h"
#include "device/devicemanager.h"
#include "common/configuration.h"
#include "capture/cursormanager.h"

/*!
    Counter used when creating the editable name.
*/
int UiDALIAnalyzer::DALIAnalyzerCounter = 0;

/*!
    Name of this analyzer.
*/
const QString UiDALIAnalyzer::name = "DALI Analyzer";

/*!
    \class UiDALIAnalyzer
    \brief This class is a DALI protocol analyzer.

    \ingroup Analyzer

    The class will analyze specified digital signals and visualize the
    interpretation as DALI protocol data.

*/


/*!
    Constructs the UiDALIAnalyzer with the given \a parent.
*/
UiDALIAnalyzer::UiDALIAnalyzer(QWidget *parent) :
    UiAnalyzer(parent)
{
    mSignalId = -1;
    mBaudRate = 2400;

    //mFormat = Types::DataFormatAscii;
    mSyncCursor = UiCursor::NoCursor;

    mIdLbl->setText("DALI");
    mNameLbl->setText(QString("DALI %1").arg(DALIAnalyzerCounter++));

    // Deallocation: "Qt Object trees" (See UiMainWindow)
    mSignalLbl = new QLabel(this);

    QPalette palette= mSignalLbl->palette();
    palette.setColor(QPalette::Text, Qt::gray);
    mSignalLbl->setPalette(palette);

    setFixedHeight(50);
}

/*!
    Set the signal ID to \a id.
*/
void UiDALIAnalyzer::setSignalId(int signalId)
{
    mSignalId = signalId;
    if (signalId < ANALOG_BASE)
    {
        mSignalLbl->setText(QString("Signal: D%1").arg(signalId));
    }
    else
    {
        mSignalLbl->setText(QString("Signal: A%1").arg(signalId - ANALOG_BASE));
    }
}


/*!
    \fn int UiDALIAnalyzer::signalId() const

    Returns the signal ID.
*/

/*!
    \fn void UiDALIAnalyzer::setBaudRate(int rate)

    Set the baud rate.
*/

/*!
    \fn int UiDALIAnalyzer::baudRate() const

    Returns the baud rate.
*/

/*!
    \fn void UiDALIAnalyzer::setStopBits(int bits)

    Set the number of stop bits.
*/

/*!
    \fn int UiDALIAnalyzer::stopBits() const

    Returns the number of stop bits.
*/

/*!
    \fn void UiDALIAnalyzer::setParity(Types::UARTParity parity)

    Set the parity.
*/

/*!
    \fn Types::UARTParity UiDALIAnalyzer::parity() const

    Returns the parity.
*/

/*!
    \fn void UiDALIAnalyzer::setDataBits(int bits)

    Set the number of data bits.
*/

/*!
    \fn int UiDALIAnalyzer::dataBits() const

    Returns the number of data bits.
*/

/*!
    \fn void UiDALIAnalyzer::setSyncCursor(UiCursor::CursorId id)

    Set the cursor to use for synchronization.
*/

/*!
    \fn UiCursor::CursorId UiDALIAnalyzer::syncCursor() const

    Returns the cursor used for synchronization.
*/


/*!
    Start to analyze the signal data.
*/
void UiDALIAnalyzer::analyze()
{
    QVector<int>*       DigitalDALIData = NULL;
    QVector<double>*    AnalogDALIData  = NULL;
    int TotalSize                       = 0;

    mDALIItems.clear();

    if (mSignalId == -1) return;

    CaptureDevice* device = DeviceManager::instance().activeDevice()->captureDevice();
    int sampleRate = device->usedSampleRate();

    if (mSignalId < ANALOG_BASE)
    {
        DigitalDALIData = device->digitalData(mSignalId);
        if (DigitalDALIData == NULL || DigitalDALIData->size() == 0) return;
        TotalSize = DigitalDALIData->size();
    }
    else
    {
        AnalogDALIData = device->analogData(mSignalId - ANALOG_BASE);
        if (AnalogDALIData == NULL || AnalogDALIData->size() == 0) return;
        TotalSize = AnalogDALIData->size();
    }


    int numSamplesPerBit = sampleRate / mBaudRate;
    // if there aren't enough samples per bit the decoding isn't reliable
    if (numSamplesPerBit < 3) return;

    int FirstPos = 0;
    int startIdx = 0;
    int value = 0;
    int numDataBits = 0;
    int Level = 0;
    int BitLength = 0;
    int Position = 0;
    int FirstBit = 0;

    DALIState state = STATE_IDLE;

    do
    {
        FirstPos = Position;
        BitLength = 1;

        if (mSignalId < ANALOG_BASE)
        {
            Level = DigitalDALIData->at(Position);
        }
        else
        {
            Level = (AnalogDALIData->at(Position) >= DALI_THRESHOLD) ? 1 : 0;
        }

        Position++;

        while (Position < TotalSize)
        {
            int NewLevel;

            if (mSignalId < ANALOG_BASE)
            {
                NewLevel = DigitalDALIData->at(Position);
            }
            else
            {
                NewLevel = (AnalogDALIData->at(Position) >= DALI_THRESHOLD) ? 1 : 0;
            }

            Position++;

            if (Level != NewLevel)
            {
               break;
            }

            BitLength++;
        }

        switch (state)
        {
        case STATE_IDLE:
            {
                if (Level == 1)
                {
                    if (BitLength >= (numSamplesPerBit * 3))
                    {
                        state = STATE_START_FIRST;
                        startIdx = Position;
                    }
                }
            }
            break;

        case STATE_START_FIRST:
            {
                if ((Level == 0) && ((BitLength * 2) > numSamplesPerBit) && ((BitLength * 2) < (numSamplesPerBit * 3)))
                {
                    state = STATE_START_SECOND;
                    value = 0;
                    numDataBits = 0;
                }
                else
                {
                    state = STATE_IDLE;
                }
            }
            break;

        case STATE_START_SECOND:
            {
                if ((Level == 1) && ((BitLength * 2) > numSamplesPerBit) && (BitLength < (numSamplesPerBit * 3)))
                {
                    value = 0;
                    numDataBits = 0;
                    if ((BitLength * 2) < (numSamplesPerBit * 3))
                    {
                        state = STATE_DATA_FIRST;
                    }
                    else
                    {
                        FirstBit = Level;
                        state = STATE_DATA_SECOND;
                    }
                }
                else
                {
                    state = STATE_IDLE;
                }
            }
            break;

        case STATE_DATA_FIRST:
            {
                if ((BitLength * 2) <= numSamplesPerBit)
                {
                    state = STATE_ERROR;                                // Too Short
                    break;
                }

                if (BitLength >= (numSamplesPerBit * 3))
                {
                    state = STATE_STOP;
                    break;
                }

                FirstBit = Level;
                state = STATE_DATA_SECOND;
            }
            break;

        case STATE_DATA_SECOND:
            {
                if (((BitLength * 2) <= numSamplesPerBit) || (FirstBit == Level))
                {
                    state = STATE_ERROR;                                // Too Short
                    break;
                }

                numDataBits++;
                value <<= 1;
                if (Level == 1)
                {
                    value |= 1;
                }

                if (BitLength >= (numSamplesPerBit * 3))
                {
                    state = STATE_STOP;
                    break;
                }

                if ((BitLength * 2) < (numSamplesPerBit * 3))
                {
                    state = STATE_DATA_FIRST;
                }
                else
                {
                    FirstBit = Level;
                    state = STATE_DATA_SECOND;
                }
            }
            break;

        default:
           {
           }
           break;
        }

        if (state == STATE_ERROR)
        {
            DALIItem item(DALIItem::TYPE_FRAME_ERROR, 0, 0, startIdx, Position);
            mDALIItems.append(item);

            state = STATE_IDLE;
        }
        else if (state == STATE_STOP)
        {
            int End = FirstPos + (numSamplesPerBit * 4);
            if (End >= TotalSize)
            {
                End =  TotalSize - 1;
            }

            DALIItem item(DALIItem::TYPE_DATA, value, numDataBits, startIdx, End);
            mDALIItems.append(item);

//          state = STATE_IDLE;
            state = STATE_START_FIRST;
            startIdx = Position;
        }
    }
    while (Position < TotalSize);
}

/*!
    Configure the analyzer.
*/
void UiDALIAnalyzer::configure(QWidget *parent)
{
    UiDALIAnalyzerConfig dialog(parent);
    dialog.setSignalId(mSignalId);
    dialog.exec();

    setSignalId(dialog.signalId());
    setSyncCursor(dialog.syncCursor());

    analyze();
    update();
}

/*!
    Returns a string representation of this analyzer.
*/
QString UiDALIAnalyzer::toSettingsString() const
{
    // type;name;Signal;Format;Baud;DataBits;StopBits;Parity;Sync

    QString str;
    str.append(UiDALIAnalyzer::name);str.append(";");
    str.append(getName());str.append(";");
    str.append(QString("%1;").arg(signalId()));
    return str;
}

/*!
    Create a DALI analyzer from the string representation \a s.

    \sa toSettingsString
*/
UiDALIAnalyzer* UiDALIAnalyzer::fromSettingsString(const QString &s)
{
    UiDALIAnalyzer* analyzer = NULL;
    QString name;

    bool ok = false;

    do
    {
        // type;name;Signal;Format;Baud;DataBits;StopBits;Parity;Sync
        QStringList list = s.split(';');
        if (list.size() != 9) break;

        // --- type
        if (list.at(0) != UiDALIAnalyzer::name) break;

        // --- name
        name = list.at(1);
        if (name.isNull()) break;

        // --- signal ID
        int signalId = list.at(2).toInt(&ok);
        if (!ok) break;

        // --- sync cursor
        int sc = list.at(8).toInt(&ok);
        if (sc < 0 || sc > UiCursor::NumCursors) break;
        UiCursor::CursorId syncCursor = (UiCursor::CursorId)sc;

        // Deallocation: The caller of this function is responsible for
        //               deallocation
        analyzer = new UiDALIAnalyzer();
        if (analyzer == NULL) break;

        analyzer->setSignalName(name);
        analyzer->setSignalId(signalId);
        analyzer->setSyncCursor(syncCursor);

    }
    while (false);

    return analyzer;
}

/*!
    Paint event handler responsible for painting this widget.
*/
void UiDALIAnalyzer::paintEvent(QPaintEvent *event)
{
    (void)event;
    QPainter painter(this);

    int textMargin = 3;

    // -----------------
    // draw background
    // -----------------
    paintBackground(&painter);


    painter.setClipRect(plotX(), 0, width()-infoWidth(), height());
    painter.translate(0, height()/2);

    CaptureDevice* device = DeviceManager::instance().activeDevice()->captureDevice();
    int sampleRate = device->usedSampleRate();


    double from = 0;
    double to = 0;
    int fromIdx = 0;
    int toIdx = 0;

    int h = height()/4;

    QString shortTxt;
    QString longTxt;

    QPen pen = painter.pen();
    pen.setColor(Configuration::instance().analyzerColor());
    painter.setPen(pen);

    for (int i = 0; i < mDALIItems.size(); i++)
    {
        DALIItem item = mDALIItems.at(i);

        fromIdx = item.startIdx;
        toIdx = item.stopIdx;


        typeAndValueAsString(item.type, item.value, item.numDataBits, shortTxt, longTxt);

        int shortTextWidth = painter.fontMetrics().width(shortTxt);
        int longTextWidth = painter.fontMetrics().width(longTxt);


        from = mTimeAxis->timeToPixelRelativeRef((double)fromIdx/sampleRate);

        // no need to draw when signal is out of plot area
        if (from > width()) break;

        if (toIdx != -1)
        {
            to = mTimeAxis->timeToPixelRelativeRef((double)toIdx/sampleRate);
        }
        else
        {
            // see if the long text version fits
            to = from + longTextWidth+textMargin*2;

            if (i+1 < mDALIItems.size())
            {
                // get position for the start of the next item
                double tmp = mTimeAxis->timeToPixelRelativeRef(
                            (double)mDALIItems.at(i+1).startIdx/sampleRate);


                // if 'to' overlaps check if short text fits
                if (to > tmp)
                {
                    to = from + shortTextWidth+textMargin*2;

                    // 'to' overlaps next item -> limit to start of next item
                    if (to > tmp)
                    {
                        to = tmp;
                    }
                }
            }
        }

        if (to-from > 4)
        {
            painter.drawLine(from, 0, from+2, -h);
            painter.drawLine(from, 0, from+2, h);

            painter.drawLine(from+2, -h, to-2, -h);
            painter.drawLine(from+2, h, to-2, h);

            painter.drawLine(to, 0, to-2, -h);
            painter.drawLine(to, 0, to-2, h);
        }
        else
        {
            painter.drawLine(from, -h, from, h);       // drawing a vertical line when the allowed width is too small
        }

        // only draw the text if it fits between 'from' and 'to'
        QRectF textRect(from + 1, -h, (to - from), 2 * h);
        if (longTextWidth < (to - from))
        {
            painter.drawText(textRect, Qt::AlignCenter, longTxt);
        }
        else if (shortTextWidth < (to-from))
        {
            painter.drawText(textRect, Qt::AlignCenter, shortTxt);
        }
    }
}

/*!
    Event handler called when this widget is being shown
*/
void UiDALIAnalyzer::showEvent(QShowEvent* event)
{
    (void) event;
    doLayout();
    setMinimumInfoWidth(calcMinimumWidth());
}

/*!
    Called when the info width has changed for this widget.
*/
void UiDALIAnalyzer::infoWidthChanged()
{
    doLayout();
}

/*!
    Position the child widgets.
*/
void UiDALIAnalyzer::doLayout()
{
    UiSimpleAbstractSignal::doLayout();

    QRect r = infoContentRect();
    int y = r.top();

    mIdLbl->move(r.left(), y);

    int x = mIdLbl->pos().x()+mIdLbl->width() + SignalIdMarginRight;
    mNameLbl->move(x, y);
    mEditName->move(x, y);

    mSignalLbl->move(r.left(), r.bottom()-mSignalLbl->height());

}

/*!
    Calculate and return the minimum width for this widget.
*/
int UiDALIAnalyzer::calcMinimumWidth()
{
    int w = mNameLbl->pos().x() + mNameLbl->minimumSizeHint().width();
    if (mEditName->isVisible())
    {
        w = mEditName->pos().x() + mEditName->width();
    }

    int w2 = mSignalLbl->pos().x()+mSignalLbl->width();
    if (w2 > w) w = w2;

    return w+infoContentMargin().right();
}

/*!
    Convert DALI \a type and data \a value to string representation. A short
    and long representation is returned in \a shortTxt and \a longTxt.
*/
void UiDALIAnalyzer::typeAndValueAsString(DALIItem::ItemType type,
                                          int value,
                                          int numDataBits,
                                          QString &shortTxt,
                                          QString &longTxt)
{
    switch (type)
    {
    case DALIItem::TYPE_DATA:
        {
            int Len = 2;

            if (numDataBits > 8)
            {
                if (numDataBits <= 16)
                {
                   Len = 4;
                }
                else if (numDataBits <= 24)
                {
                    Len = 6;
                }
                else
                {
                    Len = 8;
                }
            }

            QLatin1Char fillChar('0');
            shortTxt    = QString("%1").arg(value, Len, 16, fillChar).toUpper();
            longTxt     = QString("%1").arg(value, Len, 16, fillChar).toUpper();
        }
        break;

    case DALIItem::TYPE_PARITY_ERROR:
        shortTxt = "PE";
        longTxt = "Parity Error";
        break;
    case DALIItem::TYPE_FRAME_ERROR:
        shortTxt = "FE";
        longTxt = "Frame Error";
        break;
    }
}
