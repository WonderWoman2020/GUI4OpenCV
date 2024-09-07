#pragma once

#include <QWidget>
#include "ui_StructuringMatrix.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StructuringMatrixClass; };
QT_END_NAMESPACE

class StructuringMatrix : public QWidget
{
	Q_OBJECT

public:
	StructuringMatrix(QWidget *parent = nullptr);
	~StructuringMatrix();

private:
	Ui::StructuringMatrixClass *ui;
};
