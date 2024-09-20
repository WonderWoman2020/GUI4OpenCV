#pragma once

#include <QWidget>
#include <QGridLayout>

#include <stdexcept>

#include "StructuringMatrixButton.h"

class StructuringMatrix : public QWidget
{
	Q_OBJECT

public:
	StructuringMatrix(QWidget* parent = nullptr, int rows = 5, int cols = 5);
	~StructuringMatrix();

	/**
	 * Returns structuring matrix input data provided by the user (Which buttons have been checked, and which not).
	 * @return A matrix of bools (True - a button is checked and shows as black, False - unchecked and white).
	 * It is a vector of n vectors (n - structuring matrix rows count) that each store one row of the matrix data
	 * (all rows are the same size, which is structuring matrix columns count).
	 */
	std::vector<std::vector<bool>> getMatrixData();

	/**
	 * Returns coordinates (row, column) at the matrix of which field was chosen as characteristic element (it shows as red border over a button).
	 * Characteristic element can be either a checked or unchecked button (black or white).
	 * @return A matrix field coordinates, at which the characteristic element is set. If any charcteristic element wasn't set, returns (-1,-1) coords.
	 */
	std::pair<int, int> getCharacteristicElement();

public slots:
	/**
	 * Slot that is activated whenever a user changes input in any matrix field.
	 * It updates the structuring matrix data stored in this class field 'data'.
	 */
	void updateMatrixData();

private:

	/**
	 * Rows number in the matrix.
	 */
	int rows;

	/**
	 * Columns number in the matrix.
	 */
	int cols;

	/**
	 * A matrix of bools (True - a button is checked and shows as black, False - unchecked and white).
	 * It is a vector of n vectors (n - structuring matrix rows count) that each store one row of the matrix data
	 * (all rows are the same size, which is structuring matrix columns count).
	 */
	std::vector<std::vector<bool>> data;

	/**
	 * A matrix field coordinates, at which the characteristic element is set. If any charcteristic element wasn't set, has (-1,-1) coords.
	 */
	std::pair<int, int> characteristicElement;

	/**
	 * Dynamically builds this matrix widget to have given number of input fields.
	 */
	void buildMatrix(int rows, int cols);

	/**
	 * Reads the whole matrix current state.
	 */
	std::vector<std::vector<bool>> readStructuringElement();

	/**
	 * Finds current characteristic element.
	 */
	std::pair<int, int> readCharacteristicElement();

	/**
	 * Only 1 characteristic element is allowed, so if one is already chosen, others have the option to be chosen disabled.
	 * Only the chosen field can be unset, and then all the fields can be chosen again.
	 */
	void disableChoosingCharacteristicElement();

	/**
	 * Only 1 characteristic element is allowed, so if one is already chosen, others have the option to be chosen disabled.
	 * Only the chosen field can be unset, and then all the fields can be chosen again.
	 */
	void enableChoosingCharacteristicElement();
};