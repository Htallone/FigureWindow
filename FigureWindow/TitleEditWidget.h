/*
 * TitleEditWidget.h
 *
 *  Created on: Jun 14, 2016
 *      Author: htallone
 */

#ifndef SRC_TITLEEDITWIDGET_H_
#define SRC_TITLEEDITWIDGET_H_

#include <QtWidgets>
#include "qcustomplot/qcustomplot.h"

class TitleEditWidget : public QWidget {

	Q_OBJECT
public:
	TitleEditWidget(QWidget* parent = 0);
	TitleEditWidget(QWidget* parent, QCustomPlot *qcustomplotp);

	virtual ~TitleEditWidget();

	signals:

public slots:
	void slotTitleNameEditingFinished();
	void slotTitleFontSelected();

private slots:


private:

	QCustomPlot *qcustomplot;

	QCPPlotTitle * title;
	QLineEdit * titleNameLineEdit;

	QFontDialog * fontSelectDialog;

};

#endif /* SRC_TITLEEDITWIDGET_H_ */
