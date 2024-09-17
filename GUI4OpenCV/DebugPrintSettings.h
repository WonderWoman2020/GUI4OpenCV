#pragma once

#include <QDebug>

class DebugPrintSettings
{
public:

    /*
     * Sets how debug information will be printed out when using qDebug(), qInfo() and similar functions.
     * This pattern is only set for the scope of this window (and its children) - for another window debugging,
     * one should use a function like this in the desired window as well.
     */
	void setDebugPrintingPatterns();
};

