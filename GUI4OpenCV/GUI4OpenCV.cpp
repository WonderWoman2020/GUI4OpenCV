#include "GUI4OpenCV.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

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

}

GUI4OpenCV::~GUI4OpenCV()
{
    delete ui;
}
