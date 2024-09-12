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

signals:
	void sendInputData(std::vector<std::vector<int>> matrixData, int divisor, std::string algorithm);

public slots:
	void updateMatrixDimensions();
	void readInputData();

private:
	int matrixRow;

	void buildWindow();

	int addMatrixDimensionsInput(int atRow);
	int addMatrix(int atRow);
	int addDivisor(int atRow);
	int addAlgorithmsList(int atRow);
};
