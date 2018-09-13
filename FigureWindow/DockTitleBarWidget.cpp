/*
 * DockTitleBarWidget.cpp
 *
 *  Created on: Jun 16, 2016
 *      Author: htallone
 */

#include "DockTitleBarWidget.h"

DockTitleBarWidget::DockTitleBarWidget(QWidget * parent) : QWidget(parent) {

	titleLabel = new QLabel;
	iconLabel = new QLabel;
	minimizeButton = new QPushButton;
	maximizeButton = new QPushButton;
	closeButton    = new QPushButton;
	restoreButton  = new QPushButton;
	floatoutButton = new QPushButton;
	dockinButton   = new QPushButton;

	connect(minimizeButton, SIGNAL(clicked()), this, SLOT(slotDockWindowMinimizeButtonClick()));
	connect(maximizeButton, SIGNAL(clicked()), this, SLOT(slotDockWindowMaximizeButtonClick()));
	connect(closeButton   , SIGNAL(clicked()), this, SLOT(slotDockWindowCloseButtonClick()));
	connect(restoreButton , SIGNAL(clicked()), this, SLOT(slotDockWindowRestoreButtonClick()));
	connect(floatoutButton, SIGNAL(clicked()), this, SLOT(slotDockWindowFloatoutButtonClick()));
	connect(dockinButton  , SIGNAL(clicked()), this, SLOT(slotDockWindowDockinButtonClick()));


	iconLabel->setPixmap(QPixmap("./icon/window-minimize.png"));
	iconLabel->setFixedSize(30,30);


	windowminimize = QPixmap("./icon/window-minimize-symbolic.svg");
	windowmaximize = QPixmap("./icon/window-maximize-symbolic.svg");
	windowclose    = QPixmap("./icon/window-close-symbolic.svg");
	windowrestore  = QPixmap("./icon/window-restore-symbolic.svg");
	windowfloatout = QPixmap("./icon/window-floatout.png");
	windowdockin   = QPixmap("./icon/window-dockin.png");

	QSize fixSize = QSize(32,32);

	minimizeButton->setFixedSize(fixSize);
	maximizeButton->setFixedSize(fixSize);
	closeButton   ->setFixedSize(fixSize);
	restoreButton ->setFixedSize(fixSize);
	floatoutButton->setFixedSize(fixSize);
	dockinButton  ->setFixedSize(fixSize);

	minimizeButton->setIcon(QIcon(windowminimize));
	maximizeButton->setIcon(QIcon(windowmaximize));
	closeButton   ->setIcon(QIcon(windowclose   ));
	restoreButton ->setIcon(QIcon(windowrestore ));
	floatoutButton->setIcon(QIcon(windowfloatout));
	dockinButton  ->setIcon(QIcon(windowdockin  ));

	minimizeButton->setFlat(true);
	maximizeButton->setFlat(true);
	closeButton   ->setFlat(true);
	restoreButton ->setFlat(true);
	floatoutButton->setFlat(true);
	dockinButton  ->setFlat(true);

	minimizeButton->setFocusPolicy(Qt::NoFocus);
	maximizeButton->setFocusPolicy(Qt::NoFocus);
	closeButton   ->setFocusPolicy(Qt::NoFocus);
	restoreButton ->setFocusPolicy(Qt::NoFocus);
	floatoutButton->setFocusPolicy(Qt::NoFocus);
	dockinButton  ->setFocusPolicy(Qt::NoFocus);

	QHBoxLayout *layoutH = new QHBoxLayout;
	layoutH->addWidget(floatoutButton);
	layoutH->addWidget(dockinButton);
	//layoutH->addWidget(minimizeButton);
	layoutH->addWidget(restoreButton);
	layoutH->addWidget(maximizeButton);
	layoutH->addWidget(closeButton);
    QGridLayout *layoutGrid = new QGridLayout;
    layoutGrid->addWidget(iconLabel,0,0,Qt::AlignLeft);
    layoutGrid->addWidget(titleLabel,0,1,Qt::AlignCenter);
    layoutGrid->addLayout(layoutH,0,2,Qt::AlignRight);
    layoutGrid->setSpacing(0);
    layoutGrid->setMargin(0);
    this->setLayout(layoutGrid);
    this->setFixedHeight(40);

	 dockinButton->hide();
	 minimizeButton->hide();
	 maximizeButton->hide();
	 restoreButton->hide();

    this->setFocusPolicy(Qt::NoFocus);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background,Qt::darkGray);
    this->setPalette(palette);
//    QPalette palette1;
//    this->setPalette(palette1);
}


DockTitleBarWidget::~DockTitleBarWidget() {
	// TODO Auto-generated destructor stub
}

QSize DockTitleBarWidget::minimumSizeHint() const
{
	QSize returnSize(200,40);

	return returnSize;
}

void DockTitleBarWidget::setDockWindowTitle(const QString & titleName)
{
	titleLabel->setText(titleName);
}
void DockTitleBarWidget::setDockWindowIcon(const QPixmap & iconQPixmap)
{
	iconLabel->setPixmap(iconQPixmap);
}

void DockTitleBarWidget::slotDockWindowCloseButtonClick()
{
	 QDockWidget *dockWidget = qobject_cast<QDockWidget*>(parentWidget());
	 if(dockWidget->close())
		 emit signalDockWindowClose();

}
void DockTitleBarWidget::slotDockWindowMinimizeButtonClick()
{
//	 QDockWidget *dockWidget = qobject_cast<QDockWidget*>(parentWidget());
//	 dockWidget->showMinimized();
//	 //dockWidget->setWindowState(Qt::WindowMinimized);
}
void DockTitleBarWidget::slotDockWindowMaximizeButtonClick()
{
	 QDockWidget *dockWidget = qobject_cast<QDockWidget*>(parentWidget());
	 restoreGeo = dockWidget->geometry();
	 dockWidget->showMaximized();
	 restoreButton->show();
	 maximizeButton->hide();
	 if(dockWidget->isFloating())
	 {
		 floatoutButton->hide();
		 dockinButton->show();
	 }
	 else
	 {
		 floatoutButton->show();
		 dockinButton->hide();
	 }
}
void DockTitleBarWidget::slotDockWindowRestoreButtonClick()
{
	 QDockWidget *dockWidget = qobject_cast<QDockWidget*>(parentWidget());
	 dockWidget->setGeometry(restoreGeo);
	 //dockWidget->showNormal();
	 maximizeButton->show();
	 restoreButton->hide();
	 if(dockWidget->isFloating())
	 {
		 floatoutButton->hide();
		 dockinButton->show();
	 }
	 else
	 {
		 floatoutButton->show();
		 dockinButton->hide();
	 }
}
void DockTitleBarWidget::slotDockWindowFloatoutButtonClick()
{
	this->setFocus();
	 QDockWidget *dockWidget = qobject_cast<QDockWidget*>(parentWidget());
	 dockWidget->setFloating(true);
	 dockWidget->showNormal();
	 floatoutButton->hide();
	 dockinButton->show();
	 dockinButton->clearFocus();
	 floatoutButton->clearFocus();
	 if(dockWidget->isMaximized())
	 {
		 maximizeButton->hide();
		 restoreButton->show();
	 }
	 else
	 {
		 maximizeButton->show();
		 restoreButton->hide();
	 }
}
void DockTitleBarWidget::slotDockWindowDockinButtonClick()
{
	this->setFocus();
	 QDockWidget *dockWidget = qobject_cast<QDockWidget*>(parentWidget());
	 dockWidget->setFloating(false);
	 floatoutButton->show();
	 dockinButton->clearFocus();
	 floatoutButton->clearFocus();
	 dockinButton->hide();
	 maximizeButton->hide();
	 restoreButton->hide();
}

