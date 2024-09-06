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
	void changeSliderLabel(int value);

private:
	Ui::AlphaSliderClass *ui;
};
