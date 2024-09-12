#pragma once

#include <QDialog>

#include <QGridLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>

#include "FilterMatrix.h"


enum FilterAlgorithm
{
	LOW_PASS_FILTER,
	HIGH_PASS_FILTER
};

class FilterMatrixWindow  : public QDialog
{
	Q_OBJECT

public:
	FilterMatrixWindow(QWidget *parent);
	~FilterMatrixWindow();

signals:
	void sendInputData(std::vector<std::vector<int>> matrixData, int divisor, FilterAlgorithm algorithm);

public slots:
	void updateMatrixDimensions();
	void readInputData();

private:
	int matrixRow;

	std::map<FilterAlgorithm, std::string> algorithmNames = {
	{ LOW_PASS_FILTER, "filtr dolnoprzepustowy"},
	{ HIGH_PASS_FILTER, "filtr gornoprzepustowy"}
	};

	void buildWindow();

	int addMatrixDimensionsInput(int atRow);
	int addMatrix(int atRow);
	int addDivisor(int atRow);
	int addAlgorithmsList(int atRow);
};
