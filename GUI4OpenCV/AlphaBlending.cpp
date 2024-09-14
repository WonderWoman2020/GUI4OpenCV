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
    this->src2Resized.release();
}

cv::Mat AlphaBlending::process(cv::Mat& src1, cv::Mat& src2, int alpha)
{
    if (src1.empty() || src2.empty())
        return cv::Mat();

    try {
        if (!(this->src2Resized.rows == src1.rows && this->src2Resized.cols == src1.cols))
        {
            cv::resize(src2, this->src2Resized, cv::Size(src1.cols, src1.rows));
            qInfo() << "Resized second source image to be the same size as the first one";
        }

        double alphaNormalized = alpha / (double)255;
        double betaNormalized = 1.0 - alphaNormalized;
        cv::Mat result;
        cv::addWeighted(this->src2Resized, alphaNormalized, src1, betaNormalized, 0.0, result);
        return result;
    }
    catch (std::exception ex)
    {
        qInfo() << "Wyjatek:" << ex.what();
    }
    return cv::Mat();
}
