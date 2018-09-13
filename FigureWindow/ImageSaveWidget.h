/*
 * ImageSaveWidget.h
 *
 *  Created on: Aug 4, 2016
 *      Author: htallone
 */

#ifndef SRC_IMAGESAVEWIDGET_H_
#define SRC_IMAGESAVEWIDGET_H_

#include <QWidget>
#include <QDir>
#include "qcustomplot/qcustomplot.h"

class QComboBox;
class QLabel;
class QPushButton;
class QTableWidget;
class QTableWidgetItem;

class ImageSaveWidget : public QWidget{

	Q_OBJECT

public:
	ImageSaveWidget(QWidget *parent = 0);
	ImageSaveWidget(QWidget* parent, QCustomPlot *qcustomplotp);
	virtual ~ImageSaveWidget();

private slots:
	void browse();
	void okSave();
	void cancelSave();

private:
    QComboBox *createComboBox(const QString &text = QString());

    QCustomPlot *qcustomplot;

    QSpinBox * imageWidthSpinBox;
    QSpinBox * imageHeightSpinBox;

    QComboBox *directoryComboBox;
    QComboBox *fileNameComboBox;
    QComboBox *fileNameFilterComboBox;

    QLabel * imageWidthLabel;
    QLabel * imageHeightLabel;

    QLabel *fileNameLabel;
    QLabel *fileNameFilterLabel;
    QLabel *directoryLabel;

    QPushButton *browseButton;
    QPushButton *OKButton;
    QPushButton *cancelButton;


    QDir currentDir;
};

#endif /* SRC_IMAGESAVEWIDGET_H_ */
