#include "AlphaBlendingController.h"

AlphaBlendingController::AlphaBlendingController(QWidget *parent)
	: QWidget(parent)
{
	this->setParent(parent);

	this->alphaWindow = new AlphaSliderWindow(this);
	this->imageWindow = new SecondImageWindow(this);

    this->alphaBlending = new AlphaBlending();

    this->imageLoader = new ImageLoader();
    this->imageViewHandler = new ImageViewHandler();

    this->buildWindows();
}

AlphaBlendingController::~AlphaBlendingController()
{
	this->secondSrcImage.release();
    qInfo() << "Freed memory of second source image";

    delete this->alphaBlending;
    delete this->imageLoader;
    delete this->imageViewHandler;
}

void AlphaBlendingController::showWindows()
{
    this->alphaWindow->show();
    this->imageWindow->show();
}

bool AlphaBlendingController::readAdditionalImage()
{
    QGraphicsView* imageView = imageWindow->getImageView();
    // Informs that second source image needs to be chosen
    QMessageBox::information(this, "Mieszanie obrazow - wymagany drugi obraz",
        "Na potrzeby tej operacji nalezy wczytac drugi obraz. Wybierz drugi obraz do wczytania w nastepnym oknie.");

    cv::Mat temp = this->imageLoader->getImageDialog(this);

    // Cancels alpha linear blending operation, if imaga data hasn't been loaded
    if (temp.empty())
        return false;

    // Releases previous image data if any and assigns newly loaded image data
    this->secondSrcImage = temp;
    qInfo() << "Freed memory of second source image and loaded a new one";

    // Adds image to the second source image view
    bool updated = this->imageViewHandler->updateImageView(this, imageView, this->secondSrcImage);
    if (!updated)
    {
        this->secondSrcImage.release();
        return false;
    }

    return true;
}

void AlphaBlendingController::buildWindows()
{
    // Builds window for showing second source image
    imageWindow->resize(QSize(400, 400));

    // Connects a method to execute alpha linear blending on images, when slider value changes
    connect(alphaWindow, SIGNAL(sendInputData(int)), this, SLOT(execOperation(int)));

    // Destroying windows signals    
    connect(alphaWindow, SIGNAL(destroyed()), this, SLOT(deleteLater()));
    connect(imageWindow, SIGNAL(destroyed()), this, SLOT(deleteLater()));

    connect(this, SIGNAL(destroyed()), alphaWindow, SLOT(deleteLater()));
    connect(this, SIGNAL(destroyed()), imageWindow, SLOT(deleteLater()));
}

void AlphaBlendingController::execOperation(int alpha)
{
    if (this->firstSrcImage.empty() || this->secondSrcImage.empty())
    {
        QMessageBox::information(this, "Nie pozyskano dwoch obrazow",
            "Nie mozna przeprowadzic mieszania obrazow, poniewaz nie zaladowano dwoch obrazow. Zaladuj obraz zarowno w oknie glownym, jak i w oknie opcji mieszania obrazow.");
        return;
    }
    this->alphaWindow->setCursor(Qt::WaitCursor);
    cv::Mat result = this->alphaBlending->process(this->firstSrcImage, this->secondSrcImage, alpha);
    this->alphaWindow->setCursor(Qt::ArrowCursor);
    this->sendResult(result);
}

void AlphaBlendingController::setFirstSourceImage(cv::Mat& firstSrcImage)
{
    this->firstSrcImage = firstSrcImage;
}

