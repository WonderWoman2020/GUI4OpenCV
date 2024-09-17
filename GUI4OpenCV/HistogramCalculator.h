#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>


class HistogramCalculator
{
public:

    /*
     * Calculates histogram of all color spaces of the provided image.
     * @param image - input image. Must be of depth of at least 1 channel.
     * @return vector of arrays of size 1x256 (256 rows with 1 one column each).
     * Order of color spaces for which histogram was calculated is as follows:
     * at index 0 - blue color space histogram array (OR, grayscale histogram array, if provided image had only 1 channel),
     * at index 1 - green color space histogram array,
     * at index 2 - red color space histogram array.
     */
    std::vector<cv::Mat> calculateHistograms(cv::Mat& image);

    /*
     * Draws histograms according to which color spaces have been chosen.
     * @param histograms - it is a vector of arrays with calculated histogram data, assumed to be stored in following order:
     * at index 0 - blue color space histogram array (OR, grayscale histogram array, if provided vector contains only 1 array),
     * at index 1 - green color space histogram array,
     * at index 2 - red color space histogram array.
     * The arrays are matrices of size 1x256 (256 rows with 1 column each).
     * @param b - whether to include blue color space histogram on chart,
     * @param g - same as b, but for green color space,
     * @param r - same as b, but for red color space,
     * @param grayscale - same as b, but for grayscale.
     * @return Image of size 256x200, with depth of 3 channels. It is white and has chosen histogram charts drawn
     * in colors defined in 'colorSpaceColors' field of this class.
     */
    cv::Mat drawChosenHistograms(std::vector<cv::Mat>& histograms, bool b, bool g, bool r, bool grayscale);

private:

    // Just hardcoded color values, in which histograms will be drawn.
    std::vector<cv::Scalar> colorSpaceColors = {
        cv::Scalar(255, 0, 0),    // B
        cv::Scalar(0, 255, 0),    // G
        cv::Scalar(0, 0, 255),    // R
        cv::Scalar(127, 127, 127)    // gray
    };

    /*
     * Calculates histogram of one color space of an image provided.
     * @param imagePlane - all pixel values of 1 color space (B, G or R) of an image. Pixel values range is from 0 to 255.
     * @return Array of size 1x256 (256 rows with 1 one column each). Each value is count of pixels that have the same brightness level.
     */
    cv::Mat calculateHistogram(cv::Mat& imagePlane);

    /**
     * Draws histogram chart of one color space of an image.
     * @param histogram - Matrix of size 1x256 (256 rows with 1 one column each). Each value is count of pixels that have the same brightness level.
     * @param histogramImage - image on which the histogram chart will be drawn. It must have depth of at least 3 color channels,
     * because charts are drawn in BGR colors. It must be of size of at least 256 x n (n rows, 256 columns). Each column represents 1 brightness
     * level, while each value for that column (stored in suitable row) is the normalized count of pixels, that have this brightness level.
     * @param width - histogram width. Must be a multiple of 256, so all brigthness levels can be represented and take up the same space on chart.
     * @param height - histogram normalized height. Any number, must be only greater than 0.
     * @param color - color in BGR format, in which the histogram chart will be drawn.
     */
    void drawHistogram(cv::Mat& histogram, cv::Mat& histogramImage, int width, int height, cv::Scalar color);
};