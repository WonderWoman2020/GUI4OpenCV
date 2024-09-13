#include "ImageManager.h"

ImageManager::ImageManager()
{

}

ImageManager::~ImageManager()
{

}

cv::Mat ImageManager::readInImage(std::string path)
{
    // Tries to read the chosen file
    cv::Mat temp = cv::imread(path);
    if (temp.empty())
        return cv::Mat();
    
    return temp;
}

bool ImageManager::saveImage(std::string path, cv::Mat& image)
{
    // Checks if there is any output image data to save
    if (image.empty())
        return false;

    bool saved = false;
    // Stores output image at the chosen path
    try {
        saved = cv::imwrite(path, image);
    }
    catch (cv::Exception& ex) {
        qInfo() << ex.what();
        return false;
    }
    return saved;
}

void ImageManager::addImage(std::string id, cv::Mat& image)
{

}

void ImageManager::getImage(std::string id)
{

}

void ImageManager::updateImage(std::string id, cv::Mat& image)
{

}

void ImageManager::removeImage(std::string id)
{

}

