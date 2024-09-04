#pragma once

#include <QDialog>
#include "ui_ParametersWindow.h"

#include <QFileDialog>
#include <QGraphicsScene>
#include <QMessageBox>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class ParametersWindowClass; };
QT_END_NAMESPACE

class ParametersWindow : public QDialog
{
	Q_OBJECT

public:
	ParametersWindow(QWidget *parent = nullptr);
	~ParametersWindow();

public slots:
	void on_secondImageButton_clicked();

private:
	Ui::ParametersWindowClass *ui;

	cv::Mat srcSecondImage;
};
