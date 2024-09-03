#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <QPixmap>
#include <QGraphicsView>
#include <QtCore>
#include <QScrollBar>


class ImageViewHandler : public QObject
{
	Q_OBJECT

public:
	void setImageInView(QGraphicsView* graphicsView, QPixmap image);

	void syncImagesScrollBars(QScrollBar* srcHScroll, QScrollBar* outHScroll);
	void desyncImagesScrollBars(QScrollBar* srcHScroll, QScrollBar* outHScroll);
};