#include "StructuringMatrix.h"

StructuringMatrix::StructuringMatrix(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::StructuringMatrixClass())
{
	ui->setupUi(this);
}

StructuringMatrix::~StructuringMatrix()
{
	delete ui;
}
