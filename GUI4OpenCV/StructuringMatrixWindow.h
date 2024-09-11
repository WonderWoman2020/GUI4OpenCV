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

public slots:
	void updateMatrixDimensions();

private:

	int matrixRow;

	void buildWindow();

	int addMatrixDimensionsInput(int atRow);
	int addMatrix(int atRow);
	int addAlgorithmsList(int atRow);
};
