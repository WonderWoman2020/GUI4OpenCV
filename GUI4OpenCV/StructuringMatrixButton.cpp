#include "StructuringMatrixButton.h"

StructuringMatrixButton::StructuringMatrixButton(QWidget *parent)
	: QPushButton(parent)
{
	this->clickCounter = 0;

	QString defaultStyle = "QPushButton{ background-color: white; border: none; }";
	//QString checkedStyle = "QPushButton:checked{ background-color: black; border: none; }";
	QString hoverStyle = "QPushButton:hover{ background-color: blue; border: none; }";

	QString myCheckedStyle = "QPushButton[myChecked=\"true\"]{ background-color: black; border: none; }";
	QString chosenStyle = "QPushButton[chosen=\"true\"]{ border: 3px solid red; }";

	this->style = defaultStyle + hoverStyle + myCheckedStyle + chosenStyle;
	//this->setCheckable(true);
	this->setStyleSheet(this->style);

	this->setProperty("myChecked", false);
	this->setProperty("chosen", false);

	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	this->setFixedSize(QSize(30, 30));

	connect(this, SIGNAL(clicked()), this, SLOT(on_clicked()));
}

StructuringMatrixButton::~StructuringMatrixButton()
{}

void StructuringMatrixButton::on_clicked()
{
	this->clickCounter = (this->clickCounter + 1) % 4;

	switch (this->clickCounter)
	{
	case 0:
		this->setProperty("myChecked", false);
		this->setProperty("chosen", false);
		break;
	case 1:
		this->setProperty("myChecked", true);
		this->setProperty("chosen", false);
		break;
	case 2:
		this->setProperty("myChecked", true);
		this->setProperty("chosen", true);
		break;
	case 3:
		this->setProperty("myChecked", false);
		this->setProperty("chosen", true);
		break;
	default:
		this->setStyleSheet(this->style);
		break;
	}
	
	qInfo() << "checked:" << this->property("myChecked") << "chosen:" << this->property("chosen");
	this->setStyleSheet(this->style);
	
}