/*
 * AxisEditWidget.cpp
 *
 *  Created on: Jun 12, 2016
 *      Author: htallone
 */

#include "AxisEditWidget.h"

AxisEditWidget::AxisEditWidget(QWidget* parent) : QWidget(parent){
	// TODO Auto-generated constructor stub

}

AxisEditWidget::AxisEditWidget(QWidget* parent, QCustomPlot *qcustomplotp, int axisIndexp)
								: QWidget(parent)
{
	qcustomplot = qcustomplotp;
	axisIndex = axisIndexp;


	axisLabelNameLineEdit = new QLineEdit;
	axisRangeLowerLineEdit = new QLineEdit;
	axisRangeUpperLineEdit = new QLineEdit;
	axisRangeLowerLineEdit->setValidator(new QDoubleValidator());
	axisRangeUpperLineEdit->setValidator(new QDoubleValidator());

	axisLabelNameLineEdit->setMaximumWidth(200);
	axisRangeLowerLineEdit->setMaximumWidth(80);
	axisRangeLowerLineEdit->setAlignment(Qt::AlignCenter);
	axisRangeUpperLineEdit->setMaximumWidth(80);
	axisRangeUpperLineEdit->setAlignment(Qt::AlignCenter);
	connect(axisLabelNameLineEdit, SIGNAL(editingFinished()), this, SLOT(slotAxisLabelNameEditingFinished()));
	connect(axisRangeLowerLineEdit, SIGNAL(editingFinished()), this, SLOT(slotAxisRangeEditingFinished()));
	connect(axisRangeUpperLineEdit, SIGNAL(editingFinished()), this, SLOT(slotAxisRangeEditingFinished()));

	fontSelectDialog = new QFontDialog(this);

	QLabel * axisNameLabel = new QLabel("Label Name:");
	QLabel * axisRangeLabel = new QLabel("Axis Range:");
	QLabel * axisLabelFontLabel = new QLabel("Label Font:");
	QLabel * axisTicksFontLabel = new QLabel("Ticks Font:");

	QHBoxLayout *axisRangeLayoutH = new QHBoxLayout;
	axisRangeLayoutH->setMargin(0);
	QLabel * rangeSpaceTakeLabel = new QLabel(" ~ ");
	rangeSpaceTakeLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	axisRangeLayoutH->addWidget(axisRangeLowerLineEdit);
	axisRangeLayoutH->addWidget(rangeSpaceTakeLabel);
	axisRangeLayoutH->addWidget(axisRangeUpperLineEdit);
	axisRangeLayoutH->addStretch(1);

	QHBoxLayout *axisLabelFontLayoutH = new QHBoxLayout;
	QPushButton * axisLabelFontSelectButton = new QPushButton(QIcon("./icon/font-select.png"),"Select Font");
	//axisLabelFontSelectButton->setFlat(true);
	connect(axisLabelFontSelectButton, SIGNAL(clicked()), this, SLOT(slotAxisLabelFontSelected()));
	axisLabelFontSelectButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	axisLabelFontLayoutH->addWidget(axisLabelFontSelectButton);
	axisLabelFontLayoutH->addStretch(1);

	QHBoxLayout *axisTicksLabelFontLayoutH = new QHBoxLayout;
	QPushButton * axisTicksLabelFontSelectButton = new QPushButton(QIcon("./icon/font-select.png"),"Select Font");
	connect(axisTicksLabelFontSelectButton, SIGNAL(clicked()), this, SLOT(slotAxisTicksLabelFontSelected()));
	axisTicksLabelFontSelectButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	axisTicksLabelFontLayoutH->addWidget(axisTicksLabelFontSelectButton);
	axisTicksLabelFontLayoutH->addStretch(1);


	QGridLayout *axisEditLayoutG = new QGridLayout;
	axisEditLayoutG->addWidget(axisNameLabel, 0, 0);
	axisEditLayoutG->addWidget(axisLabelNameLineEdit, 0, 1);
	axisEditLayoutG->addWidget(axisRangeLabel, 1, 0);
	axisEditLayoutG->addLayout(axisRangeLayoutH, 1, 1);
	axisEditLayoutG->addWidget(axisLabelFontLabel, 2, 0);
	axisEditLayoutG->addLayout(axisLabelFontLayoutH, 2, 1);
	axisEditLayoutG->addWidget(axisTicksFontLabel, 3, 0);
	axisEditLayoutG->addLayout(axisTicksLabelFontLayoutH, 3, 1);

	QHBoxLayout *axisEditStretchLayoutH = new QHBoxLayout;
	axisEditStretchLayoutH->addLayout(axisEditLayoutG);
	axisEditStretchLayoutH->addStretch(1);
	QVBoxLayout *axisEditStretchLayoutV = new QVBoxLayout;
	axisEditStretchLayoutV->addLayout(axisEditStretchLayoutH);
	axisEditStretchLayoutV->addStretch(1);

	this->setLayout(axisEditStretchLayoutV);


	if(axisIndex == 0)
	{
		setAxisLabelName(qcustomplot->xAxis->label());
		axisRangeLowerLineEdit->setText(QString::number(qcustomplot->xAxis->range().lower));
		axisRangeUpperLineEdit->setText(QString::number(qcustomplot->xAxis->range().upper));
		connect(qcustomplot->xAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(setAxisRange(const QCPRange &)));
	}

	if(axisIndex == 1)
	{
		setAxisLabelName(qcustomplot->yAxis->label());
		axisRangeLowerLineEdit->setText(QString::number(qcustomplot->yAxis->range().lower));
		axisRangeUpperLineEdit->setText(QString::number(qcustomplot->yAxis->range().upper));
		connect(qcustomplot->yAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(setAxisRange(const QCPRange &)));
	}

}

AxisEditWidget::~AxisEditWidget() {
	// TODO Auto-generated destructor stub
}

void AxisEditWidget::changeAxisIndex(int indexp)
{
	axisIndex = indexp;

	if(axisIndex == 0)
	{
		setAxisLabelName(qcustomplot->xAxis->label());
		axisRangeLowerLineEdit->setText(QString::number(qcustomplot->xAxis->range().lower));
		axisRangeUpperLineEdit->setText(QString::number(qcustomplot->xAxis->range().upper));
		disconnect(qcustomplot->yAxis,0,this,0);
		connect(qcustomplot->xAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(setAxisRange(const QCPRange &)));
	}

	if(axisIndex == 1)
	{
		setAxisLabelName(qcustomplot->yAxis->label());
		axisRangeLowerLineEdit->setText(QString::number(qcustomplot->yAxis->range().lower));
		axisRangeUpperLineEdit->setText(QString::number(qcustomplot->yAxis->range().upper));
		disconnect(qcustomplot->xAxis,0,this,0);
		connect(qcustomplot->yAxis, SIGNAL(rangeChanged(const QCPRange &)), this, SLOT(setAxisRange(const QCPRange &)));
	}
}


void AxisEditWidget::setAxisLabelName(const QString & name)
{
	if(axisLabelNameLineEdit->text() != name)
		axisLabelNameLineEdit->setText(name);
}
void AxisEditWidget::setAxisRange(double lower, double upper)
{
	if(axisRangeLowerLineEdit->text().toDouble() != lower)
		axisRangeLowerLineEdit->setText(QString::number(lower));

	if(axisRangeUpperLineEdit->text().toDouble() != upper)
		axisRangeUpperLineEdit->setText(QString::number(upper));
}

void AxisEditWidget::setAxisRange(const QCPRange & newRange)
{
	setAxisRange(newRange.lower,newRange.upper);
}

void AxisEditWidget::slotAxisLabelNameEditingFinished()
{
	if((axisLabelNameLineEdit->text() == "") || (axisLabelNameLineEdit->text() == "NoName"))
	{
		if((axisIndex == 0 ) && (qcustomplot->xAxis->label() != ""))
			qcustomplot->xAxis->setLabel("");
		else if((axisIndex == 1 ) && (qcustomplot->yAxis->label() != ""))
			qcustomplot->yAxis->setLabel("");

		qcustomplot->replot();

	}
	else
	{
		if((axisIndex == 0) && (axisLabelNameLineEdit->text() != qcustomplot->xAxis->label()))
			qcustomplot->xAxis->setLabel(axisLabelNameLineEdit->text());
		else if((axisIndex == 1) && (axisLabelNameLineEdit->text() != qcustomplot->yAxis->label()))
			qcustomplot->yAxis->setLabel(axisLabelNameLineEdit->text());

		qcustomplot->replot();

	}
}
void AxisEditWidget::slotAxisRangeEditingFinished()
{
	if(((sender() == axisRangeLowerLineEdit) && (axisRangeUpperLineEdit != focusWidget()))
			|| ((sender() == axisRangeUpperLineEdit) && (axisRangeLowerLineEdit != focusWidget())))
	{
		double lower = axisRangeLowerLineEdit->text().toDouble();
		double upper = axisRangeUpperLineEdit->text().toDouble();

		if(lower<upper){
			if(axisIndex == 0)
			{
				double lowerOld = qcustomplot->xAxis->range().lower;
				double upperOld = qcustomplot->xAxis->range().upper;
				if((lower!=lowerOld) || (upper!=upperOld))
					qcustomplot->xAxis->setRange(lower,upper);
			}

			else if(axisIndex == 1)
			{
				double lowerOld = qcustomplot->yAxis->range().lower;
				double upperOld = qcustomplot->yAxis->range().upper;
				if((lower!=lowerOld) || (upper!=upperOld))
					qcustomplot->yAxis->setRange(lower,upper);
			}

			qcustomplot->replot();
		}

		else
		{
			QMessageBox::information(this,tr("Range input invalid"),tr("Range input invalid(lower>upper)! Please re-input."));
			axisRangeLowerLineEdit->setFocus();
		}


	}

}
void AxisEditWidget::slotAxisLabelFontSelected()
{
	QFont fontSelected;
	if(axisIndex == 0){
		fontSelectDialog->setCurrentFont(qcustomplot->xAxis->labelFont());
		if(fontSelectDialog->exec())
		{
			fontSelected = fontSelectDialog->selectedFont();
			if(fontSelected != qcustomplot->xAxis->labelFont())
			{
				qcustomplot->xAxis->setLabelFont(fontSelected);
				qcustomplot->replot();
			}
		}
	}
	else if(axisIndex == 1)
	{
		fontSelectDialog->setCurrentFont(qcustomplot->yAxis->labelFont());
		if(fontSelectDialog->exec())
		{
			fontSelected = fontSelectDialog->selectedFont();
			if(fontSelected != qcustomplot->yAxis->labelFont())
			{
				qcustomplot->yAxis->setLabelFont(fontSelected);
				qcustomplot->replot();
			}
		}
	}

	this->setFocus();
}
void AxisEditWidget::slotAxisTicksLabelFontSelected()
{
	QFont fontSelected;
	if(axisIndex == 0){
		fontSelectDialog->setCurrentFont(qcustomplot->xAxis->tickLabelFont());
		if(fontSelectDialog->exec())
		{
			fontSelected = fontSelectDialog->selectedFont();
			if(fontSelected != qcustomplot->xAxis->tickLabelFont())
			{
				qcustomplot->xAxis->setTickLabelFont(fontSelected);
				qcustomplot->replot();
			}
		}
	}
	else if(axisIndex == 1)
	{
		fontSelectDialog->setCurrentFont(qcustomplot->yAxis->tickLabelFont());
		if(fontSelectDialog->exec())
		{
			fontSelected = fontSelectDialog->selectedFont();
			if(fontSelected != qcustomplot->yAxis->tickLabelFont())
			{
				qcustomplot->yAxis->setTickLabelFont(fontSelected);
				qcustomplot->replot();
			}
		}
	}

	this->setFocus();
}

