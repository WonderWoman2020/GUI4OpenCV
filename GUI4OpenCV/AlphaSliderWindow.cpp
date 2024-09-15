#include "AlphaSliderWindow.h"

AlphaSliderWindow::AlphaSliderWindow(QWidget *parent)
	: QWidget(parent)
{
    this->setParent(parent);
	this->buildWindow();
}

AlphaSliderWindow::~AlphaSliderWindow()
{

}

void AlphaSliderWindow::buildWindow()
{
    // Adds grid layout to window
    QGridLayout* layout = new QGridLayout(this);
    this->setLayout(layout);

    // Adds slider widget to window
    AlphaSlider* slider = new AlphaSlider(this);
    this->layout()->addWidget(slider);
    this->resize(slider->width() + 50, slider->height() + 50);

    // Sets some window settings
    this->setWindowFlag(Qt::Window);    // Changes widget type to window
    this->setWindowModality(Qt::NonModal);
    this->setAttribute(Qt::WA_DeleteOnClose);    // Makes window destroy itself, when closed
    this->setWindowTitle("Mieszanie obrazow");

    connect(slider, SIGNAL(sliderValueChanged(int)), this, SIGNAL(sendInputData(int)));
}


