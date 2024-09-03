#include "HistogramHandler.h"


/*
    Calculates histogram of one color space provided and draws histogram chart as 'cv::Mat' image.
*/
cv::Mat HistogramHandler::calculateHistogram(cv::Mat& imagePlane, cv::Scalar histColor)
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

void HistogramHandler::drawHistogram(cv::Mat& histogram, cv::Mat& histImage, int histW, int histH, cv::Scalar histColor)
{
    int histSize = 256;
    int bin_w = cvRound((double)histW / histSize);

    // Draws histogram chart as an 'cv::Mat' image
    for (int i = 1; i < histSize; i++)
    {
        cv::line(histImage, cv::Point(bin_w * (i - 1), histH - cvRound(histogram.at<float>(i - 1))),
            cv::Point(bin_w * (i), histH - cvRound(histogram.at<float>(i))),
            histColor, 2, 8, 0);
    }

    //return histImage;
}

/*
    Creates histogram charts as 'cv::Mat' images, of each BGR color spaces of the provided image, or of grayscale image.
    Histograms stored as 'cv::Mat' images are returned in 'std::vector', and their order is as follows:
    0 - B, 1 - G, 2 - R, or if image was in grayscale, then the vector contains just one histogram, so:
    0 - grayscale histogram.
*/
std::vector<cv::Mat> HistogramHandler::calculateHistograms(cv::Mat& image)
{
    // Splits image to planes with just one color space, B, G or R (or grayscale, if provided image has just one color space)
    std::vector<cv::Mat> bgrPlanes;
    cv::split(image, bgrPlanes);

    // Deletes alfa channel if present (not from orignal image, just from this copy), so it won't have histogram calculated
    if (bgrPlanes.size() > 3)
        bgrPlanes.erase(bgrPlanes.begin() + 3, bgrPlanes.end());

    // Just hardcoded color values, in which histograms will be drawn
    std::vector<cv::Scalar> colorSpaceColors = {
        cv::Scalar(255, 0, 0),    // B
        cv::Scalar(0, 255, 0),    // G
        cv::Scalar(0, 0, 255),    // R
        cv::Scalar(127, 127, 127)    // gray
    };

    // Calculates histograms of each color space
    std::vector<cv::Mat> histograms;
    cv::Scalar histogramColor;
    for (int i = 0; i < bgrPlanes.size(); i++)
    {
        histogramColor = bgrPlanes.size() == 1 ? colorSpaceColors.back() : colorSpaceColors.at(i);    // Sets histogram color to gray or one of BGR colors
        histograms.push_back(this->calculateHistogram(bgrPlanes.at(i), histogramColor));
    }

    return histograms;
}