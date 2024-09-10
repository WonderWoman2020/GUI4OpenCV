#include "StructuringMatrix.h"


StructuringMatrix::StructuringMatrix(QWidget* parent, int rows, int cols)
{
	this->setParent(parent);
	this->buildMatrix(rows, cols);
}

StructuringMatrix::~StructuringMatrix()
{

}

/*
    Creates structuring element matrix.
*/
void StructuringMatrix::buildMatrix(int rows, int cols)
{
    QGridLayout* grid = new QGridLayout(this);
    this->setLayout(grid);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            StructuringMatrixButton* button = new StructuringMatrixButton(this);
            button->setProperty("row", i);
            button->setProperty("col", j);
            grid->addWidget(button, i, j);
        }
    }
}
