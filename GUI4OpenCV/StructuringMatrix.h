#pragma once

#include <QWidget>
#include <QGridLayout>

#include "StructuringMatrixButton.h"

class StructuringMatrix : public QWidget
{
	Q_OBJECT

public:
	StructuringMatrix(QWidget* parent = nullptr, int rows = 5, int cols = 5);
	~StructuringMatrix();

	std::vector<std::vector<bool>> getMatrixData();
	std::pair<int, int> getCharacteristicElement();

public slots:
	void updateMatrixData();

private:

	int rows;
	int cols;

	std::vector<std::vector<bool>> data;
	std::pair<int, int> characteristicElement;

	void buildMatrix(int rows, int cols);
	std::vector<std::vector<bool>> readStructuringElement();
	std::pair<int, int> readCharacteristicElement();

	void disableChoosingCharacteristicElement();
	void enableChoosingCharacteristicElement();
};