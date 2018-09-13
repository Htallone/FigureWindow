/*
 * FigureSubplotWindow.h
 *
 *  Created on: Jun 17, 2016
 *      Author: htallone
 */

#ifndef SRC_FIGURESUBPLOTWINDOW_H_
#define SRC_FIGURESUBPLOTWINDOW_H_

#include <QMainWindow>
#include "FigureWidget.h"


class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QFileDialog;

class FigureSubplotWindow : public QMainWindow {

	Q_OBJECT

public:
	FigureSubplotWindow(QWidget *parent = 0);
	virtual ~FigureSubplotWindow();


	void subplot(int row, int column, int rowSpan = 1, int columnSpan = 1);

	inline FigureWidget* currentFigure() {return figure;}
	inline static int getFigureSubplotWindowCount(){return figureCount;}

    bool saveRastered(const QString &fileName, int width, int height, double scale, const char *format, int quality = -1);
    bool savePng(const QString &fileName, int width=0, int height=0, double scale=1.0, int quality=-1);
    bool saveJpg(const QString &fileName, int width=0, int height=0, double scale=1.0, int quality=-1);
    bool saveBmp(const QString &fileName, int width=0, int height=0, double scale=1.0);

    bool savePdf(const QString &fileName, bool noCosmeticPen=false, int width=0, int height=0, const QString &pdfCreator=QString(), const QString &pdfTitle=QString());
protected:

    bool eventFilter(QObject * o, QEvent * e);

public slots:

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

    //void mousePress(QMouseEvent * e);
    void slotFigureWidgetFocusChanged(FigureWidget * figureWidget);

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



    QVBoxLayout *layout;
    QGridLayout *layoutGrid;


    static int figureCount;

    QFileDialog *saveFigureDialog;

    QCustomPlot *qcustomplot;
    FigureWidget *figure;
    QVector<FigureWidget *> figureVec;

};

#endif /* SRC_FIGURESUBPLOTWINDOW_H_ */
