#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>


class HistogramHandler
{
public:
    cv::Mat calculateHistogram(cv::Mat& imagePlane);
    std::vector<cv::Mat> calculateHistograms(cv::Mat& image);
    void drawHistogram(cv::Mat& histogram, cv::Mat& histImage, int histW, int histH, cv::Scalar histColor);
    cv::Mat drawChosenHistograms(std::vector<cv::Mat>& histograms, bool b, bool g, bool r, bool grayscale);

private:

    // Just hardcoded color values, in which histograms will be drawn
    std::vector<cv::Scalar> colorSpaceColors = {
        cv::Scalar(255, 0, 0),    // B
        cv::Scalar(0, 255, 0),    // G
        cv::Scalar(0, 0, 255),    // R
        cv::Scalar(127, 127, 127)    // gray
    };
};