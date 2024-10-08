#include "HistogramCalculator.h"


cv::Mat HistogramCalculator::calculateHistogram(cv::Mat& imagePlane)
{
    // Calculates histogram of one color space of the image
    cv::Mat planeHist;
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange[] = { range };
    cv::calcHist(&imagePlane, 1, 0, cv::Mat(), planeHist, 1, &histSize, histRange);

    // Normalizes histogram data to be 256x200
    int histH = 200;
    cv::normalize(planeHist, planeHist, 0, histH, cv::NORM_MINMAX, -1, cv::Mat());

    return planeHist;
}

std::vector<cv::Mat> HistogramCalculator::calculateHistograms(cv::Mat& image)
{
    // Splits image to planes with just one color space, B, G or R (or grayscale, if provided image has just one color space)
    std::vector<cv::Mat> bgrPlanes;
    cv::split(image, bgrPlanes);

    // Deletes alfa channel if present (not from orignal image, just from this copy), so it won't have histogram calculated
    if (bgrPlanes.size() > 3)
        bgrPlanes.erase(bgrPlanes.begin() + 3, bgrPlanes.end());

    // Calculates histograms of each color space
    std::vector<cv::Mat> histograms;
    for (int i = 0; i < bgrPlanes.size(); i++)
        histograms.push_back(this->calculateHistogram(bgrPlanes.at(i)));

    return histograms;
}

void HistogramCalculator::drawHistogram(cv::Mat& histogram, cv::Mat& histogramImage, int width, int height, cv::Scalar color)
{
    int histSize = 256;
    int bin_w = cvRound((double)width / histSize);

    // Draws histogram chart as an 'cv::Mat' image
    for (int i = 1; i < histSize; i++)
    {
        cv::line(histogramImage, cv::Point(bin_w * (i - 1), height - cvRound(histogram.at<float>(i - 1))),
            cv::Point(bin_w * (i), height - cvRound(histogram.at<float>(i))),
            color, 2, 8, 0);
    }
}

cv::Mat HistogramCalculator::drawChosenHistograms(std::vector<cv::Mat>& histograms, bool b, bool g, bool r, bool grayscale)
{
    // Prepares empty white 'cv::Mat' image, which is a canvas for the histogram chart
    cv::Mat histogramImage = cv::Mat(200, 256, CV_8UC3, cv::Scalar(255, 255, 255));

    // Draws grayscale histogram
    if (histograms.size() == 1)
    {
        if (grayscale)
            this->drawHistogram(histograms.at(0), histogramImage, 256, 200, this->colorSpaceColors.back());
        return histogramImage;
    }

    // Draws BGR histograms, according to which color spaces have been chosen
    if (b && histograms.size() > 0)
        this->drawHistogram(histograms.at(0), histogramImage, 256, 200, this->colorSpaceColors.at(0));

    if (g && histograms.size() > 1)
        this->drawHistogram(histograms.at(1), histogramImage, 256, 200, this->colorSpaceColors.at(1));

    if (r && histograms.size() > 2)
        this->drawHistogram(histograms.at(2), histogramImage, 256, 200, this->colorSpaceColors.at(2));

    return histogramImage;
}
