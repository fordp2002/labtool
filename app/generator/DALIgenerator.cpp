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
    mBaudRate = 2400;
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

void DALIGenerator::addIdle(quint32 NumDataBits)
{
    for (quint32 i = 0; i < NumDataBits; i++)
    {
        mDALIData.append(1);
    }
}

/*!
    Add data.
*/
void DALIGenerator::addData(quint32 NumDataBits, quint32 Data)
{
    mDALIData.append(0);                                            // Start Bit

    if (NumDataBits && (NumDataBits <= 32))
    {
        Data <<= (32 - NumDataBits);                                // Shift bits to the top of the word

        // LSB first (do we also need to support MSB first)
        for (quint32 i = 0; i < NumDataBits; i++)
        {
            if (Data & 0x80000000ULL)
            {
                mDALIData.append(1);
                mDALIData.append(0);
            }
            else
            {
                mDALIData.append(0);
                mDALIData.append(1);
            }

            Data <<= 1;
        }
    }

    mDALIData.append(1);                                    // Idle Bits
    mDALIData.append(1);
}
