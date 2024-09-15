#include "StructuringMatrix.h"


StructuringMatrix::StructuringMatrix(QWidget* parent, int rows, int cols)
{
    this->rows = rows;
    this->cols = cols;

	this->setParent(parent);
	this->buildMatrix(rows, cols);

    this->data = this->readStructuringElement();
    this->characteristicElement = this->readCharacteristicElement();

    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

StructuringMatrix::~StructuringMatrix()
{

}

std::vector<std::vector<bool>> StructuringMatrix::getMatrixData()
{
    return this->data;
}

std::pair<int, int> StructuringMatrix::getCharacteristicElement()
{
    return this->characteristicElement;
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
            button->setObjectName("structuringButton");
            grid->addWidget(button, i, j);

            connect(button, SIGNAL(clicked()), this, SLOT(updateMatrixData()));
        }
    }
}

std::vector<std::vector<bool>> StructuringMatrix::readStructuringElement()
{
    QList<StructuringMatrixButton*> buttons = this->findChildren<StructuringMatrixButton*>("structuringButton");
    std::vector<std::vector<bool>> dataMatrix(this->rows, std::vector<bool>(this->cols));

    if (buttons.size() != (this->rows * this->cols))
        throw std::invalid_argument("Structuring matrix couldn't have been read properly."
            " Structuring matrix buttons (matrix fields) number didn't match the declared size of the matrix.");

    for (int i = 0; i < buttons.size(); i++)
    {
        StructuringMatrixButton* button = buttons.at(i);
        if (button->property("row").canConvert(QMetaType(QMetaType::Int))
            && button->property("col").canConvert(QMetaType(QMetaType::Int)))
        {
            int row = button->property("row").toInt();
            int col = button->property("col").toInt();

            if (row >= dataMatrix.size() || col >= dataMatrix.at(row).size())
                throw std::out_of_range("Structuring matrix couldn't have been read properly."
                " For at least one structuring matrix button (a matrix field) row or column index was out of bounds, according to the declared matrix size.");

            dataMatrix[row][col] = button->checked();
        }
        else
            throw std::invalid_argument("Structuring matrix couldn't have been read properly."
                " At least one structuring matrix button (a matrix field) didn't have 'row' and 'col' properties set or they weren't set to integer numbers.");
    }
    return dataMatrix;
}

std::pair<int, int> StructuringMatrix::readCharacteristicElement()
{
    std::pair<int, int> characteristicElement;
    QList<StructuringMatrixButton*> buttons = this->findChildren<StructuringMatrixButton*>("structuringButton");

    if (buttons.size() != (this->rows * this->cols))
        throw std::invalid_argument("Structuring matrix couldn't have been read properly."
            " Structuring matrix buttons (matrix fields) number didn't match the declared size of the matrix.");

    for (int i = 0; i < buttons.size(); i++)
    {
        StructuringMatrixButton* button = buttons.at(i);
        if (button->property("row").canConvert(QMetaType(QMetaType::Int))
            && button->property("col").canConvert(QMetaType(QMetaType::Int)))
        {
            int row = button->property("row").toInt();
            int col = button->property("col").toInt();

            if (row >= this->rows || col >= this->cols)
                throw std::out_of_range("Structuring matrix couldn't have been read properly."
                    " For at least one structuring matrix button (a matrix field) row or column index was out of bounds, according to the declared matrix size.");

            // Returns first characteristic element that has been found
            if (button->characteristic())
            {
                characteristicElement.first = row;
                characteristicElement.second = col;
                return characteristicElement;
            }
        }
        else
            throw std::invalid_argument("Structuring matrix couldn't have been read properly."
                " At least one structuring matrix button (a matrix field) didn't have 'row' and 'col' properties set or they weren't set to integer numbers.");
    }

    characteristicElement.first = -1;
    characteristicElement.second = -1;
    return characteristicElement;
}

void StructuringMatrix::disableChoosingCharacteristicElement()
{
    QList<StructuringMatrixButton*> buttons = this->findChildren<StructuringMatrixButton*>("structuringButton");
    for(StructuringMatrixButton* button : buttons)
    {
        int row = button->property("row").toInt();
        int col = button->property("col").toInt();
        if (row == this->characteristicElement.first && col == this->characteristicElement.second)
            continue;

        button->setCanBeCharacteristic(false);
    }
}

void StructuringMatrix::enableChoosingCharacteristicElement()
{
    QList<StructuringMatrixButton*> buttons = this->findChildren<StructuringMatrixButton*>("structuringButton");
    for (StructuringMatrixButton* button : buttons)
    {
        int row = button->property("row").toInt();
        int col = button->property("col").toInt();
        if (row == this->characteristicElement.first && col == this->characteristicElement.second)
            continue;

        button->setCanBeCharacteristic(true);
    }
}


void StructuringMatrix::updateMatrixData()
{
    this->data = this->readStructuringElement();
    this->characteristicElement = this->readCharacteristicElement();

    if (characteristicElement.first == -1 && characteristicElement.second == -1)
        this->enableChoosingCharacteristicElement();
    else
        this->disableChoosingCharacteristicElement();

    qInfo() << this->data.at(0);
    qInfo() << this->characteristicElement;
}
