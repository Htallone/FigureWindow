

#ifndef FIGUREWINDOW_H
#define FIGUREWINDOW_H

#include <QMainWindow>
#include "qcustomplot/qcustomplot.h"
#include "SimpleEditWidget.h"
#include "ImageSaveWidget.h"

class QAction;
class QActionGroup;
class QLabel;
class QMenu;


class FigureWindow : public QMainWindow
{
    Q_OBJECT

public:
	FigureWindow(QWidget *parent = 0);
	virtual ~FigureWindow();

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


    void holdon();
    void holdoff();
    void legendon();
    void legendoff();
    void gridon();
    void gridoff();
    void boxon();
    void boxoff();


    inline QCustomPlot * figureWindowQCustomPlot() {return qcustomplot;}
    inline static int getFigureWindowCount() {return figureCount;}
    inline QMenu * getFigureWindowEditMenu() {return editMenu;}
    inline SimpleEditWidget *  getFigureWindowSimpleEditWidget() {return simpleEdit;}

protected:

public slots:
	void axisequal();
	void slotAxisRectSizeChanged();

private slots:
    void newFile();
    void open();
    void save();
    void saveas();
    void print();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void bold();
    void italic();
    void leftAlign();
    void rightAlign();
    void justify();
    void center();
    void setLineSpacing();
    void setParagraphSpacing();
    void about();
    void aboutQt();

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
    void createMenus();
    void createTools();

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;

    QToolBar *toolBarFile;
    QToolBar *toolBarEdit;

    QActionGroup *alignmentGroup;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveasAct;
    QAction *printAct;
    QAction *exitAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *boldAct;
    QAction *italicAct;
    QAction *leftAlignAct;
    QAction *rightAlignAct;
    QAction *justifyAct;
    QAction *centerAct;
    QAction *setLineSpacingAct;
    QAction *setParagraphSpacingAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    QAction *boxAct;
    QAction *gridAct;
    QAction *legendAct;
    QAction *zoomAct;
    QAction *moveAct;
    QAction *axisequalAct;
    QAction *simpleeditAct;

    QVBoxLayout *layout;
    SimpleEditWidget * simpleEdit;
    QDockWidget * dock;

    bool isMouseDoubleClick;

    static int figureCount;
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


    ImageSaveWidget * imageSaveWidget;

    QCustomPlot *qcustomplot;
    bool isholdon;
    bool islegendon;
    bool isgridon;
    bool isboxon;
    bool iszoomon;
    bool ismoveon;
    bool isaxisequalon;

    QRect windowGeometryRectOld;



};

#endif
