#include "AlphaSlider.h"

AlphaSlider::AlphaSlider(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::AlphaSliderClass())
{
	ui->setupUi(this);

	QSlider* slider = ui->alphaSlider;
	connect(slider, SIGNAL(valueChanged(int)), this, SLOT(changeSliderLabel(int)));
}

AlphaSlider::~AlphaSlider()
{
	delete ui;
}

void AlphaSlider::changeSliderLabel(int value)
{
	QSlider* slider = ui->alphaSlider;
	QLabel* label = ui->sliderLabel;

	double step = (double) slider->width() / (double) (slider->maximum() - slider->minimum());
	label->setText(QString::number(value));
	label->move(slider->x() + (std::ceil(slider->sliderPosition()*step)), label->y());
}
