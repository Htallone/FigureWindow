/*
 * PlottablesEditWidget.h
 *
 *  Created on: Jun 9, 2016
 *      Author: htallone
 */

#ifndef SRC_PLOTTABLESEDITWIDGET_H_
#define SRC_PLOTTABLESEDITWIDGET_H_

#include <QtWidgets>
#include "qcustomplot/qcustomplot.h"
#include "ColorSelectComboBox.h"

class PlottablesEditWidget: public QWidget {

	Q_OBJECT

public:
	PlottablesEditWidget(QWidget* parent = 0);
	PlottablesEditWidget(QWidget* parent, QCustomPlot *qcustomplotp, int indexp);
	virtual ~PlottablesEditWidget();

signals:

	void signalPlottablesEditWidgetChanged();
	void signalPlottablesNameChanged(int,const QString &);

public slots:
	void changePlottableIndex(int indexp);
	void unselectAllPlotttables();

	void setPlottableLegendName(const QString & name);
	void setPlottableLineStyle(int lineStyle);
	void setPlottableLineWidth(double lineWidth);
	void setPlottableLineColor(const QColor & lineColor);
	void setPlottableScatterShape(QCPScatterStyle::ScatterShape shape);

	void slotLegendNameEditingFinished();
	void slotLineStyleSelected(int index);
	void slotLineWidthValueChanged(double value);
	void slotLineColorSelected(int index);
	void slotScatterShapeSelected(int index);



private slots:



private:
	QCustomPlot *qcustomplot;
	int index;

	QLineEdit * legendNameLineEdit;
	QComboBox * lineStyleComboBox;
	QDoubleSpinBox * lineWidthSpinBox;
	ColorSelectComboBox * lineColorSelectComboBox;
	QComboBox * scatterShapeComboBox;

	QFileDialog *openPixmapDialog;

	QPixmap pixmapScatterShape;


};

#endif /* SRC_PLOTTABLESEDITWIDGET_H_ */
