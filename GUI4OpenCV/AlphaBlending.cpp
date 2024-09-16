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

cv::Mat AlphaBlending::process(cv::Mat& src1, cv::Mat& src2, int alpha)
{
    if (src1.empty() || src2.empty())
        return cv::Mat();

    try {
        if (!(this->secondImageResized.rows == src1.rows && this->secondImageResized.cols == src1.cols))
        {
            cv::resize(src2, this->secondImageResized, cv::Size(src1.cols, src1.rows));
            qInfo() << "Resized copy of second source image to be the same size as the first one, for alpha blending operation.";
            qInfo() << "Stored the resized image as a temporary copy.";
        }

        double alphaNormalized = alpha / (double)255;
        double betaNormalized = 1.0 - alphaNormalized;
        cv::Mat result;
        cv::addWeighted(this->secondImageResized, alphaNormalized, src1, betaNormalized, 0.0, result);
        return result;
    }
    catch (std::exception ex)
    {
        qDebug() << "Exception in alpha blending operation:" << ex.what();
    }
    return cv::Mat();
}
