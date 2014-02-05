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
#include "DALIgenerator.h"

/*!
    \class DALIGenerator
    \brief This is a helper class that can generate valid digital data
    for the DALI protocol.

    \ingroup Generator

*/

/*!
    Constructs the DALIGenerator with the given \a parent.
*/
DALIGenerator::DALIGenerator(QObject *parent) :
    QObject(parent)
{
    mBaudRate = 4800;
    mNumDataBits = 8;
    mNumStopBits = 1;
}

/*!
    Sets baud rate to \a rate.
*/
void DALIGenerator::setBaudRate(int rate)
{
    mBaudRate = rate;
}

/*!
    Sets number of data bits to \a numBits.
*/
void DALIGenerator::setDataBits(int numBits)
{
    mNumDataBits = numBits;
}

/*!
    Sets number of stop bits to \a numBits.
*/
void DALIGenerator::setStopBits(int numBits)
{
    mNumStopBits = numBits;
}

/*!
    Generate DALI signal using specified \a data.
*/
bool DALIGenerator::generate(QByteArray &data)
{
    mDALIData.clear();

    // idle line -> high
    mDALIData.append(1);

    for (int i = 0; i < data.size(); i++)
    {
        // start bit
        mDALIData.append(0);

        // data
        addData(data.at(i));

        // stop bit(s)
        for (int j = 0; j < mNumStopBits; j++)
        {
            mDALIData.append(1);
        }

    }

    // idle line -> high
    mDALIData.append(1);

    return true;
}

/*!
    Returns DALI signal data.
*/
QVector<int> DALIGenerator::DALIData()
{
    return mDALIData;
}

/*!
    Returns sample rate.
*/
int DALIGenerator::sampleRate()
{
    // same as baud rate
    return mBaudRate;
}

/*!
    Add data.
*/
int DALIGenerator::addData(char data)
{
    int numOnes = 0;
    // LSB first (do we also need to support MSB first)
    for (int i = 0; i < mNumDataBits; i++)
    {
        if ((data & (1<<i)) != 0)
        {
            mDALIData.append(1);
            numOnes++;
        }
        else
        {
            mDALIData.append(0);
        }
    }

    return numOnes;
}
