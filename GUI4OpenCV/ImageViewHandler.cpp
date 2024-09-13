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
    scene->setSceneRect(scene->itemsBoundingRect());    // resizes scene, so it is not larger than the items it contains

    qInfo() << scene->items().count();
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

void ImageViewHandler::syncViewsScrollBars(QGraphicsView* view1, QGraphicsView* view2, bool sync)
{
    // Fetches horizontal scroll bars of the views
    QScrollBar* scrollH1 = view1->horizontalScrollBar();
    QScrollBar* scrollH2 = view2->horizontalScrollBar();
    // Same thing for vertical scroll bars
    QScrollBar* scrollV1 = view1->verticalScrollBar();
    QScrollBar* scrollV2 = view2->verticalScrollBar();

    if (sync)
    {
        // Connects horizontal (and vertical) scroll bars to be in sync, when using either one of them
        this->syncScrollBars(scrollH1, scrollH2);
        this->syncScrollBars(scrollV1, scrollV2);
    }
    else
    {
        // Disconnects scroll bar pairs, when syncing option is switched off
        this->desyncScrollBars(scrollH1, scrollH2);
        this->desyncScrollBars(scrollV1, scrollV2);
    }
}

