

#include "figurewindow.h"
#include "FigureSubplotWindow.h"

#include <QtWidgets>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <iostream>
#include "PlottablesEditWidget.h"
#include "AxisEditWidget.h"
#include "LegendEditWidget.h"


int FigureWindow::figureCount = 0;

FigureWindow::FigureWindow(QWidget *parent)  : QMainWindow(parent)
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    qcustomplot = new QCustomPlot;
    qcustomplot->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
//    qcustomplot->axisRect()->insetLayout()->setInsetPlacement(0,QCPLayoutInset::ipFree);
//    legendPosRatio = QPointF(0,0);
//    qcustomplot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
//    qcustomplot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    qcustomplot->xAxis->setNumberFormat("gbc");
    qcustomplot->yAxis->setNumberFormat("gbc");
    connect(qcustomplot, SIGNAL(mouseDoubleClick(QMouseEvent*)), this, SLOT(mouseDoubleClick(QMouseEvent*)));
    connect(qcustomplot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent* )));
    connect(qcustomplot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMove(QMouseEvent* )));
    connect(qcustomplot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseRelease(QMouseEvent* )));
	qcustomplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	qcustomplot->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(qcustomplot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
	connect(qcustomplot->axisRect(), SIGNAL(layoutElementRectSizeChanged()), this, SLOT(slotAxisRectSizeChanged()));

	imageSaveWidget = 0;

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
    layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addWidget(qcustomplot);
    widget->setLayout(layout);
    this->setStyleSheet("QToolTip {background-color:white}");


	dock = new QDockWidget("Simple Edit");
	dock->setFeatures(QDockWidget::DockWidgetClosable
			| QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);

	this->addDockWidget(Qt::BottomDockWidgetArea,dock);
	dock->hide();

    createActions();
    createMenus();
    createTools();

    QString message = tr("Figure Window");
    statusBar()->showMessage(message);

    figureCount++;
    setWindowTitle(tr("Figure ")+QString::number(figureCount+FigureSubplotWindow::getFigureSubplotWindowCount()));
    setMinimumSize(160, 160);
    
    QRect deskRect = QApplication::desktop()->availableGeometry();
    int wit = 400;
    int hei = 300+this->menuBar()->height()+toolBarEdit->height()+this->statusBar()->height() ;
    this->resize(wit,hei);
    this->move((deskRect.width()-wit)/2,(deskRect.height()-hei)/2 );




	//this->show();
}

FigureWindow::~FigureWindow()
{

}

void FigureWindow::plot(QVector<double> x, QVector<double> y)
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

void FigureWindow::plot(QVector<double> x, QVector<double> y, QPen pen, const QString & legendname)
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

void FigureWindow::plot(QVector<double> x, QVector<double> y, QPen pen, const QCPScatterStyle & style, const QString & legendname)
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

void FigureWindow::plotScatters(QVector<double> x, QVector<double> y, const QCPScatterStyle & style, const QString & legendname)
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


void FigureWindow::setAxisRangeX(double lower, double upper)
{
	qcustomplot->axisRect()->rangeDragAxis(Qt::Horizontal)->setRange(lower,upper);
}
void FigureWindow::setAxisRangeY(double lower, double upper)
{
	qcustomplot->axisRect()->rangeDragAxis(Qt::Vertical)->setRange(lower,upper);
}
void FigureWindow::setAxisLabelX(const QString &str)
{
	qcustomplot->xAxis->setLabel(str);
}
void FigureWindow::setAxisLabelY(const QString &str)
{
	qcustomplot->yAxis->setLabel(str);
}
void FigureWindow::setAxisLabelX(const QString &str, const QFont &font, const QColor &color)
{
	qcustomplot->xAxis->setLabel(str);
	qcustomplot->xAxis->setLabelFont(font);
	qcustomplot->xAxis->setLabelColor(color);

}
void FigureWindow::setAxisLabelY(const QString &str, const QFont &font, const QColor &color)
{
	qcustomplot->yAxis->setLabel(str);
	qcustomplot->yAxis->setLabelFont(font);
	qcustomplot->yAxis->setLabelColor(color);
}
void FigureWindow::setAxisLabelFontX( const QFont &font, const QColor &color)
{
	qcustomplot->xAxis->setLabelFont(font);
	qcustomplot->xAxis->setLabelColor(color);
}
void FigureWindow::setAxisLabelFontY( const QFont &font, const QColor &color)
{
	qcustomplot->yAxis->setLabelFont(font);
	qcustomplot->yAxis->setLabelColor(color);
}
void FigureWindow::setAxisLabelFont( const QFont &font, const QColor &color)
{
	qcustomplot->xAxis->setLabelFont(font);
	qcustomplot->xAxis->setLabelColor(color);
	qcustomplot->yAxis->setLabelFont(font);
	qcustomplot->yAxis->setLabelColor(color);
}
void FigureWindow::setAxisTicksLabelFontX( const QFont &font, const QColor &color)
{
	qcustomplot->xAxis->setTickLabelFont(font);
	qcustomplot->xAxis->setTickLabelColor(color);
}
void FigureWindow::setAxisTicksLabelFontY( const QFont &font, const QColor &color)
{
	qcustomplot->yAxis->setTickLabelFont(font);
	qcustomplot->yAxis->setTickLabelColor(color);
}
void FigureWindow::setAxisTicksLabelFont( const QFont &font, const QColor &color)
{
	qcustomplot->xAxis->setTickLabelFont(font);
	qcustomplot->xAxis->setTickLabelColor(color);
	qcustomplot->yAxis->setTickLabelFont(font);
	qcustomplot->yAxis->setTickLabelColor(color);
}
void FigureWindow::setLegendFont( const QFont &font, const QColor &color)
{
	qcustomplot->legend->setFont(font);
	qcustomplot->legend->setTextColor(color);
}


void FigureWindow::holdon()
{
	isholdon = true;
}
void FigureWindow::holdoff()
{
	isholdon = false;
}
void FigureWindow::legendon()
{
	islegendon = true;
	if(qcustomplot->legend->itemCount()==0)
		qcustomplot->legend->setVisible(false);
	else
		qcustomplot->legend->setVisible(true);
	qcustomplot->replot();
}
void FigureWindow::legendoff()
{
	islegendon = false;
	qcustomplot->legend->setVisible(false);
	qcustomplot->replot();
}
void FigureWindow::gridon()
{
	isgridon = true;
	qcustomplot->xAxis->grid()->setVisible(true);
	qcustomplot->yAxis->grid()->setVisible(true);
	qcustomplot->replot();
}
void FigureWindow::gridoff()
{
	isgridon = false;
	qcustomplot->xAxis->grid()->setVisible(false);
	qcustomplot->yAxis->grid()->setVisible(false);
	qcustomplot->replot();
}
void FigureWindow::boxon()
{
	isboxon = true;
	qcustomplot->axisRect()->setVisible(true);
	qcustomplot->axisRect()->setupFullAxesBox(true);
//	qcustomplot->xAxis2->setTicks(false);
//	qcustomplot->yAxis2->setTicks(false);
	qcustomplot->replot();
}
void FigureWindow::boxoff()
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


void FigureWindow::mouseDoubleClick(QMouseEvent * e)
{

	isMouseDoubleClick = true;

	if(e->button()==Qt::LeftButton)
	{
		if(qcustomplot->axisRect()->rect().contains(e->pos()) && (qcustomplot->axisRect()->insetLayout()->selectTest(e->pos(),false) <0) )
			qcustomplot->rescaleAxes();
		qcustomplot->replot();
	}
}

void FigureWindow::mousePress(QMouseEvent * e)
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

void FigureWindow::mouseMove(QMouseEvent * e)
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
void FigureWindow::mouseRelease(QMouseEvent * e)
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


void FigureWindow::moveLegend()
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


void FigureWindow::contextMenuRequest(QPoint pos)
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


void FigureWindow::slotAxisRectSizeChanged()
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

void FigureWindow::newFile()
{

}

void FigureWindow::open()
{

}

void FigureWindow::save()
{

}

void FigureWindow::saveas()
{

	if(!imageSaveWidget)
		imageSaveWidget = new ImageSaveWidget(this,qcustomplot);
	imageSaveWidget->show();



}

void FigureWindow::print()
{


}

void FigureWindow::undo()
{

}

void FigureWindow::redo()
{

}

void FigureWindow::cut()
{

}

void FigureWindow::copy()
{

}

void FigureWindow::paste()
{

}

void FigureWindow::bold()
{

}

void FigureWindow::italic()
{

}

void FigureWindow::leftAlign()
{

}

void FigureWindow::rightAlign()
{

}

void FigureWindow::justify()
{

}

void FigureWindow::center()
{

}

void FigureWindow::setLineSpacing()
{

}

void FigureWindow::setParagraphSpacing()
{

}

void FigureWindow::about()
{

    QMessageBox::about(this, tr("About FigureWindow"),
            tr("The <b>FigureWindow</b>  "
               "use Qt."));
}

void FigureWindow::aboutQt()
{

}


void FigureWindow::boxonoff(bool isOn)
{
	if(!isOn)
		boxon();
	else
		boxoff();
}
void FigureWindow::gridonoff(bool isOn)
{
	if(!isOn)
		gridon();
	else
		gridoff();
}
void FigureWindow::legendonoff(bool isOn)
{
	if(!isOn)
		legendon();
	else
		legendoff();
}

void FigureWindow::zoomonoff(bool isOn)
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

void FigureWindow::moveonoff(bool isOn)
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

void FigureWindow::axisequal()
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


void FigureWindow::simpleedit(bool isOn)
{
	if(isOn)
	{

		windowGeometryRectOld = this->geometry();
		QRect newGeometryRect = windowGeometryRectOld;
		if(windowGeometryRectOld.width()<680)
			newGeometryRect.setWidth(680);
		newGeometryRect.setHeight(windowGeometryRectOld.height()+220);

		if(!simpleEdit)
		{
			simpleEdit = new SimpleEditWidget(this,qcustomplot);
			//layout->addWidget(simpleEdit);
			//simpleEdit->setVisible(true);

			dock->setWidget(simpleEdit);
			dock->show();

		}
		else
		{
			//simpleEdit->setVisible(true);

			dock->show();
		}

		this->resize(newGeometryRect.width(),newGeometryRect.height());

	}
	else
	{
		if(simpleEdit)
		{
			//simpleEdit->hide();
			simpleEdit->slotSimpleEditOKClicked();

			dock->hide();

		}
		else
		{
			dock->hide();
		}
		this->showNormal();
		this->resize(windowGeometryRectOld.width(),windowGeometryRectOld.height());
	}
}

void FigureWindow::createActions()
{
    newAct = new QAction(QIcon("./icon/file-new.png"),tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveasAct = new QAction(QIcon("./icon/save-as.png"),tr("&SaveAs"), this);
    saveasAct->setShortcuts(QKeySequence::SaveAs);
    saveasAct->setStatusTip(tr("Save the figure"));
    connect(saveasAct, SIGNAL(triggered()), this, SLOT(saveas()));

    printAct = new QAction(QIcon("./icon/print.png"),tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the figure"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last operation"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last operation"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    cutAct = new QAction(tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    boldAct = new QAction(tr("&Bold"), this);
    boldAct->setCheckable(true);
    boldAct->setShortcut(QKeySequence::Bold);
    boldAct->setStatusTip(tr("Make the text bold"));
    connect(boldAct, SIGNAL(triggered()), this, SLOT(bold()));

    QFont boldFont = boldAct->font();
    boldFont.setBold(true);
    boldAct->setFont(boldFont);

    italicAct = new QAction(tr("&Italic"), this);
    italicAct->setCheckable(true);
    italicAct->setShortcut(QKeySequence::Italic);
    italicAct->setStatusTip(tr("Make the text italic"));
    connect(italicAct, SIGNAL(triggered()), this, SLOT(italic()));

    QFont italicFont = italicAct->font();
    italicFont.setItalic(true);
    italicAct->setFont(italicFont);

    setLineSpacingAct = new QAction(tr("Set &Line Spacing..."), this);
    setLineSpacingAct->setStatusTip(tr("Change the gap between the lines of a "
                                       "paragraph"));
    connect(setLineSpacingAct, SIGNAL(triggered()), this, SLOT(setLineSpacing()));

    setParagraphSpacingAct = new QAction(tr("Set &Paragraph Spacing..."), this);
    setParagraphSpacingAct->setStatusTip(tr("Change the gap between paragraphs"));
    connect(setParagraphSpacingAct, SIGNAL(triggered()),
            this, SLOT(setParagraphSpacing()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(aboutQt()));

    leftAlignAct = new QAction(tr("&Left Align"), this);
    leftAlignAct->setCheckable(true);
    leftAlignAct->setShortcut(tr("Ctrl+L"));
    leftAlignAct->setStatusTip(tr("Left align the selected text"));
    connect(leftAlignAct, SIGNAL(triggered()), this, SLOT(leftAlign()));

    rightAlignAct = new QAction(tr("&Right Align"), this);
    rightAlignAct->setCheckable(true);
    rightAlignAct->setShortcut(tr("Ctrl+R"));
    rightAlignAct->setStatusTip(tr("Right align the selected text"));
    connect(rightAlignAct, SIGNAL(triggered()), this, SLOT(rightAlign()));

    justifyAct = new QAction(tr("&Justify"), this);
    justifyAct->setCheckable(true);
    justifyAct->setShortcut(tr("Ctrl+J"));
    justifyAct->setStatusTip(tr("Justify the selected text"));
    connect(justifyAct, SIGNAL(triggered()), this, SLOT(justify()));

    centerAct = new QAction(tr("&Center"), this);
    centerAct->setCheckable(true);
    centerAct->setShortcut(tr("Ctrl+E"));
    centerAct->setStatusTip(tr("Center the selected text"));
    connect(centerAct, SIGNAL(triggered()), this, SLOT(center()));

    alignmentGroup = new QActionGroup(this);
    alignmentGroup->addAction(leftAlignAct);
    alignmentGroup->addAction(rightAlignAct);
    alignmentGroup->addAction(justifyAct);
    alignmentGroup->addAction(centerAct);
    leftAlignAct->setChecked(true);

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

void FigureWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
//    fileMenu->addAction(newAct);
//    fileMenu->addAction(openAct);
//    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveasAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(copyAct);
    editMenu->addSeparator();
    editMenu->addAction(zoomAct);
    editMenu->addAction(moveAct);
    editMenu->addAction(boxAct);
    editMenu->addAction(gridAct);
    editMenu->addAction(legendAct);
    editMenu->addAction(axisequalAct);
    editMenu->addAction(simpleeditAct);
    editMenu->addAction(dock->toggleViewAction());

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

//    formatMenu = editMenu->addMenu(tr("&Format"));
//    formatMenu->addAction(boldAct);
//    formatMenu->addAction(italicAct);
//    formatMenu->addSeparator()->setText(tr("Alignment"));
//    formatMenu->addAction(leftAlignAct);
//    formatMenu->addAction(rightAlignAct);
//    formatMenu->addAction(justifyAct);
//    formatMenu->addAction(centerAct);
//    formatMenu->addSeparator();
//    formatMenu->addAction(setLineSpacingAct);
//    formatMenu->addAction(setParagraphSpacingAct);
}

void FigureWindow::createTools()
{
	toolBarFile = addToolBar(tr("&File"));
	toolBarEdit = addToolBar(tr("&Edit"));

	toolBarFile->addAction(saveasAct);

	toolBarEdit->addAction(zoomAct);
	toolBarEdit->addAction(moveAct);
	toolBarEdit->addAction(boxAct);
	toolBarEdit->addAction(gridAct);
	toolBarEdit->addAction(legendAct);
	toolBarEdit->addAction(axisequalAct);
	toolBarEdit->addAction(simpleeditAct);

}

