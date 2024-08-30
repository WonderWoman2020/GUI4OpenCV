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

    this->srcImage.release();
    this->outImage.release();
    this->srcImage = cv::imread(fileName.toStdString());
    this->srcImage.copyTo(this->outImage);

    QPixmap srcImagePix = ImageConverter::convertMatToQPixmap(this->srcImage);
    QGraphicsScene* srcScene = ui->srcImageView->scene();
    srcScene->clear();
    srcScene->addPixmap(srcImagePix);

    qInfo() << srcScene->items().count();

    QPixmap outImagePix = ImageConverter::convertMatToQPixmap(this->outImage);
    QGraphicsScene* outScene = ui->outImageView->scene();
    outScene->clear();
    outScene->addPixmap(outImagePix);
}

void GUI4OpenCV::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Zapisz obraz"),
        "/home/untitled.png",
        tr("Images (*.png *.jpg *.jpeg *.bmp)"));

    qInfo() << fileName;

    QPixmap outImagePix = ImageConverter::convertMatToQPixmap(this->outImage);
    outImagePix.save(fileName);
}