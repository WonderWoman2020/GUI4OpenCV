#include "GUI4OpenCV.h"


GUI4OpenCV::GUI4OpenCV(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GUI4OpenCVClass())
{
    // This line calls Qt's MOC (Meta-Object Compiler) to analyze code of this class.
    // MOC is a preprocessor, which analyzes any Qt related code and macros (which are not
    // understandable for standard C++ compiler), and generates a new file, that contains only
    // pure C++ code. The generated file for this class would be named as "moc_GUI4OpenCV.cpp".
    // MOC also analyzes method names, by using reflection mechanism, and creates some signal-slot
    // connections automatically for us, if we name method in specific format - for example:
    // Method "on_actionSync_triggered()"
    // - actionSync is a sender,
    // - triggered is its signal,
    // - this object is a receiver,
    // - the method is a receiver's slot, which reacts to the signal.
    ui->setupUi(this);
 
    this->histogramCalculator = new HistogramCalculator();
    this->imageViewHandler = new ImageViewHandler();
    this->imageLoader = new ImageLoader();

    // Sets debug format, in which messages are printed out in the console
    this->setDebugPrintingPatterns();

    // Sets scrolls (present in image views) to move in sync by default
    this->on_actionSync_triggered();

    // Makes signal-slot connection, so image views and histograms will update themselves, when new image is loaded
    connect(this, &GUI4OpenCV::srcImageChanged, this, &GUI4OpenCV::onImageChanged);
}

GUI4OpenCV::~GUI4OpenCV()
{
    // Deletes the window's ui root object and all it's child elements
    delete ui;

    delete this->histogramCalculator;
    delete this->imageViewHandler;

    // Frees memory of loaded images if any
    this->srcImage.release();
    this->outImage.release();
    this->srcSecondImage.release();
    this->srcSecondImageResized.release();

    // Frees memory of other images and cv::Mat objects generated by app, that were stored in-memory
    this->srcHistogramImage.release();
    this->outHistogramImage.release();
    this->srcHistograms.clear();    // std::vector::clear() calls destructor on each element it contains
    this->outHistograms.clear();
}

/*
    Sets how debug information will be printed out when using qDebug(), qInfo() and similar functions.
    This pattern is only set for the scope of this window (and its children) - for another window debugging,
    one should add a function like this in the desired window as well.
*/
void GUI4OpenCV::setDebugPrintingPatterns()
{
    // Sets the format of debug messages
    const QString debugMessagePattern = "Qt at [%{time h:mm:ss.zzz}], %{type}: %{message}";
    qSetMessagePattern(debugMessagePattern);
    
    // Some examples, that can be seen in console
    qDebug() << "Debug message";
    qInfo() << "Info message";
    qCritical() << "Critical message";
}

bool GUI4OpenCV::updateImageView(QGraphicsView* imageView, cv::Mat& image)
{
    if (image.empty())
        return false;

    try {
        this->imageViewHandler->setImageInView(imageView, ImageConverter::convertMatToQPixmap(image));
    }
    catch (std::exception& ex)
    {
        QMessageBox::critical(this, "Blad interfejsu",
            "Nie udalo sie zaladowac obrazu do interfejsu. Obraz zostal zaldadowany do pamieci, ale nastapil nieoczekiwany blad w dzialaniu interfejsu.");
        return false;
    }

    return true;
}

/*
    Handles syncing and desyncing images scrolls action.
*/
void GUI4OpenCV::on_actionSync_triggered()
{
    bool sync = ui->actionSync->isChecked();
    this->imageViewHandler->syncViewsScrollBars(ui->srcImageView, ui->outImageView, sync);
}

/*
    Handles opening source image action.
*/
void GUI4OpenCV::on_actionOpen_triggered()
{
    cv::Mat temp = this->imageLoader->getImageDialog(this);

    // Frees memory of previous loaded source image (and its processed copy in output image)
    this->srcImage.release();
    this->outImage.release();

    // Stores in memory both source image and a copy of it in output image
    this->srcImage = temp;
    this->srcImage.copyTo(this->outImage);

    // Updates both source and processed image views
    this->updateImageView(ui->srcImageView, this->srcImage);
    this->updateImageView(ui->outImageView, this->outImage);

    // Notifies other components, that new image was loaded, so they can update themselves
    emit this->srcImageChanged();
}

/*
    Handles saving processed image action.
*/
void GUI4OpenCV::on_actionSave_triggered()
{
    bool saved = this->imageLoader->saveImageDialog(this, this->outImage);
}

void GUI4OpenCV::onImageChanged()
{
    // Calculates all histograms
    this->srcHistograms = this->histogramCalculator->calculateHistograms(this->srcImage);
    this->outHistograms = this->histogramCalculator->calculateHistograms(this->outImage);

    this->onHistogramChanged();
}

void GUI4OpenCV::onHistogramChanged()
{
    // Fetches histogram color spaces chosen to show in charts
    bool histB = ui->actionHistB->isChecked();
    bool histG = ui->actionHistG->isChecked();
    bool histR = ui->actionHistR->isChecked();
    bool histGrayscale = ui->actionHistGrayscale->isChecked();

    // Draws histograms and sets them in the histogram views
    this->srcHistogramImage = this->histogramCalculator->drawChosenHistograms(this->srcHistograms, histB, histG, histR, histGrayscale);
    this->updateImageView(ui->srcHistView, this->srcHistogramImage);

    this->outHistogramImage = this->histogramCalculator->drawChosenHistograms(this->outHistograms, histB, histG, histR, histGrayscale);
    this->updateImageView(ui->outHistView, this->outHistogramImage);
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



bool GUI4OpenCV::openSecondSourceImage(QGraphicsView* imageView)
{
    cv::Mat temp = this->imageLoader->getImageDialog(this);

    // Cancels alpha linear blending operation, if imaga data hasn't been loaded
    if (temp.empty())
        return false;

    // Releases previous image data if any and assigns newly loaded image data
    this->srcSecondImage = temp;
    this->srcSecondImageResized.release();

    qInfo() << "Freed memory of second source image and loaded a new one";

    // Sets image id, so windows using this image can be assigned to this image only
    this->secondImageCounter = (this->secondImageCounter + 1) % INT32_MAX;

    // Sends signal that new second source image has been loaded, so things related to previous image can act accordingly (destroy themselves)
    emit this->srcSecondImageLoaded();

    // Adds image to the second source image view
    bool updated = this->updateImageView(imageView, this->srcSecondImage);
    if (!updated)
    {
        this->srcSecondImage.release();
        return false;
    }

    return true;
}

void GUI4OpenCV::on_actionAlfaChanging_triggered()
{   
    // Informs that second source image needs to be chosen
    QMessageBox::information(this, "Mieszanie obrazow - wymagany drugi obraz",
        "Na potrzeby tej operacji nalezy wczytac drugi obraz. Wybierz drugi obraz do wczytania w nastepnym oknie.");

    // Builds window for showing second source image
    SecondImageWindow* imageWindow = new SecondImageWindow(this, this->secondImageCounter);
    imageWindow->resize(this->ui->srcImageView->size());
    // Loads second source image
    bool opened = this->openSecondSourceImage(imageWindow->getImageView());
    if (!opened)
    {
        imageWindow->deleteLater();
        return;
    }
    imageWindow->show();

    // Makes singal - slot connection, which destroys second source image window, when new image was loaded
    connect(this, SIGNAL(srcSecondImageLoaded()), imageWindow, SLOT(deleteLater()));

    // Builds a window for the alpha slider widget
    AlphaSliderWindow* alphaWindow = new AlphaSliderWindow(this, this->secondImageCounter);
    alphaWindow->show();

    // Connects a method to execute alpha linear blending on images, when slider value changes
    connect(alphaWindow, SIGNAL(sendInputData(int)), this, SLOT(mixImages(int)));

    // Destroying windows signals

    // Makes singal - slot connection, which destroys alpha slider window, when new image was loaded
    connect(this, SIGNAL(srcSecondImageLoaded()), alphaWindow, SLOT(deleteLater()));
    // Makes singal - slot connection, which destroys second window, if one of them (alpha slider window
    // or second source image window) is closed
    connect(alphaWindow, SIGNAL(destroyed()), imageWindow, SLOT(deleteLater()));
    connect(imageWindow, SIGNAL(destroyed()), alphaWindow, SLOT(deleteLater()));
    // Makes singal - slot connection, which frees memory of the second source image, when it won't
    // be used anymore - when one of the windows (alpha slider window or second source image window) is closed
    connect(alphaWindow, SIGNAL(destroyed()), this, SLOT(freeSecondImageMemory()));
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
        this->updateImageView(ui->outImageView, this->outImage);
        emit srcImageChanged();
    }
    catch (std::exception ex)
    {
        qInfo() << "Wyjatek:" << ex.what();
    }
}

void GUI4OpenCV::on_actionStructuringMatrix_triggered()
{
    StructuringMatrixWindow* window = new StructuringMatrixWindow(this);
    connect(window, SIGNAL(sendInputData(std::vector<std::vector<bool>>, std::pair<int, int>, StructuringAlgorithm)),
        this, SLOT(executeStructuringMatrixAlgorithm(std::vector<std::vector<bool>>, std::pair<int, int>, StructuringAlgorithm)));
    window->show();
}

void GUI4OpenCV::executeStructuringMatrixAlgorithm(std::vector<std::vector<bool>> matrixData, std::pair<int, int> characteristicElement, StructuringAlgorithm algorithm)
{
    qInfo() << "Execution of chosen algorithm that uses structuring matrix.";
    qInfo() << matrixData.at(0);
    qInfo() << characteristicElement;
    qInfo() << algorithm;
}

void GUI4OpenCV::on_actionFilterMatrix_triggered()
{
    FilterMatrixWindow* window = new FilterMatrixWindow(this);
    connect(window, SIGNAL(sendInputData(std::vector<std::vector<int>>, int, FilterAlgorithm)),
        this, SLOT(executeFilterMatrixAlgorithm(std::vector<std::vector<int>>, int, FilterAlgorithm)));
    window->show();
}

void GUI4OpenCV::executeFilterMatrixAlgorithm(std::vector<std::vector<int>> matrixData, int divisor, FilterAlgorithm algorithm)
{
    qInfo() << "Execution of chosen algorithm that uses filter matrix.";
    qInfo() << matrixData.at(0);
    qInfo() << divisor;
    qInfo() << algorithm;
}