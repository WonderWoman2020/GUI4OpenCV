#include "StructuringMatrixButton.h"

StructuringMatrixButton::StructuringMatrixButton(QWidget *parent)
	: QPushButton(parent)
{

	QString defaultStyle = "QPushButton{ background-color: white; border: none; }";
	QString checkedStyle = "QPushButton:checked{ background-color: black; border: none; }";
	QString hoverStyle = "QPushButton:hover{ background-color: blue; border: none; }";
	this->setCheckable(true);
	this->setStyleSheet(defaultStyle + checkedStyle + hoverStyle);

	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	this->setFixedSize(QSize(30, 30));
}

StructuringMatrixButton::~StructuringMatrixButton()
{}
