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

    this->srcSecondImage.release();
}


void ParametersWindow::on_secondImageButton_clicked()
{
    // Opens a file explorer and gets a path of the chosen image
    QString fileName = QFileDialog::getOpenFileName(this, tr("Wybierz obraz"),
        "/home",
        tr("Images (*.png *.jpg *.jpeg *.bmp)"));

    qInfo() << fileName;

    // Cancels open action, if user clicked "cancel" and the path is null
    if (fileName.isNull() || fileName.isEmpty())
    {
        QMessageBox::information(this, "Nie wybrano obrazu do otworzenia",
            "Nie wybrano obrazu wejsciowego do otworzenia. Musisz wybrac jakis obraz wejsciowy, aby go wyswietlic.");
        return;
    }

    // Tries to read the chosen file
    cv::Mat temp = cv::imread(fileName.toStdString());
    if (temp.empty())
    {
        QMessageBox::information(this, "Nie pozyskano danych obrazu",
            "Nie mozna bylo pozyskac danych obrazu. Upewnij sie, ze podany plik zawiera dane obrazu i ze masz do niego odpowiednie pozwolenia.");
        temp.release();
        return;
    }
    this->srcSecondImage = temp;
    cv::namedWindow("Second image");
    cv::imshow("Second image", this->srcSecondImage);
}

void ParametersWindow::on_confirmButton_clicked()
{
    this->close();
}