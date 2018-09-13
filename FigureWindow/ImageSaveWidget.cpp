/*
 * ImageSaveWidget.cpp
 *
 *  Created on: Aug 4, 2016
 *      Author: htallone
 */

#include <QtWidgets>
#include "ImageSaveWidget.h"

ImageSaveWidget::ImageSaveWidget(QWidget *parent)
: QWidget(parent) {



}

ImageSaveWidget::ImageSaveWidget(QWidget *parent, QCustomPlot *qcustomplotp)
: QWidget(parent)
{
	qcustomplot = qcustomplotp;

	imageWidthSpinBox = new QSpinBox;
	imageWidthSpinBox->setRange(0,1024);
	imageWidthSpinBox->setValue(0);
	imageWidthSpinBox->setSingleStep(1);
	imageWidthSpinBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

	imageHeightSpinBox = new QSpinBox;
	imageHeightSpinBox->setRange(0,1024);
	imageHeightSpinBox->setValue(0);
	imageHeightSpinBox->setSingleStep(1);
	imageHeightSpinBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    browseButton = new QPushButton(tr("&Browse..."), this);
    connect(browseButton, &QAbstractButton::clicked, this, &ImageSaveWidget::browse);
    OKButton = new QPushButton(tr("&OK"), this);
    connect(OKButton, &QAbstractButton::clicked, this, &ImageSaveWidget::okSave);
    cancelButton =  new QPushButton(tr("&Cancel"), this);
    connect(cancelButton, &QAbstractButton::clicked, this, &ImageSaveWidget::cancelSave);


    directoryComboBox = createComboBox(QDir::currentPath());
    fileNameComboBox = createComboBox(tr("unitle.png"));
    fileNameFilterComboBox = createComboBox(tr("PNG Images (*.png)"));
    fileNameFilterComboBox->setEditable(false);
    fileNameFilterComboBox->addItem(tr("JPG Images (*.jpg)"));
    fileNameFilterComboBox->addItem(tr("BMP Images (*.bmp)"));
    fileNameFilterComboBox->addItem(tr("PDF Images (*.pdf)"));
    directoryComboBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    fileNameComboBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    fileNameFilterComboBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    imageWidthLabel = new QLabel(tr("width:"));
    imageHeightLabel = new QLabel(tr("height:"));

    fileNameLabel = new QLabel(tr("Name:"));
    fileNameFilterLabel = new QLabel(tr("Filter:"));
    directoryLabel = new QLabel(tr("In directory:"));

	QHBoxLayout *hlayoutSize = new QHBoxLayout;
	hlayoutSize->addWidget(imageWidthLabel);
	hlayoutSize->addWidget(imageWidthSpinBox);
	hlayoutSize->addWidget(imageHeightLabel);
	hlayoutSize->addWidget(imageHeightSpinBox);

	QHBoxLayout *hlayoutOKandCancel = new QHBoxLayout;
	hlayoutOKandCancel->addStretch(1);
	hlayoutOKandCancel->addWidget(OKButton);
	hlayoutOKandCancel->addWidget(cancelButton);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(hlayoutSize,0,0,1,3);
	mainLayout->addWidget(directoryLabel, 1, 0);
	mainLayout->addWidget(directoryComboBox, 1, 1);
	mainLayout->addWidget(browseButton, 1, 2);
	mainLayout->addWidget(fileNameLabel, 2, 0);
    mainLayout->addWidget(fileNameComboBox, 2, 1, 1, 2);
    mainLayout->addWidget(fileNameFilterLabel, 3, 0);
    mainLayout->addWidget(fileNameFilterComboBox, 3, 1, 1, 2);

    //mainLayout->addLayout(hlayoutOKandCancel, 4, 0,1,3);

	QVBoxLayout *vlayoutAll = new QVBoxLayout;
	vlayoutAll->addLayout(mainLayout);
	vlayoutAll->addStretch(1);
	vlayoutAll->addLayout(hlayoutOKandCancel);
    setLayout(vlayoutAll);

    this->setWindowFlags(Qt::Window);
    setWindowTitle(tr("Save Image"));
    resize(700, 200);
    hide();
}

ImageSaveWidget::~ImageSaveWidget() {
	// TODO Auto-generated destructor stub
}

static void updateComboBox(QComboBox *comboBox)
{
    if (comboBox->findText(comboBox->currentText()) == -1)
        comboBox->addItem(comboBox->currentText());
}

void ImageSaveWidget::browse()
{
    QString directory = QFileDialog::getExistingDirectory(this,
                               tr("Find Files"), QDir::currentPath());

    if (!directory.isEmpty()) {
        if (directoryComboBox->findText(directory) == -1)
            directoryComboBox->addItem(directory);
        directoryComboBox->setCurrentIndex(directoryComboBox->findText(directory));
    }
}

void ImageSaveWidget::okSave()
{
	QString fileName;
	fileName = fileNameComboBox->currentText();
	if(fileName.isEmpty())
	{
		QMessageBox::information(this,tr("empty name"),tr("input a name!"));
		return;
	}
	QString path = directoryComboBox->currentText();
	if(path.isEmpty())
		path = QDir::currentPath();

	fileName = QDir(path).filePath(fileName);
	QString filter = fileNameFilterComboBox->currentText();

	int widthImage = imageWidthSpinBox->value();
	int heightImage = imageHeightSpinBox->value();

    updateComboBox(fileNameComboBox);
    updateComboBox(fileNameFilterComboBox);
    updateComboBox(directoryComboBox);

	if(filter == "PNG Images (*.png)")
	{
		if(QFileInfo(fileName).suffix() != "png")
			fileName = fileName.append(".png");
		if(!(qcustomplot->savePng(fileName,widthImage,heightImage)))
		{
			QMessageBox::information(this,tr("Failed to save image"),tr("Failed to save image!"));
			return;
		}
	}
	else if(filter == QString("JPG Images (*.jpg)"))
	{
		if(QFileInfo(fileName).suffix() != "jpg")
			fileName = fileName.append(".jpg");
		if(!(qcustomplot->saveJpg(fileName,widthImage,heightImage)))
		{
			QMessageBox::information(this,tr("Failed to save image"),tr("Failed to save image!"));
			return;
		}
	}
	else if(filter == "BMP Images (*.bmp)")
	{
		if(QFileInfo(fileName).suffix() != "bmp")
			fileName = fileName.append(".bmp");
		if(!(qcustomplot->saveBmp(fileName,widthImage,heightImage)))
		{
			QMessageBox::information(this,tr("Failed to save image"),tr("Failed to save image!"));
			return;
		}
	}
	else if(filter == "PDF Images (*.pdf)")
	{
		if(QFileInfo(fileName).suffix() != "pdf")
			fileName = fileName.append(".pdf");
		if(!(qcustomplot->savePdf(fileName,true,widthImage,heightImage)))
		{
			QMessageBox::information(this,tr("Failed to save image"),tr("Failed to save image!"));
			return;
		}
	}
	else{

	}

	hide();
}

void ImageSaveWidget::cancelSave()
{
	hide();
}




QComboBox *ImageSaveWidget::createComboBox(const QString &text)
{
    QComboBox *comboBox = new QComboBox;
    comboBox->setEditable(true);
    comboBox->addItem(text);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return comboBox;
}
