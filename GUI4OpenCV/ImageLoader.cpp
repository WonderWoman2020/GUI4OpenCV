#include "ImageLoader.h"

ImageLoader::ImageLoader()
{

}

ImageLoader::~ImageLoader()
{

}

cv::Mat ImageLoader::readInImage(std::string path)
{
    // Tries to read the chosen file
    cv::Mat temp = cv::imread(path);
    if (temp.empty())
        return cv::Mat();
    
    return temp;
}

bool ImageLoader::saveImage(std::string path, cv::Mat& image)
{
    // Checks if there is any output image data to save
    if (image.empty())
        return false;

    bool saved = false;
    // Stores output image at the chosen path
    try {
        saved = cv::imwrite(path, image);
    }
    catch (cv::Exception& ex) {
        qInfo() << ex.what();
        return false;
    }
    return saved;
}

cv::Mat ImageLoader::getImageDialog(QWidget* parent)
{
    // Opens a file explorer and gets a path of the chosen image
    QString fileName = QFileDialog::getOpenFileName(parent, QString("Wybierz obraz"),
        "/home",
        QString("Images (*.png *.jpg *.jpeg *.bmp)"));

    qInfo() << fileName;

    // Cancels open action, if user clicked "cancel" and the path is null
    if (fileName.isNull() || fileName.isEmpty())
    {
        QMessageBox::information(parent, "Nie wybrano obrazu do otworzenia",
            "Nie wybrano obrazu wejsciowego do otworzenia. Musisz wybrac jakis obraz wejsciowy, aby go wyswietlic.");
        return cv::Mat();
    }

    // Asks user to provide path for new source image and loads it
    cv::Mat temp = this->readInImage(fileName.toStdString());

    // Aborts opening and showing image operation, if new image hasn't been loaded succesfully
    if (temp.empty())
    {
        QMessageBox::information(parent, "Nie pozyskano danych obrazu",
            "Nie mozna bylo pozyskac danych obrazu. Upewnij sie, ze podany plik zawiera dane obrazu i ze masz do niego odpowiednie pozwolenia.");
        return cv::Mat();
    }

    return temp;
}

bool ImageLoader::saveImageDialog(QWidget* parent, cv::Mat& image)
{
    // Checks if there is any output image data to save
    if (image.empty())
    {
        QMessageBox::information(parent, "Brak obrazu do zapisania",
            "Nie ma obrazu wyjsciowego do zapisania. Musisz zaladowac najpierw obraz wejsciowy.");
        return false;
    }

    // Opens a file explorer and gets a path of the location chosen to store out image (with a filename)
    QString fileName = QFileDialog::getSaveFileName(parent, QString("Zapisz obraz"),
        "/home/untitled.png",
        QString("Images (*.png *.jpg *.jpeg *.bmp)"));

    qInfo() << fileName;

    // Cancels save action, if user clicked "cancel" and the path is null
    if (fileName.isNull() || fileName.isEmpty())
    {
        QMessageBox::information(parent, "Nie wybrano sciezki do zapisania",
            "Nie wybrano sciezki do zapisania obrazu. Musisz podac sciezke wraz z nazwa pliku obrazu, aby go zapisac.");
        return false;
    }

    // Tries to save image at path chosen by user
    bool saved = this->saveImage(fileName.toStdString(), image);

    if (!saved)
    {
        QMessageBox::critical(parent, "Zapisywanie sie nie powiodlo",
            "Zapisywanie obrazu sie nie powiodlo. Sprobuj zapisac obraz w innym formacie.");
    }

    return saved;
}

