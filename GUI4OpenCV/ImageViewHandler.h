#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <QPixmap>
#include <QGraphicsView>
#include <QtCore>
#include <QScrollBar>

#include <QMessageBox>
#include "ImageConverter.h"


class ImageViewHandler : public QObject
{
	Q_OBJECT

public:

	bool updateImageView(QWidget* parent, QGraphicsView* imageView, cv::Mat& image);
	void syncViewsScrollBars(QGraphicsView* firstView, QGraphicsView* secondView, bool sync);

private:
	void setImageInView(QGraphicsView* graphicsView, QPixmap image);
	void syncScrollBars(QScrollBar* firstScroll, QScrollBar* secondScroll);
	void desyncScrollBars(QScrollBar* firstScroll, QScrollBar* secondScroll);

};