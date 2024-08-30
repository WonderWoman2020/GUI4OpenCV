#include "GUI4OpenCV.h"


GUI4OpenCV::GUI4OpenCV(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GUI4OpenCVClass())
{
    ui->setupUi(this);

    ui->srcImageView->setScene(&this->srcScene);
    ui->outImageView->setScene(&this->outScene);

    this->setDebugPrintingPatterns();
    this->syncImagesScrollBars();
}

GUI4OpenCV::~GUI4OpenCV()
{
    delete ui;
}

void GUI4OpenCV::setDebugPrintingPatterns()
{
    const QString debugMessagePattern = "Qt at [%{time h:mm:ss.zzz}], %{type}: %{message}";
    qSetMessagePattern(debugMessagePattern);
    qDebug() << "Debug info";
    qInfo() << "Info only";
    qCritical() << "Critical info";

    // TODO delete later, openCV testing
    cv::Mat img;
    img = cv::imread("mewa.jpg");
    if (!img.data)
        cv::namedWindow("No image data");
    else
    {
        cv::namedWindow("Test window");
        cv::imshow("Test window", img);
    }
}

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

void GUI4OpenCV::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Wybierz obraz"),
        "/home",
        tr("Images (*.png *.jpg *.jpeg *.bmp)"));

    qInfo() << fileName;

    this->srcImage = cv::imread(fileName.toStdString());
    QPixmap srcImagePix = ImageConverter::convertMatToQPixmap(this->srcImage);

    QGraphicsScene* srcScene = ui->srcImageView->scene();
    srcScene->clear();
    srcScene->addPixmap(srcImagePix);

    qInfo() << srcScene->items().count();
}

void GUI4OpenCV::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Zapisz obraz"),
        "/home/untitled.png",
        tr("Images (*.png *.jpg *.jpeg *.bmp)"));

    qInfo() << fileName;

    // TODO change for actual picture, add saving etc.
    QGraphicsScene* outScene = new QGraphicsScene(this);
    ui->outImageView->setScene(outScene);
    QPixmap outImage("mewaGrey.jpg");
    outScene->addPixmap(outImage);
    qInfo() << outScene->items().count();
}