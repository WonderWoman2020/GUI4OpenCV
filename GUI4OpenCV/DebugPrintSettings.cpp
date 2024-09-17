#include "DebugPrintSettings.h"


void DebugPrintSettings::setDebugPrintingPatterns()
{
    // Sets the format of debug messages
    const QString debugMessagePattern = "Qt at [%{time h:mm:ss.zzz}], %{type}: %{message}";
    qSetMessagePattern(debugMessagePattern);

    // Some examples, that can be seen in console
    qDebug() << "Debug message";
    qInfo() << "Info message";
    qCritical() << "Critical message";
}