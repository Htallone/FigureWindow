/*
 * LegendEditWidget.h
 *
 *  Created on: Jun 12, 2016
 *      Author: htallone
 */

#ifndef SRC_LEGENDEDITWIDGET_H_
#define SRC_LEGENDEDITWIDGET_H_

#include <QtWidgets>
#include "qcustomplot/qcustomplot.h"

class LegendEditWidget : public QWidget {

	Q_OBJECT

public:
	LegendEditWidget(QWidget* parent = 0);
	LegendEditWidget(QWidget* parent, QCustomPlot *qcustomplotp);
	virtual ~LegendEditWidget();

	signals:

public slots:
	void slotLegendAlignmentSelected(int alignmentIndex);
	void slotLegendFontSelected();

private slots:


private:

	QCustomPlot *qcustomplot;

	QComboBox * legendPositionComboBox;

	QFontDialog * fontSelectDialog;

	std::map<Qt::Alignment, int> alignmentIntMap;
	std::vector<Qt::Alignment> alignmentVec;

};


#endif /* SRC_LEGENDEDITWIDGET_H_ */
