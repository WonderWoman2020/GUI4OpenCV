#pragma once

#include <QWidget>
#include <QGridLayout>
#include "AlphaSlider.h"

class AlphaSliderWindow  : public QWidget
{
	Q_OBJECT

public:
	AlphaSliderWindow(QWidget *parent);
	~AlphaSliderWindow();

signals:
	void sendInputData(int sliderInput);

private:
	void buildWindow();
};
