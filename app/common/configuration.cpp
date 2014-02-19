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
#include "configuration.h"

#define COLOR_SCHEME_LIGHT "Light"
#define COLOR_SCHEME_DARK  "Dark"

#define BEHAVE_SCROLL      "Scrolling Grid/Ruler"
#define BEHAVE_FIXED       "Fixed Grid/Ruler"

const QString Configuration::ProjectFilename   = "Default.prj";
const QString Configuration::ProjectFileExt    = ".prj";
const QString Configuration::ProjectBinFileExt = ".eab";

/*!
    \class Configuration
    \brief This class keeps configuration settings common for all parts of
    the application.

    \ingroup Common

*/

/*!
    \fn Configuration& Configuration::instance()

    Returns an instance of the Configuration class.
*/

/*!
    \var const QString Configuration::ProjectFilename

    The default project file name.
*/

/*!
    \var const QString Configuration::ProjectFileExt

    The file extension used for project files.
*/

/*!
    \var const QString Configuration::ProjectBinFileExt

    The file extension used for binary signal data.
*/



/*!
    Constructs the Configuration.
*/
Configuration::Configuration()
{
    loadColorScheme(COLOR_SCHEME_LIGHT);
    loadBehavior(BEHAVE_SCROLL);
}

/*!
    Returns a list of the color schemes defined for this applicaiton.
*/
QList<QString> Configuration::colorSchemes()
{
    return QList<QString>()
            << COLOR_SCHEME_LIGHT
            << COLOR_SCHEME_DARK;
}

/*!
    Returns a list of the color schemes defined for this applicaiton.
*/
QList<QString> Configuration::Behaviours()
{
    return QList<QString>()
            << BEHAVE_FIXED
            << BEHAVE_SCROLL;
}


/*!
    Returns the active color scheme.
*/
QString Configuration::activeColorScheme()
{
    return mActiveColorScheme;
}

/*!
    Load the color scheme with name \a scheme.
*/
void Configuration::loadColorScheme(QString scheme)
{
    if (scheme == COLOR_SCHEME_LIGHT) {
        loadLightScheme();
    }
    else if (scheme == COLOR_SCHEME_DARK) {
        loadDarkScheme();
    }
}

/*!
    Returns the active color scheme.
*/
QString Configuration::activeBehavior()
{
    return mActiveBehavior;
}

/*!
    Load the Behavior with name \a.
*/
void Configuration::loadBehavior(QString behave)
{
    mActiveBehavior = behave;
}

/*!
    Returns the background color for the signal plot.
*/
QColor Configuration::plotBackgroundColor()
{
    return mPlotBackgroundColor;
}

/*!
    Set the background color of the signal plot to \a c.
*/
void Configuration::setPlotBackgroundColor(QColor &c)
{
    mPlotBackgroundColor = c;
}

/*!
    Returns the color used when drawing text.
*/
QColor Configuration::textColor()
{
    return mTextColor;
}

/*!
    Set the color used when drawing text to \a c.
*/
void Configuration::setTextColor(QColor &c)
{
    mTextColor = c;
}

/*!
    Returns the color for the digital signal with ID \a id.
*/
QColor Configuration::digitalSignalColor(int id)
{
    if (id < 0) id = 0;

    return mDigitalSignalColors[ (id%MaxDigitalColors) ];
}

/*!
    Set the color for the digital signal with ID \a id to \a c.
*/
void Configuration::setDigitalSignalColor(int id, QColor &c)
{
    if (id < 0) id = 0;

    mDigitalSignalColors[ (id%MaxDigitalColors) ] = c;
}

/*!
    Returns the color for the analog signal with ID \a id.
*/
QColor Configuration::analogSignalColor(int id)
{
    if (id < 0) id = 0;

    return mAnalogSignalColors[ (id%MaxAnalogColors) ];
}

/*!
    Set the color for the analog signal with ID \a id to \a c.
*/
void Configuration::setAnalogSignalColor(int id, QColor &c)
{
    if (id < 0) id = 0;

    mAnalogSignalColors[ (id%MaxAnalogColors) ] = c;
}

/*!
    Returns the "ground" color for the analog signal with ID \a id.
*/
QColor Configuration::analogGroundColor(int id)
{
    if (id < 0) id = 0;

    return mAnalogGroundColors[ (id%MaxAnalogColors) ];
}

/*!
    Set the ground color for the analog signal with ID \a id to \a c.
*/
void Configuration::setAnalogGroundColor(int id, QColor &c)
{
    if (id < 0) id = 0;

    mAnalogGroundColors[ (id%MaxAnalogColors) ] = c;
}

/*!
    Returns the color for the cursor with ID \a id.
*/
QColor Configuration::cursorColor(int id)
{
    if (id < 0) id = 0;

    return mCursorColors[ (id%MaxCursorColors) ];
}

/*!
    Set the color for the cursor with ID \a id to \a c.
*/
void Configuration::setCursorColor(int id, QColor &c)
{
    if (id < 0) id = 0;

    mCursorColors[ (id%MaxCursorColors) ] = c;
}

/*!
    Returns the color for grid.
*/
QColor Configuration::gridColor()
{
    return mGridColor;
}

/*!
    Set the color for the grid to \a c.
*/
void Configuration::setGridColor(QColor &c)
{
    mGridColor = c;
}

/*!
    Returns the color used when painting analyzer signals.
*/
QColor Configuration::analyzerColor()
{
    return mAnalyzerColor;
}

/*!
    Set the color used when painting analyzer signals to \a c.
*/
void Configuration::setAnalyzerColor(QColor &c)
{
    mAnalyzerColor = c;
}

/*!
    Returns the color used as background color for widgets surrounding the
    signal plot.
*/
QColor Configuration::outsidePlotColor()
{
    //return QColor(235, 235, 235);
    return QColor(249, 249, 249);
}

/*!
    Load the color scheme called "Light".
*/
void Configuration::loadLightScheme()
{
    mActiveColorScheme = COLOR_SCHEME_LIGHT;

    mPlotBackgroundColor = Qt::white;
    mTextColor           = Qt::black;

    for (int i = 0; i < MaxDigitalColors; i++) {
        mDigitalSignalColors[i] = Qt::black;
    }
    mAnalyzerColor = Qt::black;

    mAnalogSignalColors[0] = QColor(0,255,255);   // Aqua
    mAnalogSignalColors[1] = QColor(30,144,255);  // DodgerBlue
    mAnalogSignalColors[2] = QColor(255,160,120); // LightSalmon
    mAnalogSignalColors[3] = QColor(255,0,0);     // Red

    for (int i = 0; i < MaxAnalogColors; i++) {
        mAnalogGroundColors[i] = Qt::green;
    }

    mCursorColors[0] = Qt::red;
    mCursorColors[1] = Qt::blue;
    mCursorColors[2] = Qt::darkBlue;
    mCursorColors[3] = Qt::green;
    mCursorColors[4] = Qt::darkGreen;

    mGridColor = Qt::gray;

}

/*!
    Load the color scheme called "Dark".
*/
void Configuration::loadDarkScheme()
{
    mActiveColorScheme = COLOR_SCHEME_DARK;

    mPlotBackgroundColor = Qt::black;
    mTextColor           = Qt::white;

    for (int i = 0; i < MaxDigitalColors; i++) {
        mDigitalSignalColors[i] = QColor(240, 240, 240);
    }
    mAnalyzerColor = QColor(240, 240, 240);;

    mAnalogSignalColors[0] = QColor(0,255,255);   // Aqua
    mAnalogSignalColors[1] = QColor(30,144,255);  // DodgerBlue
    mAnalogSignalColors[2] = QColor(255,160,120); // LightSalmon
    mAnalogSignalColors[3] = QColor(255,0,0);     // Red

    for (int i = 0; i < MaxAnalogColors; i++) {
        mAnalogGroundColors[i] = Qt::green;
    }

    mCursorColors[0] = Qt::red;
    mCursorColors[1] = Qt::blue;
    mCursorColors[2] = QColor(0, 255, 255);
    mCursorColors[3] = Qt::green;
    mCursorColors[4] = Qt::darkGreen;

    mGridColor = Qt::gray;

}

bool Configuration::ScrollingOn()
{
    return (mActiveBehavior == BEHAVE_SCROLL) ? true : false;
}
