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

    void setDebugPrintingPatterns();
    void syncImagesScrollBars();
    void desyncImagesScrollBars();

public slots:
    void on_actionSync_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();

private:
    Ui::GUI4OpenCVClass *ui;

    cv::Mat srcImage;
    cv::Mat outImage;

    QGraphicsScene srcScene;
    QGraphicsScene outScene;
};
