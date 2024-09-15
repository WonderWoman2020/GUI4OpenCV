#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <QDebug>

class AlphaBlending
{
public:
	AlphaBlending();
	~AlphaBlending();

	void clear();
	cv::Mat process(cv::Mat& firstImage, cv::Mat& secondImage, int alpha);

private:

	cv::Mat secondImageResized;
};

