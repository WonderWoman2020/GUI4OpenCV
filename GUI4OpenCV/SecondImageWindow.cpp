#include "SecondImageWindow.h"

SecondImageWindow::SecondImageWindow(QWidget *parent)
	: QWidget(parent)
{
	this->setParent(parent);
	this->buildWindow();
}

SecondImageWindow::~SecondImageWindow()
{

}

void SecondImageWindow::buildWindow()
{    
    // Adds grid layout to the widget
    QGridLayout* layout = new QGridLayout(this);
    this->setLayout(layout);

    // Adds image view widget to the window
    QGraphicsView* secondImageView = new QGraphicsView(this);
    secondImageView->setObjectName("imageView");
    this->layout()->addWidget(secondImageView);

    // Sets some window settings
    this->setWindowFlag(Qt::Window);    // Changes widget type to window
    this->setWindowModality(Qt::NonModal);
    this->setAttribute(Qt::WA_DeleteOnClose);    // Makes window destroy itself, when closed
    this->setWindowTitle("Drugi obraz wejsciowy");
}

QGraphicsView* SecondImageWindow::getImageView()
{
    return this->findChild<QGraphicsView*>("imageView");
}
