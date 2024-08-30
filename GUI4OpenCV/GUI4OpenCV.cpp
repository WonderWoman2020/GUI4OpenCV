#include "GUI4OpenCV.h"


GUI4OpenCV::GUI4OpenCV(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GUI4OpenCVClass())
{
    ui->setupUi(this);

    const QString debugMessagePattern = "Qt at [%{time h:mm:ss.zzz}], %{type}: %{message}";

    qSetMessagePattern(debugMessagePattern);
    qDebug() << "Debug info";
    qInfo() << "Info only";
    qCritical() << "Critical info";


    cv::Mat img;
    img = cv::imread("mewa.jpg");
    if (!img.data)
        cv::namedWindow("No image data");
    else
    {
        cv::namedWindow("Test window");
        cv::imshow("Test window", img);
    }

    /*QPixmap pm("mewa.jpg");
    ui->label->setPixmap(pm);

    cv::Mat greyImg;
    cv::cvtColor(img, greyImg, cv::COLOR_BGR2GRAY);
    cv::imwrite("mewaGrey.jpg", greyImg);

    QPixmap pm2("mewaGrey.jpg");
    ui->label_2->setPixmap(pm2);*/

    QGraphicsScene* outScene = new QGraphicsScene(this);
    ui->outImageView->setScene(outScene);
    QPixmap outImage("mewaGrey.jpg");
    outScene->addPixmap(outImage);
    qDebug() << outScene->items().count();

    // Connecting horizontal scroll bars to be in sync, when using either one of them
    connect(ui->srcImageView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
        ui->outImageView->horizontalScrollBar(), SLOT(setValue(int)));
    connect(ui->outImageView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
        ui->srcImageView->horizontalScrollBar(), SLOT(setValue(int)));

    // Same thing for vertical scroll bars
    connect(ui->srcImageView->verticalScrollBar(), SIGNAL(valueChanged(int)),
        ui->outImageView->verticalScrollBar(), SLOT(setValue(int)));
    connect(ui->outImageView->verticalScrollBar(), SIGNAL(valueChanged(int)),
        ui->srcImageView->verticalScrollBar(), SLOT(setValue(int)));
}

GUI4OpenCV::~GUI4OpenCV()
{
    delete ui;
}

/*void GUI4OpenCV::on_pushButton_clicked()
{
    ui->label_3->setText("Nacisnieto");
}*/

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
    qDebug() << srcScene->items().count();
}

void GUI4OpenCV::on_actionZapisz_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
        "/home/jana/untitled.png",
        tr("Images (*.png *.xpm *.jpg)"));

    qInfo() << fileName;
}