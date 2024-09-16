#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <QDebug>

/*
 * Class using OpenCV implementation of alpha linear blending algorithm.
 */
class AlphaBlending
{
public:
	AlphaBlending();
	~AlphaBlending();

	/**
	 * Clears all temporary variables stored in the object to optimize the image processing operation.
	 * (E.g. clears resized copy of second source image)
	 */
	void clear();

	/**
	 * Executes alpha linear blending operation. All pixels of secondImage are multiplied by normalized alpha coeff = alpha/255,
	 * while all pixels of the first image are multiplied by normalized beta coeff = 1 - alpha coeff.
	 * Multiplied values from both pictures are then added and therefore they create an output image, that is a mix of both.
	 * @param firstImage - first source image, the one being showed in main window.
	 * @param secondImage - additional source image only for this operation, the one being showed in loose image window.
	 * @param alpha - value of range from 0 to 255.
	 * @return Image being a mix of the 2 source images.
	 */
	cv::Mat process(cv::Mat& firstImage, cv::Mat& secondImage, int alpha);

private:

	/**
	 * Temporary copy of second source image. It is resized to be the same size as first source image.
	 * The sizes of both images must match for this operation. The resized image doesn't keep its original ratio.
	 */
	cv::Mat secondImageResized;
};

