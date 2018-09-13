/*
 * FigureSubplotWindow.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: htallone
 */

#include "FigureSubplotWindow.h"
#include <QtWidgets>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <iostream>
#include "figurewindow.h"

int FigureSubplotWindow::figureCount = 0;

FigureSubplotWindow::FigureSubplotWindow(QWidget *parent)  : QMainWindow(parent) {
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    saveFigureDialog = new QFileDialog(this,tr("Save Figure Dialog"),QDir::currentPath()+tr("/untitled.png"),tr("PNG Images (*.png);;JPG Images (*.jpg);;BMP Images (*.bmp);;PDF Images (*.pdf)"));
    //saveFigureDialog->setOption(QFileDialog::DontUseNativeDialog);
    saveFigureDialog->setAcceptMode(QFileDialog::AcceptSave);
    saveFigureDialog->setWindowModality(Qt::WindowModal);
    saveFigureDialog->setFileMode(QFileDialog::AnyFile);
    saveFigureDialog->setNameFilterDetailsVisible(true);

    figureVec.clear();
    layoutGrid = new QGridLayout;

    layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->addLayout(layoutGrid);
    widget->setLayout(layout);
    this->setStyleSheet("QToolTip {background-color:white}");

    createActions();
    createMenus();
    createTools();
    toolBarEdit = 0;

    QString message = tr("Figure Window");
    statusBar()->showMessage(message);

    figureCount++;
    setWindowTitle(tr("Figure ")+QString::number(figureCount+FigureWindow::getFigureWindowCount()));
    setMinimumSize(160, 160);
    QRect deskRect = QApplication::desktop()->availableGeometry();
    int wit = 800;
    int hei = 600+this->menuBar()->height()+toolBarFile->height()+this->statusBar()->height() ;
    this->resize(wit,hei);
    this->move((deskRect.width()-wit)/2,(deskRect.height()-hei)/2 );



//    QDockWidget * testDock = new QDockWidget("para");
//    testDock->setFeatures(QDockWidget::DockWidgetClosable
//    		| QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
//
//    QLabel * te = new QLabel("df");
//    testDock->setWidget(te);
//    this->addDockWidget(Qt::RightDockWidgetArea,testDock);


	//this->show();

}

FigureSubplotWindow::~FigureSubplotWindow() {
	// TODO Auto-generated destructor stub
}

void FigureSubplotWindow::subplot(int row, int column, int rowSpan, int columnSpan)
{
	if(layoutGrid->itemAtPosition(row,column))
		figure = qobject_cast<FigureWidget *>(layoutGrid->itemAtPosition(row,column)->widget());
	else{
		figure = new FigureWidget(this);
		connect(figure, SIGNAL(signalFigureWidgetFocusIn(FigureWidget *)), this, SLOT(slotFigureWidgetFocusChanged(FigureWidget *)));
		figureVec.push_back(figure);
		layoutGrid->addWidget(figure,row,column,rowSpan,columnSpan);
	}

	if(!toolBarEdit)
	{
		toolBarEdit = figure->figureWidgetToolBar();
		addToolBar(toolBarEdit);
		toolBarEdit->show();
	}
	else
	{
		this->removeToolBar(toolBarEdit);
		toolBarEdit = figure->figureWidgetToolBar();
		addToolBar(toolBarEdit);
		toolBarEdit->show();
	}
}


bool FigureSubplotWindow::eventFilter(QObject * watched, QEvent * event)
{
//	if(watched == dockWidgetVec[0])
//	{
//		if(event->type() == QEvent::Close)
//		{
//			figure->axisequal();
//			return true;
//		}
//		else if(event->type() == QEvent::WindowDeactivate)
//		{
//		    QPalette palette;
//		    //palette.setColor(QPalette::Background,Qt::darkGray);
//		    dockWidgetVec[0]->titleBarWidget()->setPalette(palette);
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	}
//	else
//	{
//		return false;
//	}
}

//void FigureSubplotWindow::mouseDoubleClick(QMouseEvent * e)
//{
//
//}
//
//void FigureSubplotWindow::mousePress(QMouseEvent * e)
//{
//
//}
//
//void FigureSubplotWindow::mouseMove(QMouseEvent * e)
//{
//
//}
//void FigureSubplotWindow::mouseRelease(QMouseEvent * e)
//{
//
//}

void FigureSubplotWindow::slotFigureWidgetFocusChanged(FigureWidget * figureWidget)
{
	figure = figureWidget;
	if(!toolBarEdit)
	{
		toolBarEdit = figure->figureWidgetToolBar();
		addToolBar(toolBarEdit);
		toolBarEdit->show();
	}
	else
	{
		this->removeToolBar(toolBarEdit);
		toolBarEdit = figure->figureWidgetToolBar();
		addToolBar(toolBarEdit);
		toolBarEdit->show();
	}



//	QRect r1 = this->centralWidget()->geometry();
//	std::cout<<r1.x()<<"  "<<r1.y()<<"   "<<r1.width()<<"   "<<r1.height()<<std::endl;
//
//	QRect r2 = figure->geometry();
//	std::cout<<r2.x()<<"  "<<r2.y()<<"   "<<r2.width()<<"   "<<r2.height()<<std::endl;
//
//	QRect r3 = layoutGrid->geometry();
//	std::cout<<r3.x()<<"  "<<r3.y()<<"   "<<r3.width()<<"   "<<r3.height()<<std::endl;
}


void FigureSubplotWindow::newFile()
{

}

void FigureSubplotWindow::open()
{

}

void FigureSubplotWindow::save()
{

}

void FigureSubplotWindow::saveas()
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
		if(!(this->savePng(fileName)))
		{
			QMessageBox::information(this,tr("Failed to save image"),tr("Failed to save image!"));
			return;
		}
	}
	else if(filter == QString("JPG Images (*.jpg)"))
	{
		if(QFileInfo(fileName).suffix() != "jpg")
			fileName = fileName.append(".jpg");
		if(!(this->saveJpg(fileName)))
		{
			QMessageBox::information(this,tr("Failed to save image"),tr("Failed to save image!"));
			return;
		}
	}
	else if(filter == "BMP Images (*.bmp)")
	{
		if(QFileInfo(fileName).suffix() != "bmp")
			fileName = fileName.append(".bmp");
		if(!(this->saveBmp(fileName)))
		{
			QMessageBox::information(this,tr("Failed to save image"),tr("Failed to save image!"));
			return;
		}
	}
	else if(filter == "PDF Images (*.pdf)")
	{
		if(QFileInfo(fileName).suffix() != "pdf")
			fileName = fileName.append(".pdf");
		if(!(this->savePdf(fileName,true)))
		{
			QMessageBox::information(this,tr("Failed to save image"),tr("Failed to save image!"));
			return;
		}
	}
	else{

	}

}

void FigureSubplotWindow::print()
{
//	QString fileName;
//	if(saveFigureDialog->exec())
//	{
//		fileName = saveFigureDialog->selectedFiles().at(0);
//	}
//	if(fileName.isEmpty())
//	{
//		return;
//	}
//	//QString fileName = QFileDialog::getSaveFileName(this,tr("Save Figure Dialog"),QDir::currentPath()+tr("/untitled.png"),tr("Images(*.png *.jpg *.bmp *.pdf)")); //+tr("untitled.png")
//	QString filter = saveFigureDialog->selectedNameFilter();
//	if(filter == "PNG Images (*.png)")
//	{
//		if(QFileInfo(fileName).suffix() != "png")
//			fileName = fileName.append(".png");
//		if(!(qcustomplot->savePng(fileName)))
//		{
//			QMessageBox::information(this,tr("Failed to save image"),tr("Failed to save image!"));
//			return;
//		}
//	}
//	else if(filter == QString("JPG Images (*.jpg)"))
//	{
//		if(QFileInfo(fileName).suffix() != "jpg")
//			fileName = fileName.append(".jpg");
//		if(!(qcustomplot->saveJpg(fileName)))
//		{
//			QMessageBox::information(this,tr("Failed to save image"),tr("Failed to save image!"));
//			return;
//		}
//	}
//	else if(filter == "BMP Images (*.bmp)")
//	{
//		if(QFileInfo(fileName).suffix() != "bmp")
//			fileName = fileName.append(".bmp");
//		if(!(qcustomplot->saveBmp(fileName)))
//		{
//			QMessageBox::information(this,tr("Failed to save image"),tr("Failed to save image!"));
//			return;
//		}
//	}
//	else if(filter == "PDF Images (*.pdf)")
//	{
//		if(QFileInfo(fileName).suffix() != "pdf")
//			fileName = fileName.append(".pdf");
//		if(!(qcustomplot->savePdf(fileName,true)))
//		{
//			QMessageBox::information(this,tr("Failed to save image"),tr("Failed to save image!"));
//			return;
//		}
//	}
//	else{
//
//	}



//	if(fileName.isEmpty())
//	{
//		return;
//	}
//	else
//	{
//		//QPixmap pm = qApp->primaryScreen()->grabWindow(qApp->desktop()->winId(), this->x()+2, this->y()+2, this->frameGeometry().width()-4, this->frameGeometry().height()-4);
//		QPixmap pm = qcustomplot->grab();
//		if(!(pm.save(fileName)))
//		{
//			QMessageBox::information(this,tr("Failed to save image"),tr("Failed to save image!"));
//			return;
//		}
//	}

}

void FigureSubplotWindow::undo()
{

}

void FigureSubplotWindow::redo()
{

}

void FigureSubplotWindow::cut()
{

}

void FigureSubplotWindow::copy()
{

}

void FigureSubplotWindow::paste()
{

}

void FigureSubplotWindow::bold()
{

}

void FigureSubplotWindow::italic()
{

}

void FigureSubplotWindow::leftAlign()
{

}

void FigureSubplotWindow::rightAlign()
{

}

void FigureSubplotWindow::justify()
{

}

void FigureSubplotWindow::center()
{

}

void FigureSubplotWindow::setLineSpacing()
{

}

void FigureSubplotWindow::setParagraphSpacing()
{

}

void FigureSubplotWindow::about()
{

    QMessageBox::about(this, tr("About FigureWindow"),
            tr("The <b>FigureWindow</b>  "
               "use Qt."));
}

void FigureSubplotWindow::aboutQt()
{

}


void FigureSubplotWindow::createActions()
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

}

void FigureSubplotWindow::createMenus()
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

void FigureSubplotWindow::createTools()
{
	toolBarFile = addToolBar(tr("&File"));

	toolBarFile->addAction(saveasAct);

}

bool FigureSubplotWindow::saveRastered(const QString &fileName, int width, int height, double scale, const char *format, int quality)
{
	int newWidth, newHeight;
	if (width == 0 || height == 0)
	{
		newWidth = layoutGrid->geometry().width();
		newHeight = layoutGrid->geometry().height();
	} else
	{
		newWidth = width;
		newHeight = height;
	}
	int scaledWidth = qRound(scale*newWidth);
	int scaledHeight = qRound(scale*newHeight);

	QSize oldSize = this->centralWidget()->size();
	this->centralWidget()->resize(scaledWidth,scaledHeight);

	QPixmap pixmap(scaledWidth,scaledHeight);
	pixmap.fill(Qt::transparent);

	QPainter * painter = new QPainter(&pixmap);
	for(int i=0;i<layoutGrid->count();i++)
	{
		FigureWidget *	fw = qobject_cast<FigureWidget *>(layoutGrid->itemAt(i)->widget());
		QPixmap pixmaptemp = fw->figureWidgetQCustomPlot()->toPixmap(fw->geometry().width(),fw->geometry().height(),1);
		painter->drawPixmap(fw->geometry(),pixmaptemp,pixmaptemp.rect());
	}
	painter->end();

	this->centralWidget()->resize(oldSize);

	if (!pixmap.isNull())
		return pixmap.save(fileName, format, quality);
	else
		return false;
}

bool FigureSubplotWindow::savePng(const QString &fileName, int width, int height, double scale, int quality)
{
  return saveRastered(fileName, width, height, scale, "PNG", quality);
}
bool FigureSubplotWindow::saveJpg(const QString &fileName, int width, int height, double scale, int quality)
{
  return saveRastered(fileName, width, height, scale, "JPG", quality);
}
bool FigureSubplotWindow::saveBmp(const QString &fileName, int width, int height, double scale)
{
  return saveRastered(fileName, width, height, scale, "BMP");
}

bool FigureSubplotWindow::savePdf(const QString &fileName, bool noCosmeticPen, int width, int height, const QString &pdfCreator, const QString &pdfTitle)
{
//    QPixmap pixmap(layoutGrid->geometry().width(),layoutGrid->geometry().height());
//    pixmap.fill(Qt::transparent);
//
//    QVector<QPixmap> pixmapVec;
//    pixmapVec.clear();
//
//    QPainter * painter = new QPainter(&pixmap);
//
//	for(int i=0;i<layoutGrid->count();i++)
//	{
//		FigureWidget *	fw = qobject_cast<FigureWidget *>(layoutGrid->itemAt(i)->widget());
//		QPixmap pixmaptemp = fw->figureWidgetQCustomPlot()->toPixmap(figure->geometry().width(),figure->geometry().height());
//		painter->drawPixmap(figure->geometry(),pixmaptemp,QRect(0,0,pixmaptemp.width(),pixmaptemp.height()));
//	}
//
//	painter->end();

//  bool success = false;
//#ifdef QT_NO_PRINTER
//  Q_UNUSED(fileName)
//  Q_UNUSED(noCosmeticPen)
//  Q_UNUSED(width)
//  Q_UNUSED(height)
//  Q_UNUSED(pdfCreator)
//  Q_UNUSED(pdfTitle)
//  qDebug() << Q_FUNC_INFO << "Qt was built without printer support (QT_NO_PRINTER). PDF not created.";
//#else
//  int newWidth, newHeight;
//  if (width == 0 || height == 0)
//  {
//    newWidth = this->width();
//    newHeight = this->height();
//  } else
//  {
//    newWidth = width;
//    newHeight = height;
//  }
//  QPixmap p;
//  p.paintEngine()->drawPixmap()
//  QPrinter printer(QPrinter::ScreenResolution);
//  printer.setOutputFileName(fileName);
//  printer.setOutputFormat(QPrinter::PdfFormat);
//  printer.setColorMode(QPrinter::Color);
//  printer.printEngine()->setProperty(QPrintEngine::PPK_Creator, pdfCreator);
//  printer.printEngine()->setProperty(QPrintEngine::PPK_DocumentName, pdfTitle);
//  QRect oldViewport = this->centralWidget()->rect();
//  this->centralWidget()->setGeometry(QRect(0, 0, newWidth, newHeight));
//#if QT_VERSION < QT_VERSION_CHECK(5, 3, 0)
//  printer.setFullPage(true);
//  printer.setPaperSize(this->centralWidget()->geometry().size(), QPrinter::DevicePixel);
//#else
//  QPageLayout pageLayout;
//  pageLayout.setMode(QPageLayout::FullPageMode);
//  pageLayout.setOrientation(QPageLayout::Portrait);
//  pageLayout.setMargins(QMarginsF(0, 0, 0, 0));
//  pageLayout.setPageSize(QPageSize(this->centralWidget()->geometry().size(), QPageSize::Point, QString(), QPageSize::ExactMatch));
//  printer.setPageLayout(pageLayout);
//#endif
//  QCPPainter printpainter;
//  if (printpainter.begin(&printer))
//  {
//    printpainter.setMode(QCPPainter::pmVectorized);
//    printpainter.setMode(QCPPainter::pmNoCaching);
//    printpainter.setMode(QCPPainter::pmNonCosmetic, noCosmeticPen);
//    printpainter.setWindow(this->centralWidget()->geometry());
////    if (mBackgroundBrush.style() != Qt::NoBrush &&
////        mBackgroundBrush.color() != Qt::white &&
////        mBackgroundBrush.color() != Qt::transparent &&
////        mBackgroundBrush.color().alpha() > 0) // draw pdf background color if not white/transparent
////      printpainter.fillRect(viewport(), mBackgroundBrush);
//    draw(&printpainter);
//    printpainter.end();
//    success = true;
//  }
//  setViewport(oldViewport);
//#endif // QT_NO_PRINTER
//  return success;
}



