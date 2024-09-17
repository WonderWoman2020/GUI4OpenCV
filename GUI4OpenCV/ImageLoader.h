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

	/**
	 * Reads image at the provided path. Allowed image file formats: JPG, JPEG, PNG and BMP.
	 * @param path - path to image. Contains the filename in it as well.
	 * @return Loaded image or empty image, if loading failed.
	 */
	cv::Mat readInImage(std::string path);

	/*
	 * Saves image at the provided path. Allowed image file formats: JPG, JPEG, PNG and BMP.
	 * @param path - path for saving image. Contains the filename in it as well.
	 * @param image - the image to be saved.
	 * @return True - saving operation success, False - operation failed.
	 */
	bool saveImage(std::string path, cv::Mat& image);

	/**
	 * Handles 'open image' dialog. Shows adequate information windows throughout the operation.
	 * Loads the image chosen by user to memory. Allowed image file formats: JPG, JPEG, PNG and BMP.
	 * @param parent - a parent window for the 'open image' dialog window. All windows should have a parent, so Qt will take care of their deletion for us.
	 * @return - Loaded image chosen by user, or
	 * - empty image, if something failed while opening it.
	 */
	cv::Mat getImageDialog(QWidget* parent);

	/**
	 * Handles 'save image' dialog. Shows adequate information windows throughout the operation.
	 * Saves the image in a file at path chosen by user. Allowed image file formats: JPG, JPEG, PNG and BMP.
	 * @param parent - a parent window for the 'open image' dialog window. All windows should have a parent, so Qt will take care of their deletion for us.
	 * @param image - the image to be saved.
	 * @return True - saving operation success, False - operation failed.
	 */
	bool saveImageDialog(QWidget* parent, cv::Mat& image);
};
