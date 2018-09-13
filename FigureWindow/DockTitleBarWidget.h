/*
 * DockTitleBarWidget.h
 *
 *  Created on: Jun 16, 2016
 *      Author: htallone
 */

#ifndef SRC_DOCKTITLEBARWIDGET_H_
#define SRC_DOCKTITLEBARWIDGET_H_

#include <QObject>
#include <QtWidgets>
#include <QPixmap>

class DockTitleBarWidget:public QWidget {

	Q_OBJECT

public:
	DockTitleBarWidget(QWidget * parent = 0);
	virtual ~DockTitleBarWidget();

	QSize sizeHint() const {return minimumSizeHint();}
	QSize minimumSizeHint() const;

	void setDockWindowTitle(const QString & titleName);
	void setDockWindowIcon(const QPixmap & iconQPixmap);
signals:
	void signalDockWindowClose();
	void signalDockWindowFloatout();
	void signalDockWindowDockin();
protected:

public slots:

private slots:

	void slotDockWindowCloseButtonClick();
	void slotDockWindowMinimizeButtonClick();
	void slotDockWindowMaximizeButtonClick();
	void slotDockWindowRestoreButtonClick();
	void slotDockWindowFloatoutButtonClick();
	void slotDockWindowDockinButtonClick();
private:

	QRect restoreGeo;

	QPixmap windowminimize, windowmaximize, windowclose, windowrestore, windowfloatout, windowdockin;
	QLabel * titleLabel;
	QLabel * iconLabel;
	QPushButton * minimizeButton;
	QPushButton * maximizeButton;
	QPushButton * closeButton;
	QPushButton * restoreButton;
	QPushButton * floatoutButton;
	QPushButton * dockinButton;


};

#endif /* SRC_DOCKTITLEBARWIDGET_H_ */
