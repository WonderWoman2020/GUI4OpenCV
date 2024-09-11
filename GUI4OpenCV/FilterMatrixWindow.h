#pragma once

#include <QDialog>

#include <QGridLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>

#include "FilterMatrix.h"

class FilterMatrixWindow  : public QDialog
{
	Q_OBJECT

public:
	FilterMatrixWindow(QWidget *parent);
	~FilterMatrixWindow();


public slots:
	void updateMatrixDimensions();

private:
	int matrixRow;

	void buildWindow();

	int addMatrixDimensionsInput(int atRow);
	int addMatrix(int atRow);
	int addDivisor(int atRow);
	int addAlgorithmsList(int atRow);
};
