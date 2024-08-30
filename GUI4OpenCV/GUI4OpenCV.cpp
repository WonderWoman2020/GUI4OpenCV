#include "GUI4OpenCV.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

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

    QGraphicsScene* srcScene = new QGraphicsScene(this);
    ui->srcImageView->setScene(srcScene);
    QPixmap srcImage("mewa.jpg");
    srcScene->addPixmap(srcImage);
    qDebug() << srcScene->items().count();

    QGraphicsScene* outScene = new QGraphicsScene(this);
    ui->outImageView->setScene(outScene);
    QPixmap outImage("mewaGrey.jpg");
    outScene->addPixmap(outImage);
    qDebug() << outScene->items().count();


}

GUI4OpenCV::~GUI4OpenCV()
{
    delete ui;
}

/*void GUI4OpenCV::on_pushButton_clicked()
{
    ui->label_3->setText("Nacisnieto");
}*/