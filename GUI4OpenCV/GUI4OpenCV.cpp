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

    // Sets QGraphicsScenes in both QGraphicsViews - GUI elements for image viewing
    ui->srcImageView->setScene(&this->srcScene);
    ui->outImageView->setScene(&this->outScene);

    // Sets debug format for this window
    this->setDebugPrintingPatterns();

    // Sets scrolls to be in sync by default
    this->syncImagesScrollBars();
}

GUI4OpenCV::~GUI4OpenCV()
{
    delete ui;
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
    // Opens a file explorer and gets a path of the image chosen
    QString fileName = QFileDialog::getOpenFileName(this, tr("Wybierz obraz"),
        "/home",
        tr("Images (*.png *.jpg *.jpeg *.bmp)"));

    qInfo() << fileName;

    // Frees memory of previous loaded images if any
    this->srcImage.release();
    this->outImage.release();

    // Stores in memory both source image and a copy of it in out image
    this->srcImage = cv::imread(fileName.toStdString());
    this->srcImage.copyTo(this->outImage);

    // Adds source image as QPixmap to the source image view 
    QPixmap srcImagePix = ImageConverter::convertMatToQPixmap(this->srcImage);
    QGraphicsScene* srcScene = ui->srcImageView->scene();
    srcScene->clear();    // removes previous image, so images won't stack one on another
    srcScene->addPixmap(srcImagePix);

    qInfo() << srcScene->items().count();

    // Adds out image as QPixmap to the out image view 
    QPixmap outImagePix = ImageConverter::convertMatToQPixmap(this->outImage);
    QGraphicsScene* outScene = ui->outImageView->scene();
    outScene->clear();    // removes previous image, so images won't stack one on another
    outScene->addPixmap(outImagePix);
}

/*
    Handles saving processed image action.
*/
void GUI4OpenCV::on_actionSave_triggered()
{
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

    // Stores out image at the chosen path
    QPixmap outImagePix = ImageConverter::convertMatToQPixmap(this->outImage);
    outImagePix.save(fileName);
}