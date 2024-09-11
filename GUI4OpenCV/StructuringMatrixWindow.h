#pragma once

#include <QDialog>

#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QDialogButtonBox>

#include "StructuringMatrix.h"

class StructuringMatrixWindow  : public QDialog
{
	Q_OBJECT

public:
	StructuringMatrixWindow(QWidget *parent);
	~StructuringMatrixWindow();

private:
	void buildWindow();

	int addMatrixDimensionsInput(int atRow);
	int addMatrix(int atRow);
	int addAlgorithmsList(int atRow);
};
