#pragma once

#include <QtWidgets/QMainWindow>
#include <QPixmap>
#include <QFileDialog>
#include <QScrollBar>
#include <QGraphicsScene>
#include <QMessageBox>

#include<QCursor>
#include<QLineEdit>
#include <QValidator>
#include <QComboBox>
#include <QSpinBox>

#include "ui_GUI4OpenCV.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "ImageConverter.h"
#include "HistogramHandler.h"
#include "ImageViewHandler.h"

#include "AlphaSlider.h"

#include "StructuringMatrixButton.h"

#include "StructuringMatrix.h"

#include "StructuringMatrixWindow.h"

#include "FilterMatrix.h"

#include "FilterMatrixWindow.h"

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

    void mixImages(int alpha);

    void freeSecondImageMemory();

    void executeStructuringMatrixAlgorithm(std::vector<std::vector<bool>> matrixData, std::pair<int, int> characteristicElement, StructuringAlgorithm algorithm);
    
    void executeFilterMatrixAlgorithm(std::vector<std::vector<int>> matrixData, int divisor, FilterAlgorithm algorithm);

signals:
    void srcImageChanged();
    void outImageChanged();
    void srcSecondImageLoaded();

private:
    Ui::GUI4OpenCVClass *ui;

    HistogramHandler* histogramHandler;
    ImageViewHandler* imageViewHandler;

    cv::Mat srcImage;
    cv::Mat outImage;

    cv::Mat srcSecondImage;
    int secondImageCounter;
    cv::Mat srcSecondImageResized;

    std::vector<cv::Mat> srcHistograms;
    std::vector<cv::Mat> outHistograms;

    cv::Mat srcHistogramImage;
    cv::Mat outHistogramImage;

    void setDebugPrintingPatterns();
    void onImageChanged();
    void onHistogramChanged();
    cv::Mat readInImage();
    bool saveImage(cv::Mat& image);
    QWidget* buildEmptyWindow(QWidget* parent, QSize size, Qt::WindowModality modality);
    QWidget* openSecondSourceImage();
};
