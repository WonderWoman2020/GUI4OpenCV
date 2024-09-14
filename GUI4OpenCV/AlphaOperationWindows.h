#pragma once

#include <QWidget>

#include <QGraphicsView>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "AlphaSliderWindow.h"
#include "SecondImageWindow.h"
#include "ImageLoader.h"
#include "ImageViewHandler.h"
#include "AlphaBlending.h"

class AlphaOperationWindows  : public QWidget
{
	Q_OBJECT

public:
	AlphaOperationWindows(QWidget *parent);
	~AlphaOperationWindows();

	void buildWindows();
	void showWindows();

signals:
	void sendResult(cv::Mat& result);

public slots:
	void execOperation(int alpha);
	void setSourceImage(cv::Mat& src1);

private:
	cv::Mat src1;
	cv::Mat src2;

	AlphaSliderWindow* alphaWindow;
	SecondImageWindow* imageWindow;

	AlphaBlending* alphaBlending;

	ImageLoader* imageLoader;
	ImageViewHandler* imageViewHandler;

	bool readAdditionalImage(QGraphicsView* imageView);
};
