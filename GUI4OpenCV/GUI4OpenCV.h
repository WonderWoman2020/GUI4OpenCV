#pragma once

#include <QtWidgets/QMainWindow>
#include <QPixmap>
#include <QFileDialog>
#include <QScrollBar>
#include <QGraphicsScene>
#include <QMessageBox>

#include <QCursor>

#include "ui_GUI4OpenCV.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "ImageConverter.h"
#include "ImageLoader.h"
#include "DebugPrintSettings.h"
#include "HistogramCalculator.h"
#include "ImageViewHandler.h"

#include "StructuringMatrixWindow.h"
#include "FilterMatrixWindow.h"
#include "AlphaSliderWindow.h"
#include "SecondImageWindow.h"

#include "AlphaOperationWindows.h"

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

    void on_actionAboutApp_triggered();
    void on_actionAboutQt_triggered();

    void on_actionCursorTest_triggered();
    void on_actionAlphaChanging_triggered();
    void on_actionStructuringMatrix_triggered();
    void on_actionFilterMatrix_triggered();

    void receiveProcessingResult(cv::Mat& result);

signals:
    void srcImageChanged(cv::Mat& src1);
    void outImageChanged(cv::Mat& out);
    void srcSecondImageLoaded();

private:
    Ui::GUI4OpenCVClass *ui;

    HistogramCalculator* histogramCalculator;
    ImageViewHandler* imageViewHandler;
    ImageLoader* imageLoader;
    DebugPrintSettings* debugSettings;

    cv::Mat srcImage;
    cv::Mat outImage;

    std::vector<cv::Mat> srcHistograms;
    std::vector<cv::Mat> outHistograms;

    cv::Mat srcHistogramImage;
    cv::Mat outHistogramImage;

    void onSrcImageChanged();
    void onOutImageChanged();
    void onHistogramChanged(bool srcHistogramChanged);
};
