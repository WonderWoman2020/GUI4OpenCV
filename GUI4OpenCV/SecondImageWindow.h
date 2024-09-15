#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QGraphicsView>

class SecondImageWindow  : public QWidget
{
	Q_OBJECT

public:
	SecondImageWindow(QWidget *parent);
	~SecondImageWindow();

	QGraphicsView* getImageView();

private:
	void buildWindow();
};
