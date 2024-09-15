#include "GUI4OpenCV.h"


GUI4OpenCV::GUI4OpenCV(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GUI4OpenCVClass())
{
    // This line calls Qt's MOC (Meta-Object Compiler) to analyze code of this class.
    ui->setupUi(this);
 
    this->histogramCalculator = new HistogramCalculator();
    this->imageViewHandler = new ImageViewHandler();
    this->imageLoader = new ImageLoader();
    this->debugSettings = new DebugPrintSettings();

    // Sets debug format, in which messages are printed out in the console
    this->debugSettings->setDebugPrintingPatterns();

    // Sets scrolls (present in image views) to move in sync by default
    this->on_actionSync_triggered();

    // Makes signal-slot connection, so image views and histograms will update themselves, when new image is loaded
    connect(this, &GUI4OpenCV::srcImageChanged, this, &GUI4OpenCV::onSrcImageChanged);
    connect(this, &GUI4OpenCV::outImageChanged, this, &GUI4OpenCV::onOutImageChanged);
}

GUI4OpenCV::~GUI4OpenCV()
{
    // Deletes the window's ui root object and all it's child elements
    delete ui;

    delete this->histogramCalculator;
    delete this->imageViewHandler;
    delete this->imageLoader;
    delete this->debugSettings;

    // Frees memory of loaded images if any
    this->srcImage.release();
    this->outImage.release();

    // Frees memory of other images and cv::Mat objects generated by app, that were stored in-memory
    this->srcHistogramImage.release();
    this->outHistogramImage.release();
    this->srcHistograms.clear();    // std::vector::clear() calls destructor on each element it contains
    this->outHistograms.clear();
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
    if (temp.empty())
        return;
    // Stores in memory both source image and a copy of it in output image
    // (previously loaded images are released automatically by cv::Mat assign '=' operator)
    this->srcImage = temp;
    this->outImage = temp.clone();

    // Updates both source and processed image views
    this->imageViewHandler->updateImageView(this, ui->srcImageView, this->srcImage);
    this->imageViewHandler->updateImageView(this, ui->outImageView, this->outImage);

    // Notifies other components, that new image was loaded, so they can update themselves
    emit this->srcImageChanged(this->srcImage);
    emit this->outImageChanged(this->outImage);
}

/*
    Handles saving processed image action.
*/
void GUI4OpenCV::on_actionSave_triggered()
{
    bool saved = this->imageLoader->saveImageDialog(this, this->outImage);
}

void GUI4OpenCV::onSrcImageChanged()
{
    // Calculates changed image histograms
    this->srcHistograms = this->histogramCalculator->calculateHistograms(this->srcImage);
    
    this->onHistogramChanged(true);
}

void GUI4OpenCV::onOutImageChanged()
{
    // Calculates changed image histograms
    this->outHistograms = this->histogramCalculator->calculateHistograms(this->outImage);

    this->onHistogramChanged(false);
}

void GUI4OpenCV::onHistogramChanged(bool srcHistogramChanged)
{
    // Fetches histogram color spaces chosen to show in charts
    bool histB = ui->actionHistB->isChecked();
    bool histG = ui->actionHistG->isChecked();
    bool histR = ui->actionHistR->isChecked();
    bool histGrayscale = ui->actionHistGrayscale->isChecked();

    // Draws changed histogram and sets it in the histogram view
    if (srcHistogramChanged)
    {
        this->srcHistogramImage = this->histogramCalculator->drawChosenHistograms(this->srcHistograms, histB, histG, histR, histGrayscale);
        this->imageViewHandler->updateImageView(this, ui->srcHistView, this->srcHistogramImage);
    }
    else
    {
        this->outHistogramImage = this->histogramCalculator->drawChosenHistograms(this->outHistograms, histB, histG, histR, histGrayscale);
        this->imageViewHandler->updateImageView(this, ui->outHistView, this->outHistogramImage);
    }
}

void GUI4OpenCV::on_actionHistB_triggered()
{
    this->onHistogramChanged(true);
    this->onHistogramChanged(false);
}

void GUI4OpenCV::on_actionHistG_triggered()
{
    this->onHistogramChanged(true);
    this->onHistogramChanged(false);
}

void GUI4OpenCV::on_actionHistR_triggered()
{
    this->onHistogramChanged(true);
    this->onHistogramChanged(false);
}

void GUI4OpenCV::on_actionHistGrayscale_triggered()
{
    this->onHistogramChanged(true);
    this->onHistogramChanged(false);
}

void GUI4OpenCV::on_actionAboutApp_triggered()
{
    QString windowTitle = "O programie";
    QString appHeader = "<b>Aplikacja</b>";
    QString appDescription = "Aplikacja zostala stworzona na potrzeby laboratorium z przetwarzania obrazow, w celu wizualizacji dzialania algorytmow.";
    QString authorHeader = "<b>Autor</b>";
    QString authorName = "Nazwisko i imie";

    QString content = appHeader + "<br/>" +
        appDescription + "<br/><br/>" +
        authorHeader + "<br/>" + 
        authorName;

    QMessageBox::about(this, windowTitle, content);
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

void GUI4OpenCV::on_actionAlphaBlending_triggered()
{   
    AlphaBlendingController* alphaWindows = new AlphaBlendingController(this);
    alphaWindows->buildWindows();
    alphaWindows->showWindows();
    alphaWindows->setFirstSourceImage(this->srcImage);
    connect(this, SIGNAL(srcImageChanged(cv::Mat&)), alphaWindows, SLOT(setFirstSourceImage(cv::Mat&)));

    connect(alphaWindows, SIGNAL(sendResult(cv::Mat&)), this, SLOT(receiveProcessingResult(cv::Mat&)));
    connect(ui->actionAlphaBlending, SIGNAL(triggered()), alphaWindows, SLOT(deleteLater()));
}

void GUI4OpenCV::on_actionStructuringMatrix_triggered()
{
    StructuringMatrixWindow* window = new StructuringMatrixWindow(this);
    //connect(window, SIGNAL(sendInputData(std::vector<std::vector<bool>>, std::pair<int, int>, StructuringAlgorithm)),
    //    this, SLOT(executeStructuringMatrixAlgorithm(std::vector<std::vector<bool>>, std::pair<int, int>, StructuringAlgorithm)));
    window->show();
}

void GUI4OpenCV::on_actionFilterMatrix_triggered()
{
    FilterMatrixWindow* window = new FilterMatrixWindow(this);
    //connect(window, SIGNAL(sendInputData(std::vector<std::vector<int>>, int, FilterAlgorithm)),
    //    this, SLOT(executeFilterMatrixAlgorithm(std::vector<std::vector<int>>, int, FilterAlgorithm)));
    window->show();
}

void GUI4OpenCV::receiveProcessingResult(cv::Mat& result)
{
    if (result.empty())
        return;

    this->outImage = result;
    this->imageViewHandler->updateImageView(this, ui->outImageView, this->outImage);
    emit outImageChanged(this->outImage);
}
