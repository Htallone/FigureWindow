/*
 * FigureWidget.cpp
 *
 *  Created on: Jun 15, 2016
 *      Author: htallone
 */

#include "FigureWidget.h"
#include <QtWidgets>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <iostream>

FigureWidget::FigureWidget(QWidget * parent) : QWidget(parent){

    saveFigureDialog = new QFileDialog(this,tr("Save Single Figure Dialog"),QDir::currentPath()+tr("/untitled.png"),tr("PNG Images (*.png);;JPG Images (*.jpg);;BMP Images (*.bmp);;PDF Images (*.pdf)"));
    //saveFigureDialog->setOption(QFileDialog::DontUseNativeDialog);
    saveFigureDialog->setAcceptMode(QFileDialog::AcceptSave);
    saveFigureDialog->setWindowModality(Qt::WindowModal);
    saveFigureDialog->setFileMode(QFileDialog::AnyFile);
    saveFigureDialog->setNameFilterDetailsVisible(true);

    qcustomplot = new QCustomPlot;
    qcustomplot->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
//    qcustomplot->axisRect()->insetLayout()->setInsetPlacement(0,QCPLayoutInset::ipFree);
//    legendPosRatio = QPointF(0,0);
//    qcustomplot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
//    qcustomplot->yAxis->grid()->setZeroLinePen(Qt::NoPen);

    connect(qcustomplot, SIGNAL(mouseDoubleClick(QMouseEvent*)), this, SLOT(mouseDoubleClick(QMouseEvent*)));
    connect(qcustomplot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent* )));
    connect(qcustomplot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent* )));
    connect(qcustomplot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseRelease(QMouseEvent* )));
	qcustomplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	qcustomplot->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(qcustomplot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
	connect(qcustomplot->axisRect(), SIGNAL(layoutElementRectSizeChanged()), this, SLOT(slotAxisRectSizeChanged()));

    holdoff();
    legendoff();
    gridon();
    boxon();

	iszoomon = false;
	ismoveon = false;
	isaxisequalon = false;
	isMouseDownMove = false;
	isLegendMove = false;

	rubberBand = 0;
	simpleEdit = 0;
	simpleEditWindow = new QMainWindow(this);
	simpleEditWindow->setWindowTitle(tr("Figure Simple Edit"));
	simpleEditWindow->setMinimumSize(300, 200);
	simpleEditWindow->resize(600,200);
	//simpleEditWindow->setWindowFlags(Qt::SubWindow);

	//&  ~Qt::WindowMinimizeButtonHint &  ~Qt::WindowCloseButtonHint);

    createActions();
    createTools();
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(qcustomplot);
    this->setLayout(layout);
    this->setFocusPolicy(Qt::ClickFocus );

}

FigureWidget::~FigureWidget() {
	// TODO Auto-generated destructor stub
}


void FigureWidget::plot(QVector<double> x, QVector<double> y)
{
	if(!isholdon)
	{
		qcustomplot->clearGraphs();
		qcustomplot->clearPlottables();
	}
	QCPCurve *curve = new QCPCurve(qcustomplot->xAxis, qcustomplot->yAxis);
	qcustomplot->addPlottable(curve);
	curve->setData(x, y);
	qcustomplot->legend->removeItem(qcustomplot->legend->itemCount()-1);
	legendon();

	qcustomplot->rescaleAxes(true);
	if(isaxisequalon)
		axisequal();
	qcustomplot->replot();
}

void FigureWidget::plot(QVector<double> x, QVector<double> y, QPen pen, const QString & legendname)
{
	if(!isholdon)
	{
		qcustomplot->clearGraphs();
		qcustomplot->clearPlottables();
	}
	QCPCurve *curve = new QCPCurve(qcustomplot->xAxis, qcustomplot->yAxis);
	qcustomplot->addPlottable(curve);

	curve->setData(x, y);
	curve->setPen(pen);
	if(legendname != "")
		curve->setName(legendname);
	else
		qcustomplot->legend->removeItem(qcustomplot->legend->itemCount()-1);
	legendon();

	qcustomplot->rescaleAxes(true);
	if(isaxisequalon)
		axisequal();
	qcustomplot->replot();
}

void FigureWidget::plot(QVector<double> x, QVector<double> y, QPen pen, const QCPScatterStyle & style, const QString & legendname)
{
	if(!isholdon)
	{
		qcustomplot->clearGraphs();
		qcustomplot->clearPlottables();
	}
	QCPCurve *curve = new QCPCurve(qcustomplot->xAxis, qcustomplot->yAxis);
	qcustomplot->addPlottable(curve);

	curve->setData(x, y);
	curve->setPen(pen);
	if(legendname != "")
		curve->setName(legendname);
	else
		qcustomplot->legend->removeItem(qcustomplot->legend->itemCount()-1);
	curve->setScatterStyle(style);
	legendon();

	qcustomplot->rescaleAxes(true);
	if(isaxisequalon)
		axisequal();
	qcustomplot->replot();
}

void FigureWidget::plotScatters(QVector<double> x, QVector<double> y, const QCPScatterStyle & style, const QString & legendname)
{
	if(!isholdon)
	{
		qcustomplot->clearGraphs();
		qcustomplot->clearPlottables();
	}
	QCPCurve *curve = new QCPCurve(qcustomplot->xAxis, qcustomplot->yAxis);
	qcustomplot->addPlottable(curve);

	curve->setData(x, y);
	if(legendname != "")
		curve->setName(legendname);
	else
		qcustomplot->legend->removeItem(qcustomplot->legend->itemCount()-1);
	curve->setLineStyle(QCPCurve::lsNone);
	curve->setScatterStyle(style);
	legendon();

	qcustomplot->rescaleAxes(true);
	if(isaxisequalon)
		axisequal();
	qcustomplot->replot();

}


void FigureWidget::setAxisRangeX(double lower, double upper)
{
	qcustomplot->axisRect()->rangeDragAxis(Qt::Horizontal)->setRange(lower,upper);
}
void FigureWidget::setAxisRangeY(double lower, double upper)
{
	qcustomplot->axisRect()->rangeDragAxis(Qt::Vertical)->setRange(lower,upper);
}
void FigureWidget::setAxisLabelX(const QString &str)
{
	qcustomplot->xAxis->setLabel(str);
}
void FigureWidget::setAxisLabelY(const QString &str)
{
	qcustomplot->yAxis->setLabel(str);
}
void FigureWidget::setAxisLabelX(const QString &str, const QFont &font, const QColor &color)
{
	qcustomplot->xAxis->setLabel(str);
	qcustomplot->xAxis->setLabelFont(font);
	qcustomplot->xAxis->setLabelColor(color);

}
void FigureWidget::setAxisLabelY(const QString &str, const QFont &font, const QColor &color)
{
	qcustomplot->yAxis->setLabel(str);
	qcustomplot->yAxis->setLabelFont(font);
	qcustomplot->yAxis->setLabelColor(color);
}
void FigureWidget::setAxisLabelFontX( const QFont &font, const QColor &color)
{
	qcustomplot->xAxis->setLabelFont(font);
	qcustomplot->xAxis->setLabelColor(color);
}
void FigureWidget::setAxisLabelFontY( const QFont &font, const QColor &color)
{
	qcustomplot->yAxis->setLabelFont(font);
	qcustomplot->yAxis->setLabelColor(color);
}
void FigureWidget::setAxisLabelFont( const QFont &font, const QColor &color)
{
	qcustomplot->xAxis->setLabelFont(font);
	qcustomplot->xAxis->setLabelColor(color);
	qcustomplot->yAxis->setLabelFont(font);
	qcustomplot->yAxis->setLabelColor(color);
}
void FigureWidget::setAxisTicksLabelFontX( const QFont &font, const QColor &color)
{
	qcustomplot->xAxis->setTickLabelFont(font);
	qcustomplot->xAxis->setTickLabelColor(color);
}
void FigureWidget::setAxisTicksLabelFontY( const QFont &font, const QColor &color)
{
	qcustomplot->yAxis->setTickLabelFont(font);
	qcustomplot->yAxis->setTickLabelColor(color);
}
void FigureWidget::setAxisTicksLabelFont( const QFont &font, const QColor &color)
{
	qcustomplot->xAxis->setTickLabelFont(font);
	qcustomplot->xAxis->setTickLabelColor(color);
	qcustomplot->yAxis->setTickLabelFont(font);
	qcustomplot->yAxis->setTickLabelColor(color);
}
void FigureWidget::setLegendFont( const QFont &font, const QColor &color)
{
	qcustomplot->legend->setFont(font);
	qcustomplot->legend->setTextColor(color);
}

void FigureWidget::setTitle(const QString &str)
{
	QCPPlotTitle * title = qobject_cast<QCPPlotTitle *> (qcustomplot->plotLayout()->element(0,0));
	if(title )
	{
		qcustomplot->plotLayout()->remove(title);
		qcustomplot->plotLayout()->simplify();
		qcustomplot->plotLayout()->insertRow(0);
		qcustomplot->plotLayout()->addElement(0, 0, new QCPPlotTitle(qcustomplot,str));
	}
	else
	{
		qcustomplot->plotLayout()->simplify();
		qcustomplot->plotLayout()->insertRow(0);
		qcustomplot->plotLayout()->addElement(0, 0, new QCPPlotTitle(qcustomplot,str));
	}
	qcustomplot->replot();
}
void FigureWidget::setTitle(const QString &str, const QFont &font, const QColor &color )
{
	QCPPlotTitle * title = qobject_cast<QCPPlotTitle *> (qcustomplot->plotLayout()->element(0,0));
	if(title )
	{
		qcustomplot->plotLayout()->remove(title);
		qcustomplot->plotLayout()->simplify();
		if(str!="")
		{
			title->setText(str);
			title->setFont(font);
			title->setTextColor(color);
		}
	}
	else
	{
		qcustomplot->plotLayout()->simplify();
		qcustomplot->plotLayout()->insertRow(0);
		title = new QCPPlotTitle(qcustomplot,str);
		title->setFont(font);
		title->setTextColor(color);
		qcustomplot->plotLayout()->addElement(0, 0, title);
	}
	qcustomplot->replot();
}


void FigureWidget::holdon()
{
	isholdon = true;
}
void FigureWidget::holdoff()
{
	isholdon = false;
}
void FigureWidget::legendon()
{
	islegendon = true;
	if(qcustomplot->legend->itemCount()==0)
		qcustomplot->legend->setVisible(false);
	else
		qcustomplot->legend->setVisible(true);
	qcustomplot->replot();
}
void FigureWidget::legendoff()
{
	islegendon = false;
	qcustomplot->legend->setVisible(false);
	qcustomplot->replot();
}
void FigureWidget::gridon()
{
	isgridon = true;
	qcustomplot->xAxis->grid()->setVisible(true);
	qcustomplot->yAxis->grid()->setVisible(true);
	qcustomplot->replot();
}
void FigureWidget::gridoff()
{
	isgridon = false;
	qcustomplot->xAxis->grid()->setVisible(false);
	qcustomplot->yAxis->grid()->setVisible(false);
	qcustomplot->replot();
}
void FigureWidget::boxon()
{
	isboxon = true;
	qcustomplot->axisRect()->setVisible(true);
	qcustomplot->axisRect()->setupFullAxesBox(true);
//	qcustomplot->xAxis2->setTicks(false);
//	qcustomplot->yAxis2->setTicks(false);
	qcustomplot->replot();
}
void FigureWidget::boxoff()
{
	isboxon = false;
	//qcustomplot->axisRect()->setVisible(false);
	//qcustomplot->axisRect()->setupFullAxesBox(false);
	qcustomplot->xAxis->setVisible(false);
	qcustomplot->yAxis->setVisible(false);
	qcustomplot->xAxis2->setVisible(false);
	qcustomplot->yAxis2->setVisible(false);
	qcustomplot->replot();
}


void FigureWidget::mouseDoubleClick(QMouseEvent * e)
{

	isMouseDoubleClick = true;

	if(e->button()==Qt::LeftButton)
	{
		if(qcustomplot->axisRect()->rect().contains(e->pos()) && (qcustomplot->axisRect()->insetLayout()->selectTest(e->pos(),false) <0) )
			qcustomplot->rescaleAxes();
		qcustomplot->replot();
	}
}

void FigureWidget::mousePress(QMouseEvent * e)
{
	isMouseDoubleClick = false;

	mousePressPos = e->pos();
	mouseReleasePos = e->pos();
	//Drag and Drop 5.6
	if(e->button()==Qt::LeftButton)
	{
		isMouseDownMove = true;
		if(iszoomon)
		{
			if(qcustomplot->axisRect()->rect().contains(mousePressPos) && (qcustomplot->axisRect()->insetLayout()->selectTest(mousePressPos,false) <0))
			{
				if (!rubberBand)
					rubberBand = new QRubberBand(QRubberBand::Rectangle,qcustomplot);
				rubberBand->setGeometry(QRect(mousePressPos, QSize()));
				rubberBand->show();
			}
		}

		if(ismoveon && qcustomplot->legend->visible() && (qcustomplot->legend->selectTest(mousePressPos, false) >= 0))
		{
			isLegendMove = true;
			QRect axisInsetRect = qcustomplot->axisRect()->insetLayout()->rect();

			legendRectPos = qcustomplot->legend->outerRect().topLeft();
			legendRectSize = qcustomplot->legend->outerRect().size();
			legendWidthRatio = (double)(legendRectSize.width())/axisInsetRect.width();
			legendHeightRatio = (double)(legendRectSize.height())/axisInsetRect.height();

			if(qcustomplot->axisRect()->insetLayout()->insetPlacement(0) == QCPLayoutInset::ipBorderAligned)
			{
				QPoint error = legendRectPos - axisInsetRect.topLeft();
				legendPosRatio = QPoint((double)(error.x())/axisInsetRect.width(),(double)(error.y())/axisInsetRect.height());
				qcustomplot->axisRect()->insetLayout()->setInsetPlacement(0,QCPLayoutInset::ipFree);
			}
		}
	}
}

void FigureWidget::mouseMove(QMouseEvent * e)
{

	//Drag and Drop 5.6
	if(isMouseDownMove && ((e->buttons() & Qt::LeftButton)))
	{
		mouseReleasePos = e->pos();
		if(iszoomon)
		{
			if(qcustomplot->axisRect()->rect().contains(mousePressPos) && (qcustomplot->axisRect()->insetLayout()->selectTest(mousePressPos,false) <0))
			{
				if(rubberBand)
					rubberBand->setGeometry(QRect(mousePressPos, mouseReleasePos).normalized());
			}
		}

		if(ismoveon &&  isLegendMove/*&& (qcustomplot->legend->selectTest(mouseReleasePos, false) >= 0) */)
		{

			QPoint newLegendRectPos = legendRectPos + mouseReleasePos - mousePressPos;
			QRect axisInsetRect = qcustomplot->axisRect()->insetLayout()->rect();
			QPoint error = newLegendRectPos - axisInsetRect.topLeft();
			legendPosRatio = QPointF((double)(error.x())/axisInsetRect.width(),(double)(error.y())/axisInsetRect.height());

			qcustomplot->axisRect()->insetLayout()->setInsetRect(0,QRectF(legendPosRatio.x(),legendPosRatio.y(),legendWidthRatio,legendHeightRatio));
			qcustomplot->axisRect()->insetLayout()->update(QCPLayoutInset::upLayout);
			qcustomplot->replot();

		}


	}

}
void FigureWidget::mouseRelease(QMouseEvent * e)
{
	//this->centralWidget()->setFocus();
	mouseReleasePos = e->pos();

	int posErrorLimit = 2;
	int posError = abs((mouseReleasePos-mousePressPos).x()) + abs((mouseReleasePos-mousePressPos).y());
	//Drag and Drop 5.6
	if(e->button()==Qt::LeftButton)
	{
		isMouseDownMove = false;
		if(iszoomon)
		{
			if (rubberBand)
				rubberBand->hide();
			if(qcustomplot->axisRect()->rect().contains(mousePressPos) && (qcustomplot->axisRect()->insetLayout()->selectTest(mousePressPos,false) <0))
			{

				QCPAxis *rangeDragHorz = qcustomplot->axisRect()->rangeDragAxis(Qt::Horizontal);
				QCPAxis *rangeDragVert = qcustomplot->axisRect()->rangeDragAxis(Qt::Vertical);

				double x0,y0,x1,y1;
				x1 = rangeDragHorz->pixelToCoord(mouseReleasePos.x());
				y1 = rangeDragVert->pixelToCoord(mouseReleasePos.y());

				if((posError<=posErrorLimit) && (!isMouseDoubleClick))
				{
					rangeDragHorz->scaleRange(qcustomplot->axisRect()->rangeZoomFactor(Qt::Horizontal), x1);
					rangeDragVert->scaleRange(qcustomplot->axisRect()->rangeZoomFactor(Qt::Vertical), y1);
				}
				else
				{
					x0 = rangeDragHorz->pixelToCoord(mousePressPos.x());
					y0 = rangeDragVert->pixelToCoord(mousePressPos.y());
					if(x0>x1)
						qSwap(x0,x1);
					if(y0>y1)
						qSwap(y0,y1);
					rangeDragHorz->setRange(x0,x1);
					rangeDragVert->setRange(y0,y1);
				}
			}
		}

		if(ismoveon)
		{
			isLegendMove = false;
		}

		qcustomplot->replot();
	}


}


void FigureWidget::moveLegend()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
    	if(qcustomplot->axisRect()->insetLayout()->insetPlacement(0) != QCPLayoutInset::ipBorderAligned)
    		qcustomplot->axisRect()->insetLayout()->setInsetPlacement(0,QCPLayoutInset::ipBorderAligned);
    	qcustomplot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
    	qcustomplot->replot();
    }
  }
}


void FigureWidget::contextMenuRequest(QPoint pos)
{
	QMenu *menu = new QMenu(this);
	menu->setAttribute(Qt::WA_DeleteOnClose);
    if (qcustomplot->legend->selectTest(pos, false) >= 0)
    {
    	  menu->addAction(QIcon("./icon/alignmentTopLeft.png"),"TopLeft", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    	  menu->addAction(QIcon("./icon/alignmentTopCenter.png"),"TopCenter", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    	  menu->addAction(QIcon("./icon/alignmentTopRight.png"),"TopRight", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    	  menu->addAction(QIcon("./icon/alignmentBottomRight.png"),"BottomRight", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    	  menu->addAction(QIcon("./icon/alignmentBottomLeft.png"),"BottomLeft", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
    }
    menu->popup(qcustomplot->mapToGlobal(pos));
}


void FigureWidget::slotAxisRectSizeChanged()
{
	if(qcustomplot->axisRect()->insetLayout()->insetPlacement(0) == QCPLayoutInset::ipBorderAligned)
	{
		//do nothing
	}
	else //QCPLayoutInset::ipFree
	{
		QSize axisRectSize = qcustomplot->axisRect()->insetLayout()->rect().size();
		legendWidthRatio = (double)(legendRectSize.width())/axisRectSize.width();
		legendHeightRatio = (double)(legendRectSize.height())/axisRectSize.height();

		qcustomplot->axisRect()->insetLayout()->setInsetRect(0,QRectF(legendPosRatio.x(),legendPosRatio.y(),legendWidthRatio,legendHeightRatio));
		qcustomplot->axisRect()->insetLayout()->update(QCPLayoutInset::upLayout);
		//qcustomplot->replot();
	}

}


void FigureWidget::saveas()
{
	QString fileName;
	if(saveFigureDialog->exec())
	{
		fileName = saveFigureDialog->selectedFiles().at(0);
	}
	if(fileName.isEmpty())
	{
		return;
	}
	//QString fileName = QFileDialog::getSaveFileName(this,tr("Save Figure Dialog"),QDir::currentPath()+tr("/untitled.png"),tr("Images(*.png *.jpg *.bmp *.pdf)")); //+tr("untitled.png")
	QString filter = saveFigureDialog->selectedNameFilter();
	if(filter == "PNG Images (*.png)")
	{
		if(QFileInfo(fileName).suffix() != "png")
			fileName = fileName.append(".png");
		if(!(qcustomplot->savePng(fileName)))
		{
			QMessageBox::information(this,tr("Failed to save image"),tr("Failed to save image!"));
			return;
		}
	}
	else if(filter == QString("JPG Images (*.jpg)"))
	{
		if(QFileInfo(fileName).suffix() != "jpg")
			fileName = fileName.append(".jpg");
		if(!(qcustomplot->saveJpg(fileName)))
		{
			QMessageBox::information(this,tr("Failed to save image"),tr("Failed to save image!"));
			return;
		}
	}
	else if(filter == "BMP Images (*.bmp)")
	{
		if(QFileInfo(fileName).suffix() != "bmp")
			fileName = fileName.append(".bmp");
		if(!(qcustomplot->saveBmp(fileName)))
		{
			QMessageBox::information(this,tr("Failed to save image"),tr("Failed to save image!"));
			return;
		}
	}
	else if(filter == "PDF Images (*.pdf)")
	{
		if(QFileInfo(fileName).suffix() != "pdf")
			fileName = fileName.append(".pdf");
		if(!(qcustomplot->savePdf(fileName,true)))
		{
			QMessageBox::information(this,tr("Failed to save image"),tr("Failed to save image!"));
			return;
		}
	}
	else{

	}

}

void FigureWidget::boxonoff(bool isOn)
{
	if(!isOn)
		boxon();
	else
		boxoff();
}
void FigureWidget::gridonoff(bool isOn)
{
	if(!isOn)
		gridon();
	else
		gridoff();
}
void FigureWidget::legendonoff(bool isOn)
{
	if(!isOn)
		legendon();
	else
		legendoff();
}

void FigureWidget::zoomonoff(bool isOn)
{
	if(!isOn)
	{
		iszoomon = false;
		qcustomplot->setInteraction(QCP::iRangeDrag,true);
	}
	else
	{
		iszoomon = true;
		qcustomplot->setInteraction(QCP::iRangeDrag,false);
		moveAct->setChecked(false);
	}
}

void FigureWidget::moveonoff(bool isOn)
{
	if(!isOn)
	{
		ismoveon = false;
	}
	else
	{
		ismoveon = true;
		qcustomplot->setInteraction(QCP::iRangeDrag,true);
		zoomAct->setChecked(false);
	}
}

void FigureWidget::axisequal()
{
	isaxisequalon = true;

	QCPAxis *rangeDragHorz = qcustomplot->axisRect()->rangeDragAxis(Qt::Horizontal);
	QCPAxis *rangeDragVert = qcustomplot->axisRect()->rangeDragAxis(Qt::Vertical);

	int widthWin = qcustomplot->axisRect()->width();
	int heightWin = qcustomplot->axisRect()->height();
	if(widthWin==0)
		widthWin=1;
	if(heightWin==0)
		heightWin=1;

	double horzLength = rangeDragHorz->range().size();
	double vertLength = rangeDragVert->range().size();
	double horzCenter = rangeDragHorz->range().center();
	double vertCenter = rangeDragVert->range().center();

	double heightEqual = horzLength/widthWin*heightWin;
	double widthEqual = 0.0;
	if(vertLength < heightEqual)
	{
		rangeDragVert->setRange(vertCenter - heightEqual/2.0, vertCenter + heightEqual/2.0);
	}
	else
	{
		widthEqual = vertLength/heightWin*widthWin;
		rangeDragHorz->setRange(horzCenter - widthEqual/2.0, horzCenter + widthEqual/2.0);
	}

	qcustomplot->replot();

}


void FigureWidget::simpleedit(bool isOn)
{
	if(isOn)
	{

		if(!simpleEdit)
		{
			simpleEdit = new SimpleEditWidget(this,qcustomplot);
			simpleEditWindow->setCentralWidget(simpleEdit);
			simpleEditWindow->show();
			simpleEditWindow->move(this->parentWidget()->frameGeometry().bottomLeft());
		}
		else
		{
			//simpleEdit->setVisible(true);
			simpleEditWindow->setVisible(true);
			//simpleEditWindow->move(200,300);
			simpleEditWindow->move(this->parentWidget()->frameGeometry().bottomLeft());
		}

	}
	else
	{
		if(simpleEdit)
		{
			simpleEditWindow->hide();
			//simpleEdit->hide();
			simpleEdit->slotSimpleEditOKClicked();
		}

	}
}

void FigureWidget::createActions()
{

    saveasAct = new QAction(QIcon("./icon/save-as.png"),tr("&SaveAs"), this);
    //saveasAct->setShortcuts(QKeySequence::SaveAs);
    saveasAct->setStatusTip(tr("Save the Single figure"));
    connect(saveasAct, SIGNAL(triggered()), this, SLOT(saveas()));

    boxAct = new QAction(QIcon("./icon/box.png"),tr("&Box"), this);
    boxAct->setStatusTip(tr("box on/off"));
    boxAct->setToolTip(tr("box on/off"));
    boxAct->setCheckable(true);
    connect(boxAct, SIGNAL(toggled(bool)), this, SLOT(boxonoff(bool)));

    gridAct = new QAction(QIcon("./icon/grid.png"),tr("&Grid"), this);
    gridAct->setStatusTip(tr("grid on/off"));
    gridAct->setToolTip(tr("grid on/off"));
    gridAct->setCheckable(true);
    connect(gridAct, SIGNAL(toggled(bool)), this, SLOT(gridonoff(bool)));

    legendAct = new QAction(QIcon("./icon/legend.png"),tr("&Legend"), this);
    legendAct->setStatusTip(tr("legend on/off"));
    legendAct->setToolTip(tr("legend on/off"));
    legendAct->setCheckable(true);
    connect(legendAct, SIGNAL(toggled(bool)), this, SLOT(legendonoff(bool)));

    zoomAct = new QAction(QIcon("./icon/zoom.png"),tr("&Zoom"), this);
    zoomAct->setStatusTip(tr("zoom seleted area"));
    zoomAct->setToolTip(tr("zoom seleted area"));
    zoomAct->setCheckable(true);
    connect(zoomAct, SIGNAL(toggled(bool)), this, SLOT(zoomonoff(bool)));

    moveAct = new QAction(QIcon("./icon/move.png"),tr("&Move"), this);
    moveAct->setStatusTip(tr("move graph ..."));
    moveAct->setToolTip(tr("move graph ..."));
    moveAct->setCheckable(true);
    connect(moveAct, SIGNAL(toggled(bool)), this, SLOT(moveonoff(bool)));


    axisequalAct = new QAction(QIcon("./icon/axisequal.png"),tr("&Axis Equal"), this);
    axisequalAct->setStatusTip(tr("Axis Equal"));
    axisequalAct->setToolTip(tr("Axis Equal"));
    connect(axisequalAct, SIGNAL(triggered()), this, SLOT(axisequal()));


    simpleeditAct = new QAction(QIcon("./icon/simple-edit.png"),tr("&Simple Edit"), this);
    simpleeditAct->setStatusTip(tr("Simple Edit"));
    simpleeditAct->setToolTip(tr("Simple Edit"));
    simpleeditAct->setCheckable(true);
    connect(simpleeditAct, SIGNAL(toggled(bool)), this, SLOT(simpleedit(bool)));

}

void FigureWidget::createTools()
{

	toolBarEdit = new QToolBar(tr("&Edit"));

	toolBarEdit->addAction(saveasAct);
	toolBarEdit->addAction(zoomAct);
	toolBarEdit->addAction(moveAct);
	toolBarEdit->addAction(boxAct);
	toolBarEdit->addAction(gridAct);
	toolBarEdit->addAction(legendAct);
	toolBarEdit->addAction(axisequalAct);
	toolBarEdit->addAction(simpleeditAct);

}

void FigureWidget::focusInEvent(QFocusEvent *event)
{
	emit signalFigureWidgetFocusIn(this);
}
