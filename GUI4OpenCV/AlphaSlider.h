#pragma once

#include <QWidget>
#include "ui_AlphaSlider.h"

#include <QSlider>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class AlphaSliderClass; };
QT_END_NAMESPACE

class AlphaSlider : public QWidget
{
	Q_OBJECT

public:
	AlphaSlider(QWidget *parent = nullptr);
	~AlphaSlider();

public slots:
	/**
	 * Slot that is activated when slider value changes (when it was moved).
	 * It updates a label that is over the slider with the current value.
	 * @params value - current value of the slider, that will be set to its label.
	 */
	void changeSliderLabel(int value);

signals:
	/**
	 * Signal that is send when slider value changes (when it was moved).
	 * It should be received by some outer object, that wants to read the slider value.
	 */
	void sliderValueChanged(int value);

private:
	/**
	 * View of the slider was created as a static XML .ui file, so the ui field is present for accessing the view UI elements.
	 */
	Ui::AlphaSliderClass *ui;
};
