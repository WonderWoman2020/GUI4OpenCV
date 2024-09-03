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

    // Sets debug format for this window
    this->setDebugPrintingPatterns();

    // Sets scrolls to be in sync by default
    this->syncImagesScrollBars();

    connect(this, &GUI4OpenCV::srcImageChanged, this, &GUI4OpenCV::onImageChanged);
    //connect(this, &GUI4OpenCV::outImageChanged, this, &GUI4OpenCV::drawChosenHistograms);

}

GUI4OpenCV::~GUI4OpenCV()
{
    delete ui;

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
    Makes corresponding scroll bars of the source and out image views to move in sync.
*/
void GUI4OpenCV::syncImagesScrollBars()
{
    // Connecting horizontal scroll bars to be in sync, when using either one of them
    QScrollBar* srcHScroll = ui->srcImageView->horizontalScrollBar();
    QScrollBar* outHScroll = ui->outImageView->horizontalScrollBar();
    connect(srcHScroll, SIGNAL(valueChanged(int)), outHScroll, SLOT(setValue(int)));
    connect(outHScroll, SIGNAL(valueChanged(int)), srcHScroll, SLOT(setValue(int)));

    // Same thing for vertical scroll bars
    QScrollBar* srcVScroll = ui->srcImageView->verticalScrollBar();
    QScrollBar* outVScroll = ui->outImageView->verticalScrollBar();
    connect(srcVScroll, SIGNAL(valueChanged(int)), outVScroll, SLOT(setValue(int)));
    connect(outVScroll, SIGNAL(valueChanged(int)), srcVScroll, SLOT(setValue(int)));
}


/*
    Disables moving corresponding scroll bars of the source and out image views in sync.
*/
void GUI4OpenCV::desyncImagesScrollBars()
{
    // Disconnecting horizontal scroll bars
    QScrollBar* srcHScroll = ui->srcImageView->horizontalScrollBar();
    QScrollBar* outHScroll = ui->outImageView->horizontalScrollBar();
    disconnect(srcHScroll, SIGNAL(valueChanged(int)), outHScroll, SLOT(setValue(int)));
    disconnect(outHScroll, SIGNAL(valueChanged(int)), srcHScroll, SLOT(setValue(int)));

    // Same thing for vertical scroll bars
    QScrollBar* srcVScroll = ui->srcImageView->verticalScrollBar();
    QScrollBar* outVScroll = ui->outImageView->verticalScrollBar();
    disconnect(srcVScroll, SIGNAL(valueChanged(int)), outVScroll, SLOT(setValue(int)));
    disconnect(outVScroll, SIGNAL(valueChanged(int)), srcVScroll, SLOT(setValue(int)));
}

/*
    Sets image in the QGraphicsView's scene.
*/
void GUI4OpenCV::setImageInView(QGraphicsView* graphicsView, QPixmap image)
{
    if (graphicsView == nullptr)
        throw std::invalid_argument("Parameter 'QGraphicsView* graphicsView' was nullptr.");

    QGraphicsScene* scene = graphicsView->scene();
    if (scene == nullptr)
    {
        scene = new QGraphicsScene(graphicsView);    // Sets the view to be parent object for the scene, so memory of the scene object will be managed by parent
        graphicsView->setScene(scene);
        qInfo() << "New scene was created. It's parent is "+scene->parent()->objectName();
    }

    scene->clear();    // removes previous image, so images won't stack one on another
    scene->addPixmap(image);
    scene->setSceneRect(scene->itemsBoundingRect());    // resizes scene, so it is not larger than items (image in this case) it contains

    qInfo() << scene->items().count();
}

/*
    Calculates histogram of one color space provided and draws histogram chart as 'cv::Mat' image.
*/
cv::Mat GUI4OpenCV::calculateHistogram(cv::Mat& imagePlane, cv::Scalar histColor)
{
    // Calculates histogram of one color space of the image
    cv::Mat planeHist;
    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange[] = {range};
    cv::calcHist(&imagePlane, 1, 0, cv::Mat(), planeHist, 1, &histSize, histRange);

    // Normalizes histogram data to be 256x200
    int histH = 200;
    cv::normalize(planeHist, planeHist, 0, histH, cv::NORM_MINMAX, -1, cv::Mat());

    return planeHist;
}

void GUI4OpenCV::drawHistogram(cv::Mat& histogram, cv::Mat& histImage, int histW, int histH, cv::Scalar histColor)
{
    int histSize = 256;
    int bin_w = cvRound((double)histW / histSize);

    // Draws histogram chart as an 'cv::Mat' image
    for (int i = 1; i < histSize; i++)
    {
        cv::line(histImage, cv::Point(bin_w * (i - 1), histH - cvRound(histogram.at<float>(i - 1))),
            cv::Point(bin_w * (i), histH - cvRound(histogram.at<float>(i))),
            histColor, 2, 8, 0);
    }

    //return histImage;
}

/*
    Creates histogram charts as 'cv::Mat' images, of each BGR color spaces of the provided image, or of grayscale image.
    Histograms stored as 'cv::Mat' images are returned in 'std::vector', and their order is as follows:
    0 - B, 1 - G, 2 - R, or if image was in grayscale, then the vector contains just one histogram, so:
    0 - grayscale histogram.
*/
std::vector<cv::Mat> GUI4OpenCV::calculateHistograms(cv::Mat& image)
{
    // Splits image to planes with just one color space, B, G or R (or grayscale, if provided image has just one color space)
    std::vector<cv::Mat> bgrPlanes;
    cv::split(image, bgrPlanes);

    // Deletes alfa channel if present (not from orignal image, just from this copy), so it won't have histogram calculated
    if (bgrPlanes.size() > 3)
        bgrPlanes.erase(bgrPlanes.begin() + 3, bgrPlanes.end());

    // Just hardcoded color values, in which histograms will be drawn
    std::vector<cv::Scalar> colorSpaceColors = { 
        cv::Scalar(255, 0, 0),    // B
        cv::Scalar(0, 255, 0),    // G
        cv::Scalar(0, 0, 255),    // R
        cv::Scalar(127, 127, 127)    // gray
    };

    // Calculates histograms of each color space
    std::vector<cv::Mat> histograms;
    cv::Scalar histogramColor;
    for (int i = 0; i < bgrPlanes.size(); i++)
    {
        histogramColor = bgrPlanes.size() == 1 ? colorSpaceColors.back() : colorSpaceColors.at(i);    // Sets histogram color to gray or one of BGR colors
        histograms.push_back(this->calculateHistogram(bgrPlanes.at(i), histogramColor));
    }

    return histograms;
}

/*
    Handles syncing images scrolls action.
*/
void GUI4OpenCV::on_actionSync_triggered()
{
    if (ui->actionSync->isChecked())
        this->syncImagesScrollBars();
    else
        this->desyncImagesScrollBars();
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
        this->setImageInView(ui->srcImageView, ImageConverter::convertMatToQPixmap(this->srcImage));
        this->setImageInView(ui->outImageView, ImageConverter::convertMatToQPixmap(this->outImage));
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

void GUI4OpenCV::drawChosenHistograms()
{
    // Just hardcoded color values, in which histograms will be drawn
    std::vector<cv::Scalar> colorSpaceColors = {
        cv::Scalar(255, 0, 0),    // B
        cv::Scalar(0, 255, 0),    // G
        cv::Scalar(0, 0, 255),    // R
        cv::Scalar(127, 127, 127)    // gray
    };

    this->srcHistogramImage = cv::Mat(200, 256, CV_8UC3, cv::Scalar(255, 255, 255));
    this->outHistogramImage = cv::Mat(200, 256, CV_8UC3, cv::Scalar(255, 255, 255));

    //std::vector<cv::Mat> histogramsImages;

    // Draws source image histogram
    if (this->srcHistograms.size() == 1)
    {
        this->drawHistogram(this->srcHistograms.at(0), this->srcHistogramImage, 256, 200, colorSpaceColors.back());
        this->setImageInView(ui->srcHistView, ImageConverter::convertMatToQPixmap(this->srcHistogramImage));
    }
    else
    {
        if (ui->actionHistB->isChecked() && this->srcHistograms.size() > 0)
            this->drawHistogram(this->srcHistograms.at(0), this->srcHistogramImage, 256, 200, colorSpaceColors.at(0));

        if (ui->actionHistG->isChecked() && this->srcHistograms.size() > 1)
            this->drawHistogram(this->srcHistograms.at(1), this->srcHistogramImage, 256, 200, colorSpaceColors.at(1));

        if (ui->actionHistR->isChecked() && this->srcHistograms.size() > 2)
            this->drawHistogram(this->srcHistograms.at(2), this->srcHistogramImage, 256, 200, colorSpaceColors.at(2));

        this->setImageInView(ui->srcHistView, ImageConverter::convertMatToQPixmap(this->srcHistogramImage));
    }

    // Draws out image histogram
    if (this->outHistograms.size() == 1)
    {
        this->drawHistogram(this->outHistograms.at(0), this->outHistogramImage, 256, 200, colorSpaceColors.back());
        this->setImageInView(ui->outHistView, ImageConverter::convertMatToQPixmap(this->outHistogramImage));
    }
    else
    {
        if (ui->actionHistB->isChecked() && this->outHistograms.size() > 0)
            this->drawHistogram(this->outHistograms.at(0), this->outHistogramImage, 256, 200, colorSpaceColors.at(0));

        if (ui->actionHistG->isChecked() && this->outHistograms.size() > 1)
            this->drawHistogram(this->outHistograms.at(1), this->outHistogramImage, 256, 200, colorSpaceColors.at(1));

        if (ui->actionHistR->isChecked() && this->outHistograms.size() > 2)
            this->drawHistogram(this->outHistograms.at(2), this->outHistogramImage, 256, 200, colorSpaceColors.at(2));

        this->setImageInView(ui->outHistView, ImageConverter::convertMatToQPixmap(this->outHistogramImage));
    }
}

void GUI4OpenCV::onImageChanged()
{
    // Calculates all histograms
    this->srcHistograms = this->calculateHistograms(this->srcImage);
    this->outHistograms = this->calculateHistograms(this->outImage);
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