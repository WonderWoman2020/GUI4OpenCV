#include "StructuringMatrixWindow.h"

StructuringMatrixWindow::StructuringMatrixWindow(QWidget *parent)
	: QDialog(parent)
{
    this->matrixRow = 0;

    this->setParent(parent);
	this->buildWindow();
}

StructuringMatrixWindow::~StructuringMatrixWindow()
{

}

void StructuringMatrixWindow::buildWindow()
{
    QGridLayout* windowGrid = new QGridLayout(this);
    this->resize(100, 100);
    this->setWindowModality(Qt::WindowModal);

    int currentRow = 0;
    int rowsTaken = 0;

    rowsTaken = this->addMatrixDimensionsInput(currentRow);
    currentRow += rowsTaken;

    rowsTaken = this->addMatrix(currentRow);
    currentRow += rowsTaken;

    rowsTaken = this->addAlgorithmsList(currentRow);
    currentRow += rowsTaken;

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);

    windowGrid->addWidget(buttonBox, currentRow, 2, 1, 1);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(close()));
}

int StructuringMatrixWindow::addMatrixDimensionsInput(int atRow)
{
    // Adds labels and column and rows input fields
    QLabel* rowsLabel = new QLabel(this);
    rowsLabel->setText("Liczba wierszy: ");
    QLabel* colsLabel = new QLabel(this);
    colsLabel->setText("Liczba kolumn: ");
    QSpinBox* rows = new QSpinBox(this);
    QSpinBox* cols = new QSpinBox(this);
    rows->setObjectName("rows");
    cols->setObjectName("cols");

    rows->setValue(5);
    cols->setValue(5);

    QGridLayout* grid = (QGridLayout*)this->layout();
    int rowsTaken = 0;
    grid->addWidget(rowsLabel, atRow + rowsTaken, 0, 1, 1);
    grid->addWidget(rows, atRow+rowsTaken, 1, 1, 1);
    rowsTaken++;
    grid->addWidget(colsLabel, atRow+rowsTaken, 0, 1, 1);
    grid->addWidget(cols, atRow+rowsTaken, 1, 1, 1);
    rowsTaken++;

    connect(rows, SIGNAL(valueChanged(int)), this, SLOT(updateMatrixDimensions()));
    connect(cols, SIGNAL(valueChanged(int)), this, SLOT(updateMatrixDimensions()));

    return rowsTaken;
}

int StructuringMatrixWindow::addMatrix(int atRow)
{
    // Adds structuring element label and matrix
    int rowsNum = 5;
    int colsNum = 5;
    StructuringMatrix* widget = new StructuringMatrix(this, rowsNum, colsNum);
    widget->setObjectName("structuringMatrix");

    auto data = widget->getMatrixData();
    qInfo() << data.at(0);

    QGridLayout* grid = (QGridLayout*)this->layout();
    int rowsTaken = 0;
    QLabel* matrixLabel = new QLabel(grid->parentWidget());
    matrixLabel->setText("Element strukturalny: ");
    grid->addWidget(matrixLabel, atRow, 0, 1, 1);
    widget->setParent(grid->parentWidget());
    grid->addWidget(widget, atRow, 1, 1, 3);
    rowsTaken++;

    this->matrixRow = atRow;

    return rowsTaken;
}

int StructuringMatrixWindow::addAlgorithmsList(int atRow)
{
    // Adds algorithms dropdown list
    QLabel* algorithmsLabel = new QLabel(this);
    algorithmsLabel->setText("Algorytm: ");
    QComboBox* algorithms = new QComboBox(this);
    algorithms->addItem("erozja");
    algorithms->addItem("dylatacja");
    algorithms->addItem("otwarcie");
    algorithms->addItem("zamkniecie");

    QGridLayout* grid = (QGridLayout*)this->layout();
    int rowsTaken = 0;
    grid->addWidget(algorithmsLabel, atRow, 0, 1, 1);
    grid->addWidget(algorithms, atRow, 1, 1, 1);
    rowsTaken++;

    return rowsTaken;
}

void StructuringMatrixWindow::updateMatrixDimensions()
{
    StructuringMatrix* previousMatrix = this->findChild<StructuringMatrix*>("structuringMatrix");
    previousMatrix->setParent(nullptr);
    previousMatrix->deleteLater();

    QSpinBox* rows = (QSpinBox*)this->findChild<QSpinBox*>("rows");
    QSpinBox* cols = (QSpinBox*)this->findChild<QSpinBox*>("cols");

    StructuringMatrix* matrix = new StructuringMatrix(this, rows->value(), cols->value());
    matrix->setObjectName("structuringMatrix");
    QGridLayout* grid = (QGridLayout*)this->layout();
    grid->addWidget(matrix, this->matrixRow, 1, 1, 3);
}