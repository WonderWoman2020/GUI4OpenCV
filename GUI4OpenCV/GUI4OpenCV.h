#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GUI4OpenCV.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GUI4OpenCVClass; };
QT_END_NAMESPACE

class GUI4OpenCV : public QMainWindow
{
    Q_OBJECT

public:
    GUI4OpenCV(QWidget *parent = nullptr);
    ~GUI4OpenCV();

/*public slots:
    void on_pushButton_clicked();*/

private:
    Ui::GUI4OpenCVClass *ui;
};
