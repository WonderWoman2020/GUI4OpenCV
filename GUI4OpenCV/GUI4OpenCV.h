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

#include "AlphaBlendingController.h"

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

    /**
     * Handles syncing and desyncing source and output image views corresponding scrolls bars.
     */
    void on_actionSync_triggered();

    /**
     * Handles opening source image action.
     */
    void on_actionOpen_triggered();

    /**
     * Handles saving processed image action.
     */
    void on_actionSave_triggered();

    /**
     * Slot that activates update of histograms (same as other histogram option triggers).
     */
    void on_actionHistB_triggered();

    /**
     * Slot that activates update of histograms (same as other histogram option triggers).
     */
    void on_actionHistG_triggered();

    /**
     * Slot that activates update of histograms (same as other histogram option triggers).
     */
    void on_actionHistR_triggered();

    /**
     * Slot that activates update of histograms (same as other histogram option triggers).
     */
    void on_actionHistGrayscale_triggered();

    /**
     * Opens 'about app' information window.
     */
    void on_actionAboutApp_triggered();

    /**
     * Opens 'about Qt framework' information window.
     */
    void on_actionAboutQt_triggered();

    /**
     * Example on how to change cursor icon to loading.
     */
    void on_actionCursorTest_triggered();

    /**
     * Slot that builds necessary components for executing alpha blending operation and runs them.
     */
    void on_actionAlphaBlending_triggered();

    /**
     * Shows structuring matrix input window.
     * --- Needs to be implemented to run an image processing operation.
     * See example how it can be done in 'GUI4OpenCV::on_actionAlphaBlending_triggered()' ---
     */
    void on_actionStructuringMatrix_triggered();

    /**
     * Shows filter matrix input window.
     * --- Needs to be implemented to run an image processing operation.
     * See example how it can be done in 'GUI4OpenCV::on_actionAlphaBlending_triggered()' ---
     */
    void on_actionFilterMatrix_triggered();

    /**
     * Slot that receives the processed image and updates output image view. Controller of the processing operation needs to be connected to this slot.
     * See example of connecting AlphaBlendingController object's sendResult() method in 'GUI4OpenCV::on_actionAlphaBlending_triggered()'.
     * @param result - Resulting image sent by one of image processing algorithm, that have been connected to this slot.
     */
    void receiveProcessingResult(cv::Mat& result);

signals:
    void srcImageChanged(cv::Mat& src);
    void outImageChanged(cv::Mat& out);

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

    QString authorName = "Imie i nazwisko";

    /**
     * Updates other components that are related to current source image.
     */
    void onSrcImageChanged();

    /**
     * Updates other components that are related to current output image.
     */
    void onOutImageChanged();

    /**
     * Draws histogram of source or output image and updates its view. Draws charts only of the color spaces chosen by user.
     * @param srcHistogramChanged - Which image histogram to update. True - update source image histogram, False - update the output one.
     */
    void onHistogramChanged(bool srcHistogramChanged);
};
