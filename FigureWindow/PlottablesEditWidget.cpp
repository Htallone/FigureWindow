/*
 * PlottablesEditWidget.cpp
 *
 *  Created on: Jun 9, 2016
 *      Author: htallone
 */

#include "PlottablesEditWidget.h"

PlottablesEditWidget::PlottablesEditWidget(QWidget* parent) : QWidget(parent)
{

}

PlottablesEditWidget::PlottablesEditWidget(QWidget* parent, QCustomPlot *qcustomplotp, int indexp)
											: QWidget(parent) {

	qcustomplot = qcustomplotp;
	index = indexp;

	legendNameLineEdit = new QLineEdit;
	lineStyleComboBox = new QComboBox;
	lineWidthSpinBox = new QDoubleSpinBox;
	lineColorSelectComboBox = new ColorSelectComboBox;
	scatterShapeComboBox = new QComboBox;


	openPixmapDialog = new QFileDialog(this,tr("Open Pixmap Dialog"),QDir::currentPath()+tr("/icon"),tr("PNG Images (*.png);;JPG Images (*.jpg);;BMP Images (*.bmp)"));
	//openPixmapDialog->setOption(QFileDialog::DontUseNativeDialog);
	openPixmapDialog->setAcceptMode(QFileDialog::AcceptOpen);
	openPixmapDialog->setWindowModality(Qt::WindowModal);
	openPixmapDialog->setFileMode(QFileDialog::ExistingFiles);
	openPixmapDialog->setNameFilterDetailsVisible(true);

	pixmapScatterShape = QPixmap();

	connect(legendNameLineEdit, SIGNAL(editingFinished()), this, SLOT(slotLegendNameEditingFinished()));

	lineStyleComboBox->setToolTip(tr("Line Style"));
	lineStyleComboBox->setEditable(false);
	lineStyleComboBox->setMinimumWidth(80);
	lineStyleComboBox->setIconSize(QSize(80,14));
	QStringList penStyleName = {tr("NoLine"), tr("SolidLine"), tr("DashLine"), tr("DotLine"), tr("DashDotLine"), tr("DashDotDotLine")};
	for(int i=Qt::NoPen;i<Qt::CustomDashLine;i++)
	{
		QPixmap pix(80,14);
		pix.fill(Qt::transparent);

		QBrush brush(Qt::black);
		QPen pen(brush,2.5,(Qt::PenStyle)i);

		QPainter painter(&pix);
		painter.setPen(pen);
		painter.drawLine(2,7,78,7);

		lineStyleComboBox->addItem(QIcon(pix),penStyleName.at(i));
	}
	lineStyleComboBox->setCurrentIndex(2);
	connect(lineStyleComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotLineStyleSelected(int)));


	lineWidthSpinBox->setRange(0.5,10);
	lineWidthSpinBox->setSingleStep(0.5);
	lineWidthSpinBox->setToolTip(tr("Line Width"));
	connect(lineWidthSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotLineWidthValueChanged(double)));

	lineColorSelectComboBox->setToolTip(tr("Line Color"));
	lineColorSelectComboBox->setEditable(false);
	connect(lineColorSelectComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotLineColorSelected(int)));


	scatterShapeComboBox->setToolTip(tr("Scatter Shape"));
	scatterShapeComboBox->setEditable(false);
	scatterShapeComboBox->setIconSize(QSize(16,16));
	QPixmap pix(16,16);
	pix.fill(QColor("transparent"));
	scatterShapeComboBox->addItem(QIcon(pix),tr("None"));
	scatterShapeComboBox->addItem(QIcon("./icon/ssDot.png"),tr("Dot"));
	scatterShapeComboBox->addItem(QIcon("./icon/ssCross.png"),tr("Cross"));
	scatterShapeComboBox->addItem(QIcon("./icon/ssPlus.png"),tr("Plus"));
	scatterShapeComboBox->addItem(QIcon("./icon/ssCircle.png"),tr("Circle"));
	scatterShapeComboBox->addItem(QIcon("./icon/ssDisc.png"),tr("Disc"));
	scatterShapeComboBox->addItem(QIcon("./icon/ssSquare.png"),tr("Square"));
	scatterShapeComboBox->addItem(QIcon("./icon/ssDiamond.png"),tr("Diamond"));
	scatterShapeComboBox->addItem(QIcon("./icon/ssStar.png"),tr("Star"));
	scatterShapeComboBox->addItem(QIcon("./icon/ssTriangle.png"),tr("Triangle"));
	scatterShapeComboBox->addItem(QIcon("./icon/ssTriangleInverted.png"),tr("TriangleInverted"));
	scatterShapeComboBox->addItem(QIcon("./icon/ssCrossSquare.png"),tr("CrossSquare"));
	scatterShapeComboBox->addItem(QIcon("./icon/ssPlusSquare.png"),tr("PlusSquare"));
	scatterShapeComboBox->addItem(QIcon("./icon/ssCrossCircle.png"),tr("CrossCircle"));
	scatterShapeComboBox->addItem(QIcon("./icon/ssPlusCircle.png"),tr("PlusCircle"));
	scatterShapeComboBox->addItem(QIcon("./icon/ssPeace.png"),tr("Peace"));
	scatterShapeComboBox->addItem(QIcon("./icon/ssPixmap.png"),tr("Pixmap"));
	scatterShapeComboBox->setCurrentIndex(1);
	connect(scatterShapeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotScatterShapeSelected(int)));

	QLabel * legendNameLabel = new QLabel(tr("Legend Name:"));
	QLabel * lineStyleLabel  = new QLabel(tr("Style:"));
	QLabel * lineWidthLabel  = new QLabel(tr("Width:"));
	QLabel * lineColorLabel  = new QLabel(tr("Color:"));
	QLabel * scatterShapeLabel  = new QLabel(tr("Scatter Shape:"));

	QVBoxLayout *legendLayoutV = new QVBoxLayout;
	legendLayoutV->addWidget(legendNameLabel);
	legendLayoutV->addWidget(legendNameLineEdit);
	legendLayoutV->setSpacing(0);

	QVBoxLayout *scatterLayoutV = new QVBoxLayout;
	scatterLayoutV->addWidget(scatterShapeLabel);
	scatterLayoutV->addWidget(scatterShapeComboBox);
	scatterLayoutV->setSpacing(0);

	QVBoxLayout *legendScatterLayoutV = new QVBoxLayout;
	legendScatterLayoutV->addLayout(legendLayoutV);
	legendScatterLayoutV->addSpacing(10);
	legendScatterLayoutV->addLayout(scatterLayoutV);
	legendScatterLayoutV->addStretch(1);

	QGroupBox * lineGroup = new QGroupBox("Line");
	QGridLayout *lineGroupLayoutG = new QGridLayout;
	lineGroupLayoutG->addWidget(lineStyleLabel, 0, 0);
	lineGroupLayoutG->addWidget(lineStyleComboBox, 0, 1);
	lineGroupLayoutG->addWidget(lineWidthLabel, 1, 0);
	lineGroupLayoutG->addWidget(lineWidthSpinBox, 1, 1);
	lineGroupLayoutG->addWidget(lineColorLabel, 2, 0);
	lineGroupLayoutG->addWidget(lineColorSelectComboBox, 2, 1);
	lineGroup->setLayout(lineGroupLayoutG);

	QVBoxLayout *lineGroupLayoutV = new QVBoxLayout;
	lineGroupLayoutV->addWidget(lineGroup);
	lineGroupLayoutV->addStretch(1);

	QHBoxLayout *plottablesEditWidgetLayoutH = new QHBoxLayout;
	plottablesEditWidgetLayoutH->addLayout(legendScatterLayoutV);
	plottablesEditWidgetLayoutH->addLayout(lineGroupLayoutV);
	plottablesEditWidgetLayoutH->addStretch(1);

	this->setLayout(plottablesEditWidgetLayoutH);

	QCPCurve * plottable = qobject_cast<QCPCurve *> (qcustomplot->plottable(index));
	if(plottable)
	{
		if(plottable->name() == "")
			setPlottableLegendName("NoName");
		else
			setPlottableLegendName(plottable->name());

		if(plottable->lineStyle() == QCPCurve::lsNone)
		{
			setPlottableLineStyle(0);
			QPen pen = plottable->pen();
			pen.setStyle(Qt::NoPen);
			plottable->setPen(pen);
		}
		else
			setPlottableLineStyle(plottable->pen().style());

		setPlottableLineWidth(plottable->pen().widthF());
		setPlottableLineColor(plottable->pen().color());
		setPlottableScatterShape(plottable->scatterStyle().shape());
	}
}

PlottablesEditWidget::~PlottablesEditWidget() {

}

void PlottablesEditWidget::changePlottableIndex(int indexp)
{
	index = indexp;
	QCPCurve * plottable = qobject_cast<QCPCurve *> (qcustomplot->plottable(index));
	if(plottable->name() == "")
		setPlottableLegendName("NoName");
	else
		setPlottableLegendName(plottable->name());

	if(plottable->lineStyle() == QCPCurve::lsNone)
		setPlottableLineStyle(0);
	else
		setPlottableLineStyle(plottable->pen().style());

	setPlottableLineWidth(plottable->pen().widthF());
	setPlottableLineColor(plottable->pen().color());
	setPlottableScatterShape(plottable->scatterStyle().shape());
}

void PlottablesEditWidget::unselectAllPlotttables()
{
	for(int i=0;i<qcustomplot->plottableCount();i++)
		qcustomplot->plottable(i)->setSelected(false);
	qcustomplot->replot();
}

void PlottablesEditWidget::setPlottableLegendName(const QString & name)
{
	if(legendNameLineEdit->text() != name)
		legendNameLineEdit->setText(name);
}
void PlottablesEditWidget::setPlottableLineStyle(int lineStyle)
{
	if(lineStyleComboBox->currentIndex() != lineStyle)
		lineStyleComboBox->setCurrentIndex(lineStyle);
}
void PlottablesEditWidget::setPlottableLineWidth(double lineWidth)
{
	if(lineWidthSpinBox->value() != lineWidth)
		lineWidthSpinBox->setValue(lineWidth);
}
void PlottablesEditWidget::setPlottableLineColor(const QColor & lineColor)
{
	if(lineColorSelectComboBox->color() != lineColor)
		lineColorSelectComboBox->setColor(lineColor);
}
void PlottablesEditWidget::setPlottableScatterShape(QCPScatterStyle::ScatterShape shape)
{
	if(scatterShapeComboBox->currentIndex() != (int)(shape))
		scatterShapeComboBox->setCurrentIndex((int)(shape));
}


void PlottablesEditWidget::slotLegendNameEditingFinished()
{
	QCPCurve * plottable = qobject_cast<QCPCurve *> (qcustomplot->plottable(index));
	if((legendNameLineEdit->text() == "") || (legendNameLineEdit->text() == "NoName"))
	{
		emit signalPlottablesNameChanged(index,"NoName");
                plottable->setName("");
		if(plottable->removeFromLegend())
			qcustomplot->replot();

	}
	else
	{
		if(plottable->name() != legendNameLineEdit->text())
			plottable->setName(legendNameLineEdit->text());

		if(!qcustomplot->legend->hasItemWithPlottable(plottable))
			plottable->addToLegend();

		emit signalPlottablesNameChanged(index,plottable->name());


	}

	if(qcustomplot->legend->itemCount()>0)
		qcustomplot->legend->setVisible(true);
	else
		qcustomplot->legend->setVisible(false);

	qcustomplot->replot();
}

void PlottablesEditWidget::slotLineStyleSelected(int curindex)
{
	QCPCurve * plottable = qobject_cast<QCPCurve *> (qcustomplot->plottable(index));
	if((int)(plottable->pen().style()) != curindex)
	{
		if(curindex == 0)
		{
			plottable->setLineStyle(QCPCurve::lsNone);
			QPen pen = plottable->pen();
			pen.setStyle(Qt::NoPen);
			plottable->setPen(pen);
		}
		else
		{
			plottable->setLineStyle(QCPCurve::lsLine);
			QPen pen = plottable->pen();
			pen.setStyle((Qt::PenStyle)curindex);
			plottable->setPen(pen);
		}
		qcustomplot->replot();
	}

}

void PlottablesEditWidget::slotLineWidthValueChanged(double value)
{
	QCPCurve * plottable = qobject_cast<QCPCurve *> (qcustomplot->plottable(index));
	if(plottable->pen().widthF() != value)
	{
		QPen pen = plottable->pen();
		pen.setWidthF(value);
		plottable->setPen(pen);

		qcustomplot->replot();
	}
}
void PlottablesEditWidget::slotLineColorSelected(int /*indexp*/)
{
	QCPCurve * plottable = qobject_cast<QCPCurve *> (qcustomplot->plottable(index));
	QColor curColor = lineColorSelectComboBox->color();
	if(plottable->pen().color() != curColor)
	{
		QPen pen = plottable->pen();
		pen.setColor(curColor);
		plottable->setPen(pen);

		qcustomplot->replot();
	}
}

void PlottablesEditWidget::slotScatterShapeSelected( int indexp)
{
	QCPCurve * plottable = qobject_cast<QCPCurve *> (qcustomplot->plottable(index));
	QCPScatterStyle ssNew = plottable->scatterStyle();

	if((indexp == (int)(QCPScatterStyle::ssPixmap)))
	{
		if(plottable->scatterStyle().shape() != (int)(QCPScatterStyle::ssPixmap))
		{
			QString fileName;
			if(openPixmapDialog->exec())
			{
				fileName = openPixmapDialog->selectedFiles().at(0);
				if(fileName.isEmpty())
				{
					QMessageBox::information(this,tr("Select a Pixmap failed"),tr("Select a Pixmap failed! will set no scatter"));
					scatterShapeComboBox->setCurrentIndex(0);
					return;
				}
				else
				{
					pixmapScatterShape = QPixmap(fileName);
					if(pixmapScatterShape.isNull())
					{
						QMessageBox::information(this,tr("Select a Pixmap failed"),tr("Select a Pixmap failed! will set no scatter"));
						scatterShapeComboBox->setCurrentIndex(0);
						return;
					}
					ssNew.setShape(QCPScatterStyle::ssPixmap);
					ssNew.setPixmap(pixmapScatterShape);
				}
			}
			else
			{
				QMessageBox::information(this,tr("Select a Pixmap Dialog Open failed"),tr("Select a Pixmap failed! will set no scatter"));
				scatterShapeComboBox->setCurrentIndex(0);
				return;
			}
		}
	}
	else
	{
		if( indexp != (int)(plottable->scatterStyle().shape()))
			ssNew.setShape((QCPScatterStyle::ScatterShape)indexp);
	}
	plottable->setScatterStyle(ssNew);
	qcustomplot->replot();

}
