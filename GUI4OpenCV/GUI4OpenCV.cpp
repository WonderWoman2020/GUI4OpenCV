#include "GUI4OpenCV.h"


GUI4OpenCV::GUI4OpenCV(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GUI4OpenCVClass())
{
    // This is a part of Qt Meta-Object System. This runs MOC (Meta-Object Compiler)
    // which scans this class code and analyzes it with reflection mechanisms. MOC for example 
    // automatically creates connections of signals and slots from analyzing method names like "on_pushButton_clicked()".
    // It also generates a file, something like "moc_filename.cpp", that is in pure C++, with no Qt macros etc.,
    // so it can be processed further by C++ compiler.
    ui->setupUi(this);

    this->histogramHandler = new HistogramHandler();
    this->imageViewHandler = new ImageViewHandler();

    // Sets debug format for this window
    this->setDebugPrintingPatterns();

    // Sets scrolls to be in sync by default
    this->on_actionSync_triggered();

    connect(this, &GUI4OpenCV::srcImageChanged, this, &GUI4OpenCV::onImageChanged);
}

GUI4OpenCV::~GUI4OpenCV()
{
    delete ui;

    delete this->histogramHandler;
    delete this->imageViewHandler;

    // Frees memory of loaded images if any
    this->srcImage.release();
    this->outImage.release();
}

/*
    Sets how debug information will be printed out when using qDebug(), qInfo() and similar functions.
    This pattern is only set for the scope of this window - for another window debugging, one should add
    a function like this in the desired window as well.
*/
void GUI4OpenCV::setDebugPrintingPatterns()
{
    // Sets the format of debug messages, in which these will be printed in console
    const QString debugMessagePattern = "Qt at [%{time h:mm:ss.zzz}], %{type}: %{message}";
    qSetMessagePattern(debugMessagePattern);
    
    // Some examples, that can be seen in console
    qDebug() << "Debug info";
    qInfo() << "Info only";
    qCritical() << "Critical info";
}


/*
    Handles syncing images scrolls action.
*/
void GUI4OpenCV::on_actionSync_triggered()
{
    // Connecting horizontal scroll bars to be in sync, when using either one of them
    QScrollBar* srcHScroll = ui->srcImageView->horizontalScrollBar();
    QScrollBar* outHScroll = ui->outImageView->horizontalScrollBar();

    // Same thing for vertical scroll bars
    QScrollBar* srcVScroll = ui->srcImageView->verticalScrollBar();
    QScrollBar* outVScroll = ui->outImageView->verticalScrollBar();

    if (ui->actionSync->isChecked())
    {
        this->imageViewHandler->syncImagesScrollBars(srcHScroll, outHScroll);
        this->imageViewHandler->syncImagesScrollBars(srcVScroll, outVScroll);
    }
    else
    {
        this->imageViewHandler->desyncImagesScrollBars(srcHScroll, outHScroll);
        this->imageViewHandler->desyncImagesScrollBars(srcVScroll, outVScroll);
    }
}

/*
    Handles opening source image action.
*/
void GUI4OpenCV::on_actionOpen_triggered()
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

    // Frees memory of previous loaded images if any
    this->srcImage.release();
    this->outImage.release();

    // Stores in memory both source image and a copy of it in out image
    this->srcImage = temp;
    this->srcImage.copyTo(this->outImage);

    // Adds images to the source and processed image views 
    try {
        this->imageViewHandler->setImageInView(ui->srcImageView, ImageConverter::convertMatToQPixmap(this->srcImage));
        this->imageViewHandler->setImageInView(ui->outImageView, ImageConverter::convertMatToQPixmap(this->outImage));
    }
    catch (std::exception& ex)
    {
        QMessageBox::critical(this, "Blad interfejsu",
            "Nie udalo sie zaladowac obrazu do interfejsu. Obraz zostal zaldadowany do pamieci, ale nastapil nieoczekiwany blad w dzialaniu interfejsu.");
        return;
    }

    emit this->srcImageChanged();
}

/*
    Handles saving processed image action.
*/
void GUI4OpenCV::on_actionSave_triggered()
{
    // Checks if there is any out image data to save
    if (this->outImage.empty())
    {
        QMessageBox::information(this, "Brak obrazu do zapisania",
            "Nie ma obrazu wyjsciowego do zapisania. Musisz zaladowac najpierw obraz wejsciowy.");
        return;
    }

    // Opens a file explorer and gets a path of the location chosen to store out image (with a filename)
    QString fileName = QFileDialog::getSaveFileName(this, tr("Zapisz obraz"),
        "/home/untitled.png",
        tr("Images (*.png *.jpg *.jpeg *.bmp)"));

    qInfo() << fileName;

    // Cancels save action, if user clicked "cancel" and the path is null
    if (fileName.isNull() || fileName.isEmpty())
    {
        QMessageBox::information(this, "Nie wybrano sciezki do zapisania",
            "Nie wybrano sciezki do zapisania obrazu. Musisz podac sciezke wraz z nazwa pliku obrazu, aby go zapisac.");
        return;
    }

    // Stores out image at the chosen path
    bool saved = false;
    try {
        saved = cv::imwrite(fileName.toStdString(), this->outImage);
    }
    catch (cv::Exception& ex) {
        QMessageBox::critical(this, "Zapisywanie sie nie powiodlo",
            "Zapisywanie obrazu sie nie powiodlo. Wyjatek OpenCV: "+QString(ex.what())+QString("."));
        return;
    }
    if (!saved)
    {
        QMessageBox::critical(this, "Zapisywanie sie nie powiodlo",
            "Zapisywanie obrazu sie nie powiodlo. Sprobuj zapisac obraz w innym formacie.");
    }
}

/*
    Draws histograms and sets them in the histogram views.
*/
void GUI4OpenCV::drawChosenHistograms()
{
    // Just hardcoded color values, in which histograms will be drawn
    std::vector<cv::Scalar> colorSpaceColors = {
        cv::Scalar(255, 0, 0),    // B
        cv::Scalar(0, 255, 0),    // G
        cv::Scalar(0, 0, 255),    // R
        cv::Scalar(127, 127, 127)    // gray
    };

    // Prepares empty white 'cv::Mat' image, which is a canvas for the histogram chart
    this->srcHistogramImage = cv::Mat(200, 256, CV_8UC3, cv::Scalar(255, 255, 255));
    this->outHistogramImage = cv::Mat(200, 256, CV_8UC3, cv::Scalar(255, 255, 255));

    // Draws histograms of both source and out image
    for (int i = 0; i < 2; i++)
    {
        // Chooses which histogram to set to draw (source image histogram is created in the first for-loop iteration, then out image histogram)
        cv::Mat histogramImage = (i == 0 ? this->srcHistogramImage : this->outHistogramImage);
        std::vector<cv::Mat> histograms = (i == 0 ? this->srcHistograms : this->outHistograms);
        QGraphicsView* histView = (i == 0 ? ui->srcHistView : ui->outHistView);

        // Draws grayscale histogram
        if (histograms.size() == 1)
        {
            this->histogramHandler->drawHistogram(histograms.at(0), histogramImage, 256, 200, colorSpaceColors.back());
            this->imageViewHandler->setImageInView(histView, ImageConverter::convertMatToQPixmap(histogramImage));
            return;
        }

        // Draws BGR histograms, according to which color spaces have been chosen
        if (ui->actionHistB->isChecked() && histograms.size() > 0)
            this->histogramHandler->drawHistogram(histograms.at(0), histogramImage, 256, 200, colorSpaceColors.at(0));

        if (ui->actionHistG->isChecked() && histograms.size() > 1)
            this->histogramHandler->drawHistogram(histograms.at(1), histogramImage, 256, 200, colorSpaceColors.at(1));

        if (ui->actionHistR->isChecked() && histograms.size() > 2)
            this->histogramHandler->drawHistogram(histograms.at(2), histogramImage, 256, 200, colorSpaceColors.at(2));

        this->imageViewHandler->setImageInView(histView, ImageConverter::convertMatToQPixmap(histogramImage));
    }
}

void GUI4OpenCV::onImageChanged()
{
    // Calculates all histograms
    this->srcHistograms = this->histogramHandler->calculateHistograms(this->srcImage);
    this->outHistograms = this->histogramHandler->calculateHistograms(this->outImage);
    // Draws histograms and sets them in the histogram views
    this->drawChosenHistograms();
}

void GUI4OpenCV::on_actionHistB_triggered()
{
    this->drawChosenHistograms();
}

void GUI4OpenCV::on_actionHistG_triggered()
{
    this->drawChosenHistograms();
}

void GUI4OpenCV::on_actionHistR_triggered()
{
    this->drawChosenHistograms();
}

void GUI4OpenCV::on_actionHistGrayscale_triggered()
{
    this->drawChosenHistograms();
}