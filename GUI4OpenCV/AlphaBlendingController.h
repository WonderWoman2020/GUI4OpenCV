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

class AlphaBlendingController  : public QWidget
{
	Q_OBJECT

public:
	AlphaBlendingController(QWidget *parent);
	~AlphaBlendingController();

	void showWindows();
	bool readAdditionalImage();

signals:
	void sendResult(cv::Mat& result);

public slots:
	void execOperation(int alpha);
	void setFirstSourceImage(cv::Mat& firstSrcImage);

private:
	cv::Mat firstSrcImage;
	cv::Mat secondSrcImage;

	AlphaSliderWindow* alphaWindow;
	SecondImageWindow* imageWindow;

	AlphaBlending* alphaBlending;

	ImageLoader* imageLoader;
	ImageViewHandler* imageViewHandler;

	void buildWindows();
};
