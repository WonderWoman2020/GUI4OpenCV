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

void calculateHistogram(cv::Mat& image)
{
    std::vector<cv::Mat> bgrPlanes;
    cv::split(image, bgrPlanes);
    cv::Mat bHist;
    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange[] = {range};
    cv::calcHist(&bgrPlanes.at(0), 1, 0, cv::Mat(), bHist, 1, &histSize, histRange);

    int hist_w = 256, hist_h = 200;
    int bin_w = cvRound((double)hist_w / histSize);

    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::normalize(bHist, bHist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
    for (int i = 1; i < histSize; i++)
    {
        line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(bHist.at<float>(i - 1))),
            cv::Point(bin_w * (i), hist_h - cvRound(bHist.at<float>(i))),
            cv::Scalar(255, 0, 0), 2, 8, 0);
    }
    cv::imshow("Source image", image);
    cv::imshow("calcHist Demo", histImage);
    cv::waitKey();

}

/*void GUI4OpenCV::setHistogramChartInView(QWidget* chartView, std::vector<short>& histogramValues)
{
    
}*/


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
        calculateHistogram(this->srcImage);
    }
    catch (std::exception& ex)
    {
        QMessageBox::critical(this, "Blad interfejsu",
            "Nie udalo sie zaladowac obrazu do interfejsu. Obraz zostal zaldadowany do pamieci, ale nastapil nieoczekiwany blad w dzialaniu interfejsu.");
    }
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