#include "ParametersWindow.h"

ParametersWindow::ParametersWindow(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::ParametersWindowClass())
{
	ui->setupUi(this);
}

ParametersWindow::~ParametersWindow()
{
	delete ui;
}
