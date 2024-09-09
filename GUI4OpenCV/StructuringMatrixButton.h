#pragma once

#include <QPushButton>

class StructuringMatrixButton  : public QPushButton
{
	Q_OBJECT

public:
	StructuringMatrixButton(QWidget *parent);
	~StructuringMatrixButton();

	void setCanBeChosen(bool canBeChosen);
	bool checked();
	bool chosen();

public slots:
	void on_clicked();

private:
	int clickCounter;
	QString style;

	bool canBeChosen;
};
