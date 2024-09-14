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
	cv::Mat process(cv::Mat& src1, cv::Mat& src2, int alpha);

private:

	cv::Mat src2Resized;
};

