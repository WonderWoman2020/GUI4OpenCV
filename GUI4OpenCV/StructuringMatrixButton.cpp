#include "StructuringMatrixButton.h"

StructuringMatrixButton::StructuringMatrixButton(QWidget *parent)
	: QPushButton(parent)
{
	this->clickCounter = 0;
	this->canBeCharacteristic = true;

	QString defaultStyle = "QPushButton{ background-color: white; border: none; }";
	//QString checkedStyle = "QPushButton:checked{ background-color: black; border: none; }";
	QString hoverStyle = "QPushButton:hover{ background-color: blue; border: none; }";

	QString myCheckedStyle = "QPushButton[myChecked=\"true\"]{ background-color: black; border: none; }";
	QString characteristicStyle = "QPushButton[characteristic=\"true\"]{ border: 3px solid red; }";

	this->style = defaultStyle + hoverStyle + myCheckedStyle + characteristicStyle;
	//this->setCheckable(true);
	this->setStyleSheet(this->style);

	this->setProperty("myChecked", false);
	this->setProperty("characteristic", false);

	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	this->setFixedSize(QSize(30, 30));

	connect(this, SIGNAL(clicked()), this, SLOT(onClicked()));
}

StructuringMatrixButton::~StructuringMatrixButton()
{}

void StructuringMatrixButton::onClicked()
{
	if(this->canBeCharacteristic)
		this->clickCounter = (this->clickCounter + 1) % 4;
	else
		this->clickCounter = (this->clickCounter + 1) % 2;

	switch (this->clickCounter)
	{
	case 0:
		this->setProperty("myChecked", false);
		this->setProperty("characteristic", false);
		break;
	case 1:
		this->setProperty("myChecked", true);
		this->setProperty("characteristic", false);
		break;
	case 2:
		this->setProperty("myChecked", true);
		this->setProperty("characteristic", true);
		break;
	case 3:
		this->setProperty("myChecked", false);
		this->setProperty("characteristic", true);
		break;
	default:
		this->setStyleSheet(this->style);
		break;
	}
	
	qInfo() << "Matrix button last interacted with:" << "checked -" << this->property("myChecked") << "characteristic -" << this->property("characteristic");
	this->setStyleSheet(this->style);
	
}

void StructuringMatrixButton::setCanBeCharacteristic(bool canBeCharacteristic)
{
	this->canBeCharacteristic = canBeCharacteristic;
}

bool StructuringMatrixButton::checked()
{
	QVariant checkedProperty = this->property("myChecked");
	if (checkedProperty.canConvert(QMetaType(QMetaType::Bool)))
		return checkedProperty.toBool();

	return false;
}

bool StructuringMatrixButton::characteristic()
{
	QVariant characteristicProperty = this->property("characteristic");
	if (characteristicProperty.canConvert(QMetaType(QMetaType::Bool)))
		return characteristicProperty.toBool();

	return false;
}