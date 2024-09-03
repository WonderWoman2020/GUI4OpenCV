#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <QPixmap>
#include <QGraphicsView>


class ImageViewHandler
{
public:
	void setImageInView(QGraphicsView* graphicsView, QPixmap image);
};