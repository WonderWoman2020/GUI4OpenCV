#include "FilterMatrix.h"

FilterMatrix::FilterMatrix(QWidget *parent, int rows, int cols)
	: QWidget(parent)
{
	this->rows = rows;
	this->cols = cols;

	this->setParent(parent);
	this->buildMatrix(rows, cols);

    this->data.assign(this->rows, std::vector<int>(this->cols));
    for (int i = 0; i < this->data.size(); i++)
        std::fill(this->data.at(i).begin(), this->data.at(i).end(), 0);

	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

FilterMatrix::~FilterMatrix()
{

}

std::vector<std::vector<int>> FilterMatrix::getMatrixData()
{
	return this->data;
}

void FilterMatrix::buildMatrix(int rows, int cols)
{
    QGridLayout* grid = new QGridLayout(this);
    this->setLayout(grid);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            QLineEdit* inputField = new QLineEdit(this);
            QValidator* integerValidator = new QIntValidator(INT32_MIN, INT32_MAX, this);
            inputField->setValidator(integerValidator);
            inputField->setFixedSize(QSize(30, 30));
            inputField->setAlignment(Qt::AlignCenter);

            inputField->setProperty("row", i);
            inputField->setProperty("col", j);
            inputField->setObjectName("filterField");
            grid->addWidget(inputField, i, j);

            connect(inputField, SIGNAL(editingFinished()), this, SLOT(updateMatrixData()));
        }
    }
}

void FilterMatrix::updateMatrixData()
{
    QLineEdit* inputField = (QLineEdit*) this->sender();
    
    if (inputField->property("row").canConvert(QMetaType(QMetaType::Int))
        && inputField->property("col").canConvert(QMetaType(QMetaType::Int)))
    {
        int row = inputField->property("row").toInt();
        int col = inputField->property("col").toInt();

        if (row >= this->data.size() || col >= this->data.at(row).size())
            throw std::out_of_range("Filter matrix couldn't have been read properly."
                " For at least one filter matrix input field, row or column index was out of bounds, according to the declared matrix size.");

        this->data[row][col] = inputField->text().toInt();
        qInfo() << "Updated filter matrix data at (" + QString::number(row) + QString(",") + QString::number(col) + "):" << this->data[row][col];
    }
    else
        throw std::invalid_argument("Filter matrix couldn't have been read properly."
            " At least one filter matrix input field didn't have 'row' and 'col' properties set or they weren't set to integer numbers.");
}
