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
#include "HistogramHandler.h"
#include "ImageViewHandler.h"

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

    void on_actionHistB_triggered();
    void on_actionHistG_triggered();
    void on_actionHistR_triggered();
    void on_actionHistGrayscale_triggered();

signals:
    void srcImageChanged();
    void outImageChanged();

private:
    Ui::GUI4OpenCVClass *ui;

    HistogramHandler* histogramHandler;
    ImageViewHandler* imageViewHandler;

    cv::Mat srcImage;
    cv::Mat outImage;

    std::vector<cv::Mat> srcHistograms;
    std::vector<cv::Mat> outHistograms;

    cv::Mat srcHistogramImage;
    cv::Mat outHistogramImage;

    void setDebugPrintingPatterns();
    void syncImagesScrollBars();
    void desyncImagesScrollBars();

    void drawChosenHistograms();

    void onImageChanged();
};
