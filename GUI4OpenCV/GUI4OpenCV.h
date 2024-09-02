#pragma once

#include <QtWidgets/QMainWindow>
#include <QPixmap>
#include <QFileDialog>
#include <QScrollBar>
#include <QGraphicsScene>
#include <QMessageBox>

#include "ui_GUI4OpenCV.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "ImageConverter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GUI4OpenCVClass; };
QT_END_NAMESPACE

class GUI4OpenCV : public QMainWindow
{
    Q_OBJECT

public:
    GUI4OpenCV(QWidget *parent = nullptr);
    ~GUI4OpenCV();

public slots:
    void on_actionSync_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();

private:
    Ui::GUI4OpenCVClass *ui;

    cv::Mat srcImage;
    cv::Mat outImage;

    std::vector<cv::Mat> srcHistograms;
    cv::Mat srcCurrentHistogram;
    std::vector<cv::Mat> outHistograms;

    void setDebugPrintingPatterns();
    void syncImagesScrollBars();
    void desyncImagesScrollBars();
    void setImageInView(QGraphicsView* graphicsView, QPixmap image);
    cv::Mat calculateHistogram(cv::Mat& imagePlane, cv::Scalar histColor);
    void drawHistogram(cv::Mat& histogram, cv::Mat& histImage, int histW, int histH, cv::Scalar histColor);
    std::vector<cv::Mat> createHistograms(cv::Mat& image);
};
