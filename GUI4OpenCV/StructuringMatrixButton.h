#pragma once

#include <QPushButton>

class StructuringMatrixButton  : public QPushButton
{
	Q_OBJECT

public:
	StructuringMatrixButton(QWidget *parent);
	~StructuringMatrixButton();

	void setCanBeCharacteristic(bool canBeCharacteristic);
	bool checked();
	bool characteristic();

public slots:
	void onClicked();

private:
	int clickCounter;
	QString style;

	bool canBeCharacteristic;
};
