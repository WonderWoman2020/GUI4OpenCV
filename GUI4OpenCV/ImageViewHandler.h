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

	void syncScrollBars(QScrollBar* srcHScroll, QScrollBar* outHScroll);
	void desyncScrollBars(QScrollBar* srcHScroll, QScrollBar* outHScroll);

	void syncViewsScrollBars(QGraphicsView* view1, QGraphicsView* view2, bool sync);
};