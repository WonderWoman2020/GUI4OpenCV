#pragma once

#include <QWidget>
#include <QGridLayout>

#include "StructuringMatrixButton.h"

class StructuringMatrix : public QWidget
{
	Q_OBJECT

public:
	StructuringMatrix(QWidget* parent = nullptr, int rows = 3, int cols = 3);
	~StructuringMatrix();

private:
	void buildMatrix(int rows, int cols);
};