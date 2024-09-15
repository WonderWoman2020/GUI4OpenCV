#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <QWidget>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

class ImageLoader
{
public:
	ImageLoader();
	~ImageLoader();

	cv::Mat readInImage(std::string path);
	bool saveImage(std::string path, cv::Mat& image);

	cv::Mat getImageDialog(QWidget* parent);
	bool saveImageDialog(QWidget* parent, cv::Mat& image);
};
