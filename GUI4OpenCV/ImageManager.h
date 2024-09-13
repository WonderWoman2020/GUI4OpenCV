#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <QDebug>

class ImageManager
{
public:
	ImageManager();
	~ImageManager();

	cv::Mat readInImage(std::string path);
	bool saveImage(std::string path, cv::Mat& image);
	
	void addImage(std::string id, cv::Mat& image);
	void getImage(std::string id);
	void updateImage(std::string id, cv::Mat& image);
	void removeImage(std::string id);

private:
	std::map<std::string, cv::Mat> images;
};
