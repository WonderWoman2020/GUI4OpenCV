#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <QPixmap>

class ImageConverter
{
public:
	/*
	 * Converts image from Qt's format (QPixmap) to OpenCV format (cv::Mat). Converted image is of depth of 3 channels.
	 * Converted image IS NOT a copy, it uses the same image data buffer as provided image.
	 * @param image - image in Qt's QPixmap format.
	 * @return Converted image in OpenCV cv::Mat format. 
	 */
	static cv::Mat convertQPixmapToMat(QPixmap& image);

	/*
	 * Converts image from OpenCV format (cv::Mat) to Qt's format (QPixmap). Converted image is of depth of 3 channels.
	 * Converted image IS NOT a copy, it uses the same image data buffer as provided image.
	 * @param image - image in OpenCV cv::Mat format.
	 * @return Converted image in Qt's QPixmap format.
	 */
	static QPixmap convertMatToQPixmap(cv::Mat& image);

};