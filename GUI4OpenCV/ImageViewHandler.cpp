#include "ImageViewHandler.h"


void ImageViewHandler::setImageInView(QGraphicsView* graphicsView, QPixmap image)
{
    if (graphicsView == nullptr)
        throw std::invalid_argument("Parameter 'QGraphicsView* graphicsView' was nullptr.");

    // Takes existing scene of the view or creates a new one, if there wasn't any
    QGraphicsScene* scene = graphicsView->scene();
    if (scene == nullptr)
    {
        scene = new QGraphicsScene(graphicsView);
        graphicsView->setScene(scene);
    }

    scene->clear();    // removes previous image, so images won't stack one on another
    scene->addPixmap(image);    // adds the image to the view (precisely, to its scene)
    scene->setSceneRect(scene->itemsBoundingRect());    // resizes scene, so it isn't larger than the items it contains
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

        qCritical() << "Exception in updating image view operation:" << ex.what();
        return false;
    }

    return true;
}

void ImageViewHandler::syncScrollBars(QScrollBar* firstScroll, QScrollBar* secondScroll)
{
    connect(firstScroll, SIGNAL(valueChanged(int)), secondScroll, SLOT(setValue(int)));
    connect(secondScroll, SIGNAL(valueChanged(int)), firstScroll, SLOT(setValue(int)));
}

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

