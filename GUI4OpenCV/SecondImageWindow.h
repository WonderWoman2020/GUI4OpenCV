#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QGraphicsView>

class SecondImageWindow  : public QWidget
{
	Q_OBJECT

public:
	SecondImageWindow(QWidget *parent, int imageID);
	~SecondImageWindow();

	QGraphicsView* getImageView();

private:
	int imageID;

	void buildWindow();
};
