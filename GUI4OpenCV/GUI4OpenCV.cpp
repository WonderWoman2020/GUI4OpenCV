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

    cv::Mat img;
    img = cv::imread("mewa.jpg");
    if (!img.data)
    {
        cv::namedWindow("No image data");
    }
    cv::namedWindow("Test window");
    cv::imshow("Test window", img);

    QPixmap pm("mewa.jpg");
    ui->label->setPixmap(pm);
    
    cv::Mat greyImg;
    cv::cvtColor(img, greyImg, cv::COLOR_BGR2GRAY);
    cv::imwrite("mewaGrey.jpg", greyImg);

    QPixmap pm2("mewaGrey.jpg");
    ui->label_2->setPixmap(pm2);

    const QString debugMessagePattern = "Qt at [%{time h:mm:ss.zzz}], %{type}: %{message}";

    qSetMessagePattern(debugMessagePattern);
    qDebug() << "Debug info";
    qInfo() << "Info only";
    qCritical() << "Critical info";
}

GUI4OpenCV::~GUI4OpenCV()
{
    delete ui;
}

void GUI4OpenCV::on_pushButton_clicked()
{
    ui->label_3->setText("Nacisnieto");
}