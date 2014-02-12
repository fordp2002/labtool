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
#ifndef SIMULATORCAPTUREDEVICE_H
#define SIMULATORCAPTUREDEVICE_H

#include <QObject>
#include <QVector>
#include "device/capturedevice.h"
#include "uisimulatorconfigdialog.h"

class SimulatorCaptureDevice : public CaptureDevice
{
    Q_OBJECT
public:
    explicit SimulatorCaptureDevice(QObject *parent = 0);
    ~SimulatorCaptureDevice();
    
    QList<int> supportedSampleRates();
    int maxNumDigitalSignals();
    int maxNumAnalogSignals();
    QList<double> supportedVPerDiv(double ProbeMult);

    void configureBeforeStart(QWidget* parent);
    void start(int sampleRate);
    void stop();

    int lastSampleIndex();
    QVector<int>* digitalData(int signalId);
    void setDigitalData(int signalId, QVector<int> data);

    QVector<double>* analogData(int signalId);
    void setAnalogData(int signalId, QVector<double> data);

    void clearSignalData();

    int digitalTriggerIndex();
    void setDigitalTriggerIndex(int idx);
    void digitalTransitions(int signalId, QList<int> &list);

    void reconfigure(int sampleRate = -1);

signals:
    
public slots:

private:

    enum Constants {
        MaxDigitalSignals = 8,
        MaxAnalogSignals = 2
    };


    UiSimulatorConfigDialog* mConfigDialog;

    int mEndSampleIdx;
    QVector<int>* mDigitalSignals[MaxDigitalSignals];
    QVector<double>* mAnalogSignals[MaxAnalogSignals];
    QList<int>* mDigitalSignalTransitions[MaxDigitalSignals];

    QList<double> mSupportedVPerDiv;

    int mTriggerIdx;

    QVector<int>*       AddDigitalData  (double WaveSteps, QVector<int>& WaveData);
    QVector<double>*    AddAnalogData   (double WaveSteps, QVector<int>& WaveData, double HighValue, double LowValue, double ProbeMult);

    int numberOfSamples();
    void generateRandomDigitalSignals();
    void generateI2CDigitalSignals();
    void generateUartDigitalSignals();
    void generateSpiDigitalSignals();
    void generateDALIDigitalSignals();

    void generateDALIAnalogSignals();
    void generateRandomAnalogSignals();
    void generateSineAnalogSignals();
    void deleteSignalData();
    void setDigitalSignalData(int id, QVector<int>* data);
    
};

#endif // SIMULATORCAPTUREDEVICE_H
