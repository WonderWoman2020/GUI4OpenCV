#pragma once

#include <QDialog>
#include "ui_ParametersWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ParametersWindowClass; };
QT_END_NAMESPACE

class ParametersWindow : public QDialog
{
	Q_OBJECT

public:
	ParametersWindow(QWidget *parent = nullptr);
	~ParametersWindow();

private:
	Ui::ParametersWindowClass *ui;
};
