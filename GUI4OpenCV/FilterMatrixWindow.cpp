#include "FilterMatrixWindow.h"

FilterMatrixWindow::FilterMatrixWindow(QWidget *parent)
	: QDialog(parent)
{
    this->matrixRow = 0;

    this->setParent(parent);
    this->setWindowTitle("Macierz filtrow");
    this->buildWindow();
}

FilterMatrixWindow::~FilterMatrixWindow()
{

}


void FilterMatrixWindow::readInputData()
{
    FilterMatrix* matrix = this->findChild<FilterMatrix*>("filterMatrix");
    QLineEdit* divisorField = this->findChild<QLineEdit*>("divisorField");
    QComboBox* algorithms = this->findChild<QComboBox*>("algorithmsList");

    auto matrixData = matrix->getMatrixData();
    std::string chosenAlgorithm = algorithms->currentText().toStdString();
    int divisor = divisorField->text().toInt();

    emit sendInputData(matrixData, divisor, chosenAlgorithm);
}

void FilterMatrixWindow::buildWindow()
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

    rowsTaken = this->addDivisor(currentRow);
    currentRow += rowsTaken;

    rowsTaken = this->addAlgorithmsList(currentRow);
    currentRow += rowsTaken;

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    windowGrid->addWidget(buttonBox, currentRow, 2, 1, 1);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(close()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(readInputData()));
}

int FilterMatrixWindow::addMatrixDimensionsInput(int atRow)
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
    grid->addWidget(rows, atRow + rowsTaken, 1, 1, 1);
    rowsTaken++;
    grid->addWidget(colsLabel, atRow + rowsTaken, 0, 1, 1);
    grid->addWidget(cols, atRow + rowsTaken, 1, 1, 1);
    rowsTaken++;

    connect(rows, SIGNAL(valueChanged(int)), this, SLOT(updateMatrixDimensions()));
    connect(cols, SIGNAL(valueChanged(int)), this, SLOT(updateMatrixDimensions()));

    return rowsTaken;
}

int FilterMatrixWindow::addMatrix(int atRow)
{
    // Adds structuring element label and matrix
    int rowsNum = 5;
    int colsNum = 5;
    FilterMatrix* widget = new FilterMatrix(this, rowsNum, colsNum);
    widget->setObjectName("filterMatrix");

    auto data = widget->getMatrixData();
    qInfo() << data.at(0);

    QGridLayout* grid = (QGridLayout*)this->layout();
    int rowsTaken = 0;
    QLabel* matrixLabel = new QLabel(this);
    matrixLabel->setText("Macierz filtrow: ");
    grid->addWidget(matrixLabel, atRow, 0, 1, 1);
    widget->setParent(this);
    grid->addWidget(widget, atRow, 1, 1, 3);
    rowsTaken++;

    this->matrixRow = atRow;

    return rowsTaken;
}

int FilterMatrixWindow::addDivisor(int atRow)
{
    QLabel* divisorLabel = new QLabel(this);
    divisorLabel->setText("Dzielnik: ");
    QLineEdit* divisor = new QLineEdit(this);
    QValidator* integerValidator = new QIntValidator(INT32_MIN, INT32_MAX, this);
    divisor->setValidator(integerValidator);
    divisor->setFixedSize(QSize(30, 30));
    divisor->setObjectName("divisorField");
    divisor->setText("1");

    QGridLayout* grid = (QGridLayout*)this->layout();
    int rowsTaken = 0;
    grid->addWidget(divisorLabel, atRow, 0, 1, 1);
    grid->addWidget(divisor, atRow, 1, 1, 1);
    rowsTaken++;

    return rowsTaken;
}

int FilterMatrixWindow::addAlgorithmsList(int atRow)
{
    // Adds algorithms dropdown list
    QLabel* algorithmsLabel = new QLabel(this);
    algorithmsLabel->setText("Algorytm: ");
    QComboBox* algorithms = new QComboBox(this);
    algorithms->setObjectName("algorithmsList");

    algorithms->addItem("filtr dolnoprzepustowy");
    algorithms->addItem("filtr gornoprzepustowy");
    algorithms->addItem("...");

    QGridLayout* grid = (QGridLayout*)this->layout();
    int rowsTaken = 0;
    grid->addWidget(algorithmsLabel, atRow, 0, 1, 1);
    grid->addWidget(algorithms, atRow, 1, 1, 1);
    rowsTaken++;

    return rowsTaken;
}

void FilterMatrixWindow::updateMatrixDimensions()
{
    FilterMatrix* previousMatrix = this->findChild<FilterMatrix*>("filterMatrix");
    previousMatrix->setParent(nullptr);
    previousMatrix->deleteLater();

    QSpinBox* rows = (QSpinBox*)this->findChild<QSpinBox*>("rows");
    QSpinBox* cols = (QSpinBox*)this->findChild<QSpinBox*>("cols");

    FilterMatrix* matrix = new FilterMatrix(this, rows->value(), cols->value());
    matrix->setObjectName("filterMatrix");
    QGridLayout* grid = (QGridLayout*)this->layout();
    grid->addWidget(matrix, this->matrixRow, 1, 1, 3);

    this->resize(100, 100);
}