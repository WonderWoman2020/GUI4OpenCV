#pragma once

#include <QWidget>

#include <QGridLayout>
#include <QLineEdit>
#include <QValidator>

#include <stdexcept>

class FilterMatrix  : public QWidget
{
	Q_OBJECT

public:
	FilterMatrix(QWidget *parent, int rows = 5, int cols = 5);
	~FilterMatrix();

	/**
	 * Returns filter matrix input data provided by the user.
	 * @return A matrix of integers. It is a vector of n vectors (n - filter matrix rows count) 
	 * that each store one row of the matrix data (all rows are the same size, which is filter matrix columns count). The numbers can be negative.
	 */
	std::vector<std::vector<int>> getMatrixData();

public slots:
	/**
	 * Slot that is activated whenever a user changes input in any matrix field.
	 * It updates the filter matrix data stored in this class field 'data'.
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
	 * A matrix of integers. It is a vector of n vectors (n - filter matrix rows count) 
	 * that each store one row of the matrix data (all rows are the same size, which is filter matrix columns count). The numbers can be negative.
	 */
	std::vector<std::vector<int>> data;

	/**
	 * Dynamically builds this matrix widget to have given number of input fields.
	 */
	void buildMatrix(int rows, int cols);
};
