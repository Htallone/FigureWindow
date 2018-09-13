/*
 * SimpleEditWidget.h
 *
 *  Created on: Jun 11, 2016
 *      Author: htallone
 */

#ifndef SRC_SIMPLEEDITWIDGET_H_
#define SRC_SIMPLEEDITWIDGET_H_

#include <QtWidgets>
#include "qcustomplot/qcustomplot.h"
#include "TitleEditWidget.h"
#include "AxisEditWidget.h"
#include "LegendEditWidget.h"
#include "PlottablesEditWidget.h"

class SimpleEditWidget : public QWidget {

	Q_OBJECT

public:
	SimpleEditWidget(QWidget* parent = 0);
	SimpleEditWidget(QWidget* parent, QCustomPlot *qcustomplotp);
	virtual ~SimpleEditWidget();


signals:

public slots:
	void slotSimpleEditTreeViewSelectionChanged(const QItemSelection &, const QItemSelection &);
	void slotSimpleEditTreeViewNodeNameChanged(int plotIndex,const QString &);
	void slotSimpleEditOKClicked();

	void updateSimpleEdit();

private slots:

private:

	QCustomPlot *qcustomplot;

	QTreeView * simpleEditTreeView;

	TitleEditWidget * titleEdit;
	AxisEditWidget * axisEdit;
	LegendEditWidget * legendEdit;
	PlottablesEditWidget * plottablesEdit;
	QStackedWidget * stackedWidget;
	QWidget * blankWidget;

};

#endif /* SRC_SIMPLEEDITWIDGET_H_ */
