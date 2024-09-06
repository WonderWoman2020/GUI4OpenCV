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

void GUI4OpenCV::onImageChanged()
{
    // Calculates all histograms
    this->srcHistograms = this->histogramHandler->calculateHistograms(this->srcImage);
    this->outHistograms = this->histogramHandler->calculateHistograms(this->outImage);

    this->onHistogramChanged();
}

void GUI4OpenCV::onHistogramChanged()
{
    // Draws histograms and sets them in the histogram views
    this->srcHistogramImage = this->histogramHandler->drawChosenHistograms(this->srcHistograms,
        ui->actionHistB->isChecked(), ui->actionHistG->isChecked(), ui->actionHistR->isChecked(), ui->actionHistGrayscale->isChecked());
    this->imageViewHandler->setImageInView(ui->srcHistView, ImageConverter::convertMatToQPixmap(this->srcHistogramImage));

    this->outHistogramImage = this->histogramHandler->drawChosenHistograms(this->outHistograms,
        ui->actionHistB->isChecked(), ui->actionHistG->isChecked(), ui->actionHistR->isChecked(), ui->actionHistGrayscale->isChecked());
    this->imageViewHandler->setImageInView(ui->outHistView, ImageConverter::convertMatToQPixmap(this->outHistogramImage));
}

void GUI4OpenCV::on_actionHistB_triggered()
{
    this->onHistogramChanged();
}

void GUI4OpenCV::on_actionHistG_triggered()
{
    this->onHistogramChanged();
}

void GUI4OpenCV::on_actionHistR_triggered()
{
    this->onHistogramChanged();
}

void GUI4OpenCV::on_actionHistGrayscale_triggered()
{
    this->onHistogramChanged();
}

void GUI4OpenCV::on_actionAboutApp_triggered()
{
    QMessageBox::about(this, "O programie", QString("<b>Aplikacja</b>")+QString("<br/>Aplikacja zostala stworzona na potrzeby laboratorium z")+
        QString(" przetwarzania obrazow, w celu wizualizacji dzialania algorytmow.") + QString(
        "<br/><br/><b>Autor</b><br/>Nazwisko i imie"));
}

void GUI4OpenCV::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this, "O frameworku Qt");
}

void GUI4OpenCV::on_actionCursorTest_triggered()
{
    QMessageBox msgBox = QMessageBox(this);
    msgBox.setCursor(Qt::WaitCursor);
    msgBox.setWindowTitle("Test of cursor");
    msgBox.setText("Test to see if cursor changes.");
    msgBox.exec();
    msgBox.deleteLater();
    //this->setCursor(Qt::ArrowCursor);
}

void GUI4OpenCV::on_actionAlfaChanging_triggered()
{
    qInfo() << "Firstly, here will: 1. Ask for second image. 2. If image loaded to main window class field, there will be opened a window with an alfa slider only.";
    
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

    /*// Adds images to the source and processed image views 
    try {
        this->imageViewHandler->setImageInView(ui->srcImageView, ImageConverter::convertMatToQPixmap(this->srcImage));
    }
    catch (std::exception& ex)
    {
        QMessageBox::critical(this, "Blad interfejsu",
            "Nie udalo sie zaladowac obrazu do interfejsu. Obraz zostal zaldadowany do pamieci, ale nastapil nieoczekiwany blad w dzialaniu interfejsu.");
        return;
    }*/

    // Builds a window for alpha slider widget
    QWidget* widget = new QWidget(this, Qt::Window);
    QGridLayout* layout = new QGridLayout(widget);
    AlphaSlider* slider = new AlphaSlider(widget);
    widget->resize(slider->width()+50, slider->height()+50);
    layout->addWidget(slider);
    widget->setLayout(layout);
    widget->setWindowModality(Qt::NonModal);
    widget->show();

    // Connects a method to execute alpha linear blending on images, when slider value changes
    connect(slider, SIGNAL(sliderValueChanged(int)), this, SLOT(mixImages(int)));
}

void GUI4OpenCV::mixImages(int alpha)
{
    qInfo() << "Do stuff";
    try {
        //cv::Mat gray;
        //cv::cvtColor(this->srcImage, gray, cv::COLOR_BGR2GRAY);
        //cv::cvtColor(gray, gray, cv::COLOR_GRAY2BGR);
        cv::Mat srcSecondResized;
        cv::resize(this->srcSecondImage, srcSecondResized, cv::Size(this->srcImage.cols, this->srcImage.rows));
        cv::Mat result;
        double alphaNormalized = alpha / (double)255;
        double betaNormalized = 1.0 - alphaNormalized;
        cv::addWeighted(srcSecondResized, alphaNormalized, this->srcImage, betaNormalized, 0.0, result);
        this->outImage = result;
        this->imageViewHandler->setImageInView(ui->outImageView, ImageConverter::convertMatToQPixmap(this->outImage));
        emit srcImageChanged();
    }
    catch (std::exception ex)
    {
        qInfo() << "Wyjatek:" << ex.what();
    }
}