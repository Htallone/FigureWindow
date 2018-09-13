/*
 * AxisEditWidget.h
 *
 *  Created on: Jun 12, 2016
 *      Author: htallone
 */

#ifndef SRC_AXISEDITWIDGET_H_
#define SRC_AXISEDITWIDGET_H_

#include <QtWidgets>
#include "qcustomplot/qcustomplot.h"

class AxisEditWidget : public QWidget {

	Q_OBJECT

public:
	AxisEditWidget(QWidget* parent = 0);
	AxisEditWidget(QWidget* parent, QCustomPlot *qcustomplotp, int axisIndex);

	virtual ~AxisEditWidget();
signals:

public slots:
	void changeAxisIndex(int indexp);

	void setAxisLabelName(const QString & name);
	void setAxisRange(double lower, double upper);
	void setAxisRange(const QCPRange & newRange);

	void slotAxisLabelNameEditingFinished();
	void slotAxisRangeEditingFinished();
	void slotAxisLabelFontSelected();
	void slotAxisTicksLabelFontSelected();


private slots:

private:

	QCustomPlot *qcustomplot;
	int axisIndex;

	QLineEdit * axisLabelNameLineEdit;
	QLineEdit * axisRangeLowerLineEdit;
	QLineEdit * axisRangeUpperLineEdit;

	QFontDialog * fontSelectDialog;


};

#endif /* SRC_AXISEDITWIDGET_H_ */
