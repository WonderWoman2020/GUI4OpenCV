#include "ImageViewHandler.h"


/*
    Sets image in the QGraphicsView's scene.
*/
void ImageViewHandler::setImageInView(QGraphicsView* graphicsView, QPixmap image)
{
    if (graphicsView == nullptr)
        throw std::invalid_argument("Parameter 'QGraphicsView* graphicsView' was nullptr.");

    QGraphicsScene* scene = graphicsView->scene();
    if (scene == nullptr)
    {
        scene = new QGraphicsScene(graphicsView);
        graphicsView->setScene(scene);
        qInfo() << "New scene was created. It's parent is " + scene->parent()->objectName();
    }

    scene->clear();    // removes previous image, so images won't stack one on another
    scene->addPixmap(image);
    scene->setSceneRect(scene->itemsBoundingRect());    // resizes scene, so it isn't larger than the items it contains

    qInfo() << scene->items().count();
}

bool ImageViewHandler::updateImageView(QWidget* parent, QGraphicsView* imageView, cv::Mat& image)
{
    if (image.empty())
        return false;

    try {
        this->setImageInView(imageView, ImageConverter::convertMatToQPixmap(image));
    }
    catch (std::exception& ex)
    {
        QMessageBox::critical(parent, "Blad interfejsu",
            "Nie udalo sie zaladowac obrazu do interfejsu. Obraz zostal zaldadowany do pamieci, ale nastapil nieoczekiwany blad w dzialaniu interfejsu.");
        return false;
    }

    return true;
}

/*
    Makes corresponding scroll bars of the source and out image views to move in sync.
*/
void ImageViewHandler::syncScrollBars(QScrollBar* firstScroll, QScrollBar* secondScroll)
{
    connect(firstScroll, SIGNAL(valueChanged(int)), secondScroll, SLOT(setValue(int)));
    connect(secondScroll, SIGNAL(valueChanged(int)), firstScroll, SLOT(setValue(int)));
}


/*
    Disables moving corresponding scroll bars of the source and out image views in sync.
*/
void ImageViewHandler::desyncScrollBars(QScrollBar* firstScroll, QScrollBar* secondScroll)
{
    disconnect(firstScroll, SIGNAL(valueChanged(int)), secondScroll, SLOT(setValue(int)));
    disconnect(secondScroll, SIGNAL(valueChanged(int)), firstScroll, SLOT(setValue(int)));
}

void ImageViewHandler::syncViewsScrollBars(QGraphicsView* firstView, QGraphicsView* secondView, bool sync)
{
    // Fetches horizontal scroll bars of the views
    QScrollBar* firstScrollH = firstView->horizontalScrollBar();
    QScrollBar* secondScrollH = secondView->horizontalScrollBar();
    // Same thing for vertical scroll bars
    QScrollBar* firstScrollV = firstView->verticalScrollBar();
    QScrollBar* secondScrollV = secondView->verticalScrollBar();

    if (sync)
    {
        // Connects horizontal (and vertical) scroll bars to be in sync, when using either one of them
        this->syncScrollBars(firstScrollH, secondScrollH);
        this->syncScrollBars(firstScrollV, secondScrollV);
    }
    else
    {
        // Disconnects scroll bar pairs, when syncing option is switched off
        this->desyncScrollBars(firstScrollH, secondScrollH);
        this->desyncScrollBars(firstScrollV, secondScrollV);
    }
}

