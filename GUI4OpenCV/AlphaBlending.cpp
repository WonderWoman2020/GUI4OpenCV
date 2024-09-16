#include "AlphaBlending.h"

AlphaBlending::AlphaBlending()
{

}

AlphaBlending::~AlphaBlending()
{
    this->clear();
}

void AlphaBlending::clear()
{
    this->secondImageResized.release();
    qInfo() << "Freed memory of temporary resized copy of second source image in alpha blending operation";
}

cv::Mat AlphaBlending::process(cv::Mat& firstImage, cv::Mat& secondImage, int alpha)
{
    // Cancels mixing images, if not both of them were provided
    if (firstImage.empty() || secondImage.empty())
        return cv::Mat();

    try {
        // Resizes a copy of second source image and stores it as temporary variable.
        // Thanks to storing the temporary copy, it is being done only once per the same image.
        if (!(this->secondImageResized.rows == firstImage.rows && this->secondImageResized.cols == firstImage.cols))
        {
            cv::resize(secondImage, this->secondImageResized, cv::Size(firstImage.cols, firstImage.rows));
            qInfo() << "Resized copy of second source image to be the same size as the first one, for alpha blending operation.";
            qInfo() << "Stored the resized image as a temporary copy.";
        }

        // Executes alpha blending operation
        double alphaNormalized = alpha / (double)255;
        double betaNormalized = 1.0 - alphaNormalized;
        cv::Mat result;
        cv::addWeighted(this->secondImageResized, alphaNormalized, firstImage, betaNormalized, 0.0, result);
        return result;
    }
    catch (std::exception ex)
    {
        qDebug() << "Exception in alpha blending operation:" << ex.what();
    }
    return cv::Mat();
}
