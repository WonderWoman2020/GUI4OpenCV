#include "AlphaOperationWindows.h"

AlphaOperationWindows::AlphaOperationWindows(QWidget *parent)
	: QWidget(parent)
{
	this->setParent(parent);

	this->alphaWindow = new AlphaSliderWindow(this, 0);
	this->imageWindow = new SecondImageWindow(this, 0);

    this->alphaBlending = new AlphaBlending();

    this->imageLoader = new ImageLoader();
    this->imageViewHandler = new ImageViewHandler();
}

AlphaOperationWindows::~AlphaOperationWindows()
{
	this->src2.release();
    qInfo() << "Freed memory of second source image";

    delete this->alphaBlending;
    delete this->imageLoader;
    delete this->imageViewHandler;
}

void AlphaOperationWindows::showWindows()
{
    this->alphaWindow->show();
    this->imageWindow->show();
}

bool AlphaOperationWindows::readAdditionalImage(QGraphicsView* imageView)
{
    // Informs that second source image needs to be chosen
    QMessageBox::information(this, "Mieszanie obrazow - wymagany drugi obraz",
        "Na potrzeby tej operacji nalezy wczytac drugi obraz. Wybierz drugi obraz do wczytania w nastepnym oknie.");

    cv::Mat temp = this->imageLoader->getImageDialog(this);

    // Cancels alpha linear blending operation, if imaga data hasn't been loaded
    if (temp.empty())
        return false;

    // Releases previous image data if any and assigns newly loaded image data
    this->src2 = temp;
    qInfo() << "Freed memory of second source image and loaded a new one";

    // Adds image to the second source image view
    bool updated = this->imageViewHandler->updateImageView(this, imageView, this->src2);
    if (!updated)
    {
        this->src2.release();
        return false;
    }

    return true;
}

void AlphaOperationWindows::buildWindows()
{
    // Builds window for showing second source image
    imageWindow->resize(QSize(400, 400));
    // Loads second source image
    bool opened = this->readAdditionalImage(imageWindow->getImageView());
    if (!opened)
    {
        imageWindow->deleteLater();
        return;
    }

    // Connects a method to execute alpha linear blending on images, when slider value changes
    connect(alphaWindow, SIGNAL(sendInputData(int)), this, SLOT(execOperation(int)));
    this->alphaBlending->clear();

    // Destroying windows signals    
    connect(alphaWindow, SIGNAL(destroyed()), this, SLOT(deleteLater()));
    connect(imageWindow, SIGNAL(destroyed()), this, SLOT(deleteLater()));

    connect(this, SIGNAL(destroyed()), alphaWindow, SLOT(deleteLater()));
    connect(this, SIGNAL(destroyed()), imageWindow, SLOT(deleteLater()));
}

void AlphaOperationWindows::execOperation(int alpha)
{
    if (this->src1.empty() || this->src2.empty())
    {
        QMessageBox::information(this, "Nie pozyskano dwoch obrazow",
            "Nie mozna przeprowadzic mieszania obrazow, poniewaz nie zaladowano dwoch obrazow. Zaladuj obraz zarowno w oknie glownym, jak i w oknie opcji mieszania obrazow.");
        return;
    }
    cv::Mat result = this->alphaBlending->process(this->src1, this->src2, alpha);
    this->sendResult(result);
}

void AlphaOperationWindows::setSourceImage(cv::Mat& src1)
{
    this->src1 = src1;
}

