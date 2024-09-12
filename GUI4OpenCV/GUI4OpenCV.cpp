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
    cv::Mat temp = this->readInImage();
    if (temp.empty())
        return;

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

cv::Mat GUI4OpenCV::readInImage()
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
        return cv::Mat();
    }

    // Tries to read the chosen file
    cv::Mat temp = cv::imread(fileName.toStdString());
    if (temp.empty())
    {
        QMessageBox::information(this, "Nie pozyskano danych obrazu",
            "Nie mozna bylo pozyskac danych obrazu. Upewnij sie, ze podany plik zawiera dane obrazu i ze masz do niego odpowiednie pozwolenia.");
        temp.release();
        return cv::Mat();
    }
    return temp;
}


QWidget* GUI4OpenCV::buildEmptyWindow(QWidget* parent, QSize size, Qt::WindowModality modality)
{
    QWidget* secondImageWindow = new QWidget(parent, Qt::Window);
    QGridLayout* imageWindowlayout = new QGridLayout(secondImageWindow);
    secondImageWindow->setLayout(imageWindowlayout);
    secondImageWindow->resize(size);
    secondImageWindow->setWindowModality(modality);    // Sets other windows to still be accessible, when this window shows up
    return secondImageWindow;
}

QWidget* GUI4OpenCV::openSecondSourceImage()
{
    // Lets user choose the second source image and loads it
    cv::Mat temp = this->readInImage();
    // Cancels alpha linear blending operation, if imaga data hasn't been loaded
    if (temp.empty())
    {
        QMessageBox::information(this, "Nie pozyskano drugiego obrazu",
            "Nie mozna przeprowadzic mieszania obrazow, poniewaz nie zaladowano drugiego obrazu. Wybierz drugi obraz do operacji lub sprobuj zaladowac inny obraz.");
        temp.release();
        return nullptr;
    }

    // Releases previous image data if any and assigns newly loaded image data (assign operator '=' for cv::Mat operands handles that)
    this->srcSecondImage = temp;
    this->srcSecondImageResized.release();
    qInfo() << "Freed memory of second source image and loaded a new one";
    // Sets image id, so windows using this image can be assigned to this image only
    this->secondImageCounter = (this->secondImageCounter + 1) % INT32_MAX;
    // Sends signal that new second source image has been loaded, so things related to previous image can act accordingly (destroy themselves)
    emit this->srcSecondImageLoaded();

    // Builds window for showing second source image
    QWidget* secondImageWindow = this->buildEmptyWindow(this, this->ui->srcImageView->size(), Qt::NonModal);
    secondImageWindow->setAttribute(Qt::WA_DeleteOnClose);    // Makes window destroy itself, when closed
    secondImageWindow->setProperty("imageID", this->secondImageCounter);    // Relates window to the image it will be showing
    // Adds image view widget to the window
    QGraphicsView* secondImageView = new QGraphicsView(secondImageWindow);
    secondImageWindow->layout()->addWidget(secondImageView);
    secondImageWindow->show();

    // Makes singal - slot connection, which destroys second source image window, when new image was loaded
    connect(this, SIGNAL(srcSecondImageLoaded()), secondImageWindow, SLOT(deleteLater()));

    // Adds image to the second source image view 
    try {
        this->imageViewHandler->setImageInView(secondImageView, ImageConverter::convertMatToQPixmap(this->srcSecondImage));
    }
    catch (std::exception& ex)
    {
        QMessageBox::critical(this, "Blad interfejsu",
            "Nie udalo sie zaladowac obrazu do interfejsu. Obraz zostal zaldadowany do pamieci, ale nastapil nieoczekiwany blad w dzialaniu interfejsu.");
        return nullptr;
    }

    return secondImageWindow;
}

void GUI4OpenCV::on_actionAlfaChanging_triggered()
{   
    // Informs that second source image needs to be chosen
    QMessageBox::information(this, "Mieszanie obrazow - wymagany drugi obraz",
        "Na potrzeby tej operacji nalezy wczytac drugi obraz. Wybierz drugi obraz do wczytania w nastepnym oknie.");

    QWidget* secondImageWindow = this->openSecondSourceImage();
    if (secondImageWindow == nullptr)
        return;

    // Builds a window for the alpha slider widget
    AlphaSlider* slider = new AlphaSlider();
    QWidget* widget = this->buildEmptyWindow(this, QSize(slider->width() + 50, slider->height() + 50), Qt::NonModal);
    widget->setAttribute(Qt::WA_DeleteOnClose);    // Makes window destroy itself, when closed
    widget->setProperty("imageID", this->secondImageCounter);    // Relates window to the image it will be using for alpha linear blending
    // Adds alpha slider widget to the window
    slider->setParent(widget);
    widget->layout()->addWidget(slider);
    widget->show();

    // Connects a method to execute alpha linear blending on images, when slider value changes
    connect(slider, SIGNAL(sliderValueChanged(int)), this, SLOT(mixImages(int)));

    // Destroying windows signals

    // Makes singal - slot connection, which destroys alpha slider window, when new image was loaded
    connect(this, SIGNAL(srcSecondImageLoaded()), widget, SLOT(deleteLater()));
    // Makes singal - slot connection, which destroys second window, if one of them (alpha slider window
    // or second source image window) is closed
    connect(widget, SIGNAL(destroyed()), secondImageWindow, SLOT(deleteLater()));
    connect(secondImageWindow, SIGNAL(destroyed()), widget, SLOT(deleteLater()));
    // Makes singal - slot connection, which frees memory of the second source image, when it won't
    // be used anymore - when one of the windows (alpha slider window or second source image window) is closed
    connect(widget, SIGNAL(destroyed()), this, SLOT(freeSecondImageMemory()));
}

void GUI4OpenCV::freeSecondImageMemory()
{
    QObject* sender = this->sender();
    QVariant imageIDProperty = sender->property("imageID");
    if (imageIDProperty.canConvert(QMetaType(QMetaType::Int)))
    {
        int imageID = imageIDProperty.toInt();
        // Frees memory of the second source image only, if sender is a window related to this image (containted it or used it).
        // If a new image is already loaded, that isn't related to the sender, it shouldn't be freed.
        if (this->secondImageCounter == imageID)
        {
            this->srcSecondImage.release();
            this->srcSecondImageResized.release();
            qInfo() << "Freed memory of second source image";
        }
    }
}

void GUI4OpenCV::mixImages(int alpha)
{
    if (this->srcImage.empty() || this->srcSecondImage.empty())
    {
        QMessageBox::information(this, "Nie pozyskano dwoch obrazow",
            "Nie mozna przeprowadzic mieszania obrazow, poniewaz nie zaladowano dwoch obrazow. Zaladuj obraz zarowno w oknie glownym, jak i w oknie opcji mieszania obrazow.");
        return;
    }

    try {
        cv::Mat srcSecondResized;
        if (this->srcSecondImageResized.rows == this->srcImage.rows && this->srcSecondImageResized.cols == this->srcImage.cols)
            srcSecondResized = this->srcSecondImageResized;
        else
        {
            cv::resize(this->srcSecondImage, srcSecondResized, cv::Size(this->srcImage.cols, this->srcImage.rows));
            this->srcSecondImageResized = srcSecondResized;
            qInfo() << "Resized second source image to be the same size as the first one";
        }
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

void GUI4OpenCV::on_actionStructuringMatrix_triggered()
{
    /*connect(okButton, SIGNAL(clicked()), this, SLOT(executeStructuringMatrixAlgorithm()));*/

    StructuringMatrixWindow* window = new StructuringMatrixWindow(this);
    connect(window, SIGNAL(sendInputData(std::vector<std::vector<bool>>, std::pair<int, int>, std::string)),
        this, SLOT(executeStructuringMatrixAlgorithm(std::vector<std::vector<bool>>, std::pair<int, int>, std::string)));
    window->show();
}

void GUI4OpenCV::executeStructuringMatrixAlgorithm(std::vector<std::vector<bool>> matrixData, std::pair<int, int> characteristicElement, std::string algorithm)
{
    qInfo() << "Execution of chosen algorithm that uses structuring matrix.";
    qInfo() << matrixData.at(0);
    qInfo() << characteristicElement;
    qInfo() << algorithm;
}

void GUI4OpenCV::on_actionFilterMatrix_triggered()
{
    FilterMatrixWindow* window = new FilterMatrixWindow(this);
    connect(window, SIGNAL(sendInputData(std::vector<std::vector<int>>, int, std::string)),
        this, SLOT(executeFilterMatrixAlgorithm(std::vector<std::vector<int>>, int, std::string)));
    window->show();
}

void GUI4OpenCV::executeFilterMatrixAlgorithm(std::vector<std::vector<int>> matrixData, int divisor, std::string algorithm)
{
    qInfo() << "Execution of chosen algorithm that uses filter matrix.";
    qInfo() << matrixData.at(0);
    qInfo() << divisor;
    qInfo() << algorithm;
}