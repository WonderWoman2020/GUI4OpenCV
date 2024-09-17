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

	/**
	 * Sets image in an image view.
	 * @param parent - parent window for information window showed, if something goes wrong.
	 * All windows should have a parent, so Qt will take care of their deletion for us.
	 * @param imageView - the image view in which image will be presented. This parameter CAN'T be nullptr.
	 * @param image - the image to present.
	 * @return True - if operation succeded, False - otherwise.
	 */
	bool updateImageView(QWidget* parent, QGraphicsView* imageView, cv::Mat& image);

	/**
	 * Synchronizes (or desynchronizes) movement of provided views corresponding scroll bars. (Synchronizes horizontal scroll bars with each other
	 * and same for vertical scroll bars).
	 * @param firstView - one of the views that have scroll bars which need to be synchronized.
	 * @param secondView - same as firstView, just the other view.
	 * @param sync - True - synchronize scroll bars, False - desynchronize.
	 */
	void syncViewsScrollBars(QGraphicsView* firstView, QGraphicsView* secondView, bool sync);

private:

	/**
	 * Sets image in the QGraphicsView's scene.
	 */
	void setImageInView(QGraphicsView* graphicsView, QPixmap image);

	/**
	 * Makes corresponding scroll bars of the source and out image views to move in sync.
	 */
	void syncScrollBars(QScrollBar* firstScroll, QScrollBar* secondScroll);

	/**
	 * Disables moving corresponding scroll bars of the source and out image views in sync.
	 */
	void desyncScrollBars(QScrollBar* firstScroll, QScrollBar* secondScroll);

};