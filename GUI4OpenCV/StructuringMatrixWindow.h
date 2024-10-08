#pragma once

#include <QDialog>

#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QDialogButtonBox>

#include "StructuringMatrix.h"

enum StructuringAlgorithm
{
	EROSION,
	DILATION,
	OPENING,
	CLOSING
};


class StructuringMatrixWindow  : public QDialog
{
	Q_OBJECT

public:
	StructuringMatrixWindow(QWidget *parent);
	~StructuringMatrixWindow();

signals:
	void sendInputData(std::vector<std::vector<bool>> matrixData, std::pair<int, int> characteristicElement, StructuringAlgorithm algorithm);

public slots:
	void updateMatrixDimensions();
	void readInputData();

private:

	int matrixRow;

	std::map<StructuringAlgorithm, std::string> algorithmNames = {
		{ EROSION, "erozja"},
		{ DILATION, "dylatacja"},
		{ OPENING, "otwarcie"},
		{ CLOSING, "zamkniecie"}
	};

	void buildWindow();

	int addMatrixDimensionsInput(int atRow);
	int addMatrix(int atRow);
	int addAlgorithmsList(int atRow);
};
