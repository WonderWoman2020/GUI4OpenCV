#pragma once

#include <QWidget>

#include <QGridLayout>
#include <QLineEdit>
#include <QValidator>

class FilterMatrix  : public QWidget
{
	Q_OBJECT

public:
	FilterMatrix(QWidget *parent, int rows = 5, int cols = 5);
	~FilterMatrix();

	std::vector<std::vector<int>> getMatrixData();

public slots:
	void updateMatrixData();

private:

	int rows;
	int cols;

	std::vector<std::vector<int>> data;

	void buildMatrix(int rows, int cols);
};
