#pragma once

#include <QtWidgets/QMainWindow>
#include <QPixmap>
#include <QFileDialog>
#include <QScrollBar>
#include <QGraphicsScene>
#include <QMessageBox>

#include<QCursor>

#include "ui_GUI4OpenCV.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "ImageConverter.h"
#include "HistogramCalculator.h"
#include "ImageViewHandler.h"

#include "StructuringMatrixWindow.h"
#include "FilterMatrixWindow.h"
#include "AlphaSliderWindow.h"
#include "SecondImageWindow.h"

#include "ImageLoader.h"
#include "DebugPrintSettings.h"

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
    void on_actionAlfaChanging_triggered();
    void on_actionStructuringMatrix_triggered();
    void on_actionFilterMatrix_triggered();

    void freeSecondImageMemory();

    void mixImages(int alpha);
    void executeStructuringMatrixAlgorithm(std::vector<std::vector<bool>> matrixData, std::pair<int, int> characteristicElement, StructuringAlgorithm algorithm);
    void executeFilterMatrixAlgorithm(std::vector<std::vector<int>> matrixData, int divisor, FilterAlgorithm algorithm);

signals:
    void srcImageChanged();
    void outImageChanged();
    void srcSecondImageLoaded();

private:
    Ui::GUI4OpenCVClass *ui;

    HistogramCalculator* histogramCalculator;
    ImageViewHandler* imageViewHandler;

    ImageLoader* imageLoader;
    DebugPrintSettings* debugSettings;

    cv::Mat srcImage;
    cv::Mat outImage;

    cv::Mat srcSecondImage;
    int secondImageCounter;
    cv::Mat srcSecondImageResized;

    std::vector<cv::Mat> srcHistograms;
    std::vector<cv::Mat> outHistograms;

    cv::Mat srcHistogramImage;
    cv::Mat outHistogramImage;

    void onImageChanged();
    void onHistogramChanged();
    bool openSecondSourceImage(QGraphicsView* imageView);
};
