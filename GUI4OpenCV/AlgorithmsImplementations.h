#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

class AlgorithmsImplementations
{
public:
	cv::Mat alphaLinearBlending(cv::Mat& src1, cv::Mat& src2, int alphaValue);

	cv::Mat erosion(cv::Mat src, std::vector<std::vector<bool>> matrixData, std::pair<int, int> characteristicElement);

	cv::Mat lowPassFiltering(cv::Mat src, std::vector<std::vector<int>> matrixData, int divisor);
};