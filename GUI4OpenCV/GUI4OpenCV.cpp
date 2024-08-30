#include "GUI4OpenCV.h"


GUI4OpenCV::GUI4OpenCV(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GUI4OpenCVClass())
{
    ui->setupUi(this);

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

void GUI4OpenCV::on_actionWczytaj_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
        "/home",
        tr("Images (*.png *.xpm *.jpg)"));

    qInfo() << fileName;

    QGraphicsScene* srcScene = new QGraphicsScene(this);
    ui->srcImageView->setScene(srcScene);
    QPixmap srcImage(fileName);
    srcScene->addPixmap(srcImage);
    qInfo() << srcScene->items().count();
}

void GUI4OpenCV::on_actionZapisz_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
        "/home/jana/untitled.png",
        tr("Images (*.png *.xpm *.jpg)"));

    qInfo() << fileName;

    // TODO change for actual picture, add saving etc.
    QGraphicsScene* outScene = new QGraphicsScene(this);
    ui->outImageView->setScene(outScene);
    QPixmap outImage("mewaGrey.jpg");
    outScene->addPixmap(outImage);
    qInfo() << outScene->items().count();
}