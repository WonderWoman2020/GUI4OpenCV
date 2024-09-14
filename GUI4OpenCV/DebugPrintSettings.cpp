#include "DebugPrintSettings.h"


/*
    Sets how debug information will be printed out when using qDebug(), qInfo() and similar functions.
    This pattern is only set for the scope of this window (and its children) - for another window debugging,
    one should add a function like this in the desired window as well.
*/
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