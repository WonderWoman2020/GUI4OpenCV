#include "ImageConverter.h"


cv::Mat ImageConverter::convertQPixmapToMat(QPixmap& image)
{
	QImage intermediateForm = image.toImage().rgbSwapped();
	intermediateForm.convertToFormat(QImage::Format_RGB888);

	cv::Mat converted = cv::Mat(intermediateForm.height(),
		intermediateForm.width(),
		CV_8UC(3),
		intermediateForm.bits(),
		intermediateForm.bytesPerLine());

	return converted;
}

QPixmap ImageConverter::convertMatToQPixmap(cv::Mat& image)
{
	QImage intermediateForm(image.data,
		image.cols,
		image.rows,
		image.step,
		QImage::Format_RGB888);

	QPixmap converted = QPixmap::fromImage(intermediateForm.rgbSwapped());
	return converted;
}