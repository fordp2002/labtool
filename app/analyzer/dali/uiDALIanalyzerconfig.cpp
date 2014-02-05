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
#include "uiDALIanalyzerconfig.h"

#include <QFormLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>

#include "common/inputhelper.h"

/*!
    \class UiDALIAnalyzerConfig
    \brief Dialog window used to configure the DALI analyzer.

    \ingroup Analyzer

*/


/*!
    Constructs the UiDALIAnalyzerConfig with the given \a parent.
*/
UiDALIAnalyzerConfig::UiDALIAnalyzerConfig(QWidget *parent) :
    UiAnalyzerConfig(parent)
{
    setWindowTitle(tr("DALI Analyzer"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // Deallocation: Re-parented when calling verticalLayout->addLayout
    QFormLayout* formLayout = new QFormLayout;

    mSignalBox = InputHelper::createSignalBox(this, 0);
    formLayout->addRow(tr("DALI Signal: "), mSignalBox);

    mFormatBox = InputHelper::createFormatBox(this, Types::DataFormatHex);
    formLayout->addRow(tr("Data format: "), mFormatBox);

//    mBaudRate = InputHelper::createDALIBaudRateBox(this, 115200);
//    formLayout->addRow(tr("Baud Rate: "), mBaudRate);

//    mDataBitsBox = InputHelper::createDALIDataBitsBox(this, 8);
//    formLayout->addRow(tr("Data bits: "), mDataBitsBox);

    mCursorBox = InputHelper::createActiveCursorsBox(this, UiCursor::NoCursor);
    // Deallocation: "Qt Object trees" (See UiMainWindow)
    QLabel* cursorLbl = new QLabel(tr("Synchronize: "), this);
    cursorLbl->setToolTip(tr("Start to analyze from a cursor position"));
    formLayout->addRow(cursorLbl, mCursorBox);


    // Deallocation: Ownership changed when calling setLayout
    QVBoxLayout* verticalLayout = new QVBoxLayout();

    // Deallocation: "Qt Object trees" (See UiMainWindow)
    QDialogButtonBox* bottonBox = new QDialogButtonBox(
                QDialogButtonBox::Ok,
                Qt::Horizontal,
                this);
    bottonBox->setCenterButtons(true);

    connect(bottonBox, SIGNAL(accepted()), this, SLOT(accept()));

    verticalLayout->addLayout(formLayout);
    verticalLayout->addWidget(bottonBox);


    setLayout(verticalLayout);
}

/*!
    Returns the selected signal ID.
*/
int UiDALIAnalyzerConfig::signalId()
{
    return InputHelper::intValue(mSignalBox);
}

/*!
    Set the signal ID to \a id.
*/
void UiDALIAnalyzerConfig::setSignalId(int id)
{
    InputHelper::setInt(mSignalBox, id);
}

/*!
    Set the data format to \a format.
*/
void UiDALIAnalyzerConfig::setDataFormat(Types::DataFormat format)
{
    InputHelper::setInt(mFormatBox, (int)format);
}

/*!
    Returns the data format.
*/
Types::DataFormat UiDALIAnalyzerConfig::dataFormat()
{
    int f = InputHelper::intValue(mFormatBox);
    return (Types::DataFormat)f;
}

/*!
    Set the baud rate to \a rate.
*/
void UiDALIAnalyzerConfig::setBaudRate(int rate)
{
    mBaudRate->setText(QString("%1").arg(rate));
}

/*!
    Returns the selected baud rate.
*/
int UiDALIAnalyzerConfig::baudRate()
{
    return mBaudRate->text().toInt();
}

/*!
    Set number of stop bits to \a bits.
*/
void UiDALIAnalyzerConfig::setStopBits(int bits)
{
    InputHelper::setInt(mStopBitsBox, bits);
}

/*!
    Returns selected number of stop bits.
*/
int UiDALIAnalyzerConfig::stopBits()
{
    return InputHelper::intValue(mStopBitsBox);
}

/*!
    Set number of data bits to \a bits.
*/
void UiDALIAnalyzerConfig::setDataBits(int bits)
{
    InputHelper::setInt(mDataBitsBox, bits);
}

/*!
    Returns selected number of data bits.
*/
int UiDALIAnalyzerConfig::dataBits()
{
    return InputHelper::intValue(mDataBitsBox);
}

/*!
    Returns the cursor used for synchronization.
*/
UiCursor::CursorId UiDALIAnalyzerConfig::syncCursor()
{
    return (UiCursor::CursorId)InputHelper::intValue(mCursorBox);
}

/*!
    Sets the cursor used for synchronization.
*/
void UiDALIAnalyzerConfig::setSyncCursor(UiCursor::CursorId id)
{
    InputHelper::setInt(mCursorBox, id);
}





