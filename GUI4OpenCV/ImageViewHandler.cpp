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
        scene = new QGraphicsScene(graphicsView);    // Sets the view to be parent object for the scene, so memory of the scene object will be managed by parent
        graphicsView->setScene(scene);
        qInfo() << "New scene was created. It's parent is " + scene->parent()->objectName();
    }

    scene->clear();    // removes previous image, so images won't stack one on another
    scene->addPixmap(image);
    scene->setSceneRect(scene->itemsBoundingRect());    // resizes scene, so it is not larger than items (image in this case) it contains

    qInfo() << scene->items().count();
}