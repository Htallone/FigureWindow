/*
 * FigureWidget.h
 *
 *  Created on: Jun 15, 2016
 *      Author: htallone
 */

#ifndef SRC_FIGUREWIDGET_H_
#define SRC_FIGUREWIDGET_H_

#include <QtWidgets>
#include "qcustomplot/qcustomplot.h"
#include "SimpleEditWidget.h"

class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QFileDialog;

class FigureWidget : public QWidget{

	Q_OBJECT

public:
	FigureWidget(QWidget * parent = 0);
	virtual ~FigureWidget();

    void plot(QVector<double> x, QVector<double> y);
    void plot(QVector<double> x, QVector<double> y, QPen pen, const QString & legendname = "");
    void plot(QVector<double> x, QVector<double> y, QPen pen, const QCPScatterStyle & style, const QString & legendname = "");

    void plotScatters(QVector<double> x, QVector<double> y, const QCPScatterStyle & style, const QString & legendname = "");



    void setAxisRangeX(double lower, double upper);
    void setAxisRangeY(double lower, double upper);
    void setAxisLabelX(const QString &str);
    void setAxisLabelY(const QString &str);
    void setAxisLabelX(const QString &str, const QFont &font, const QColor &color = Qt::black);
    void setAxisLabelY(const QString &str, const QFont &font, const QColor &color = Qt::black);
    void setAxisLabelFontX( const QFont &font, const QColor &color = Qt::black);
    void setAxisLabelFontY( const QFont &font, const QColor &color = Qt::black);
    void setAxisLabelFont( const QFont &font, const QColor &color = Qt::black);
    void setAxisTicksLabelFontX( const QFont &font, const QColor &color = Qt::black);
    void setAxisTicksLabelFontY( const QFont &font, const QColor &color = Qt::black);
    void setAxisTicksLabelFont( const QFont &font, const QColor &color = Qt::black);
    void setLegendFont( const QFont &font, const QColor &color = Qt::black);

    void setTitle(const QString &str);
    void setTitle(const QString &str, const QFont &font, const QColor &color = Qt::black);

    void holdon();
    void holdoff();
    void legendon();
    void legendoff();
    void gridon();
    void gridoff();
    void boxon();
    void boxoff();


    inline QToolBar * figureWidgetToolBar() {return toolBarEdit;}
    inline QCustomPlot * figureWidgetQCustomPlot() {return qcustomplot;}


    signals:
	void signalFigureWidgetFocusIn(FigureWidget * figureWidget);

protected:

    void focusInEvent(QFocusEvent *event);

public slots:
	void axisequal();
	void slotAxisRectSizeChanged();

private slots:

	void saveas();
    void boxonoff(bool isOn);
    void gridonoff(bool isOn);
    void legendonoff(bool isOn);
    void zoomonoff(bool isOn);
    void moveonoff(bool isOn);
    void simpleedit(bool );


    void mouseDoubleClick(QMouseEvent * e);
    void mousePress(QMouseEvent * e);
    void mouseMove(QMouseEvent * e);
    void mouseRelease(QMouseEvent * e);
    void moveLegend();
    void contextMenuRequest(QPoint pos);


private:

    void createActions();
    void createTools();

    QToolBar *toolBarEdit;

    QAction *saveasAct;
    QAction *boxAct;
    QAction *gridAct;
    QAction *legendAct;
    QAction *zoomAct;
    QAction *moveAct;
    QAction *axisequalAct;
    QAction *simpleeditAct;

    SimpleEditWidget * simpleEdit;
    QMainWindow * simpleEditWindow;

    bool isMouseDoubleClick;

    QPoint mousePressPos;
    QPoint mouseReleasePos;
    bool isMouseDownMove;

    QSize legendRectSize;
    QPoint legendRectPos;
    QPointF legendPosRatio;
    double legendWidthRatio;
    double legendHeightRatio;
    bool isLegendMove;

    QRubberBand *rubberBand;

    QCustomPlot *qcustomplot;
    bool isholdon;
    bool islegendon;
    bool isgridon;
    bool isboxon;
    bool iszoomon;
    bool ismoveon;
    bool isaxisequalon;

    QFileDialog *saveFigureDialog;

};

#endif /* SRC_FIGUREWIDGET_H_ */
