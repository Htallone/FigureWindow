


#include <QApplication>
#include <random>

#include "figurewindow.h"
#include "FigureSubplotWindow.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    QPen pen,pen1,pen2,pen3;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    pen.setColor(Qt::black);

    pen1.setStyle(Qt::DashLine);
    pen1.setWidth(2);
    pen1.setColor(Qt::red);

    pen2.setStyle(Qt::DotLine);
    pen2.setWidth(2);
    pen2.setColor(Qt::blue);

    pen3.setStyle(Qt::DashDotLine);
    pen3.setWidth(2);
    pen3.setColor(Qt::darkMagenta);



    int pointCount = 100;
    QVector<double> x1(pointCount), y1(pointCount);
    QVector<double> x2(pointCount), y2(pointCount);
    QVector<double> x3(pointCount), y3(pointCount);
    QVector<double> x4(pointCount), y4(pointCount);
    QVector<double> x5(pointCount), y5(pointCount);
    QVector<double> x6(pointCount), y6(pointCount);


    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(0.0,1.0);


    for (int i=0; i<pointCount; ++i)
    {
      double phi = (i/(double)(pointCount-1))*3*M_PI;
      x1[i] = qSqrt(phi)*qCos(phi);
      y1[i] = qSqrt(phi)*qSin(phi);

      x2[i] = i/25.0 - 2; // x goes from -1 to 1
      y2[i] = sin(x2[i]*x2[i]);  // let's plot a quadratic function

      double t = i/(double)(pointCount-1)*2*M_PI;
      x3[i] = 2*qCos(2*t)+qCos(1*t)+2*qSin(t);
      y3[i] = 2*qSin(2*t)-qSin(1*t);


      double r = sqrt(distribution(generator));
      double theta = 2.0*M_PI*distribution(generator);
      x4[i] = r*cos(theta);
      y4[i] = r*sin(theta);

      double tt = i/(double)(pointCount-1)*2*M_PI;
      x5[i] = cos(tt);
      y5[i] = sin(tt);

      double rr = 0.8+0.2*i/(double)(pointCount-1);
      x6[i] = x5[i]*rr + 0.04*(2*distribution(generator)-1);
      y6[i] = y5[i]*rr + 0.04*(2*distribution(generator)-1);
    }


    FigureWindow figWin;
    figWin.plot(x1,y1);
    figWin.holdon();
    figWin.plot(x2,y2,pen1,"x2-y2");
    figWin.plot(x3,y3,pen2,QCPScatterStyle(QCPScatterStyle::ssCircle,pen.color(),Qt::white,6),"x3-y3");

    figWin.show();



    FigureWindow * figWinP = new FigureWindow;
    figWinP->axisequal();
	figWinP->plot(x5,y5,pen,"x5-y5");
    figWinP->holdon();
    figWinP->plot(x6,y6,pen3,"x6-y6");
	figWinP->setWindowTitle("x5-y5 VS x6-y6 title");
    figWinP->setAxisLabelX("x");
    figWinP->setAxisLabelY("y");

	figWinP->show();



    FigureSubplotWindow windowSub;
    // windowSub.resize(QSize(740,600));

    windowSub.subplot(0,0);
    windowSub.currentFigure()->plot(x1,y1);
    windowSub.currentFigure()->holdon();
    windowSub.currentFigure()->setTitle("Figure 1");

    windowSub.subplot(0,1);
    windowSub.currentFigure()->plot(x2,y2);
    windowSub.currentFigure()->holdon();
    windowSub.currentFigure()->setTitle("图 2");

    
    windowSub.subplot(1,0);
    windowSub.currentFigure()->plot(x3,y3);
    windowSub.currentFigure()->holdon();
    windowSub.currentFigure()->setTitle("test 3");
    
    windowSub.subplot(1,1);
    windowSub.currentFigure()->setTitle("test 4");
    windowSub.currentFigure()->plotScatters(x4,y4,QCPScatterStyle(QCPScatterStyle::ssCircle,pen,QBrush(Qt::white),6));
    windowSub.currentFigure()->holdon();
    windowSub.currentFigure()->plot(x5,y5);

    windowSub.show();

    windowSub.subplot(1,1);
    windowSub.currentFigure()->axisequal();

    return app.exec();



//    Qt::white	3	White (#ffffff)
//    Qt::black	2	Black (#000000)
//    Qt::red	7	Red (#ff0000)
//    Qt::darkRed	13	Dark red (#800000)
//    Qt::green	8	Green (#00ff00)
//    Qt::darkGreen	14	Dark green (#008000)
//    Qt::blue	9	Blue (#0000ff)
//    Qt::darkBlue	15	Dark blue (#000080)
//    Qt::cyan	10	Cyan (#00ffff)
//    Qt::darkCyan	16	Dark cyan (#008080)
//    Qt::magenta	11	Magenta (#ff00ff)
//    Qt::darkMagenta	17	Dark magenta (#800080)
//    Qt::yellow	12	Yellow (#ffff00)
//    Qt::darkYellow	18	Dark yellow (#808000)
//    Qt::gray	5	Gray (#a0a0a4)
//    Qt::darkGray	4	Dark gray (#808080)
//    Qt::lightGray	6	Light gray (#c0c0c0)
//    Qt::transparent	19	a transparent black value (i.e., QColor(0, 0, 0, 0))
//    Qt::color0	0	0 pixel value (for bitmaps)
//    Qt::color1	1	1 pixel value (for bitmaps)


}

