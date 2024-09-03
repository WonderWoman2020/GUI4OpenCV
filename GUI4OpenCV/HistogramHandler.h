#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>


class HistogramHandler
{
public:
    cv::Mat calculateHistogram(cv::Mat& imagePlane, cv::Scalar histColor);
    std::vector<cv::Mat> calculateHistograms(cv::Mat& image);
    void drawHistogram(cv::Mat& histogram, cv::Mat& histImage, int histW, int histH, cv::Scalar histColor);
};