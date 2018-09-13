QT       += widgets printsupport

TARGET = FigWin
TEMPLATE = app

RESOURCES =

QMAKE_CXXFLAGS	+= -std=c++11 

INCLUDEPATH += FigureWindow

HEADERS      += qcustomplot/qcustomplot.h
SOURCES      += qcustomplot/qcustomplot.cpp

HEADERS      += FigureWindow/figurewindow.h \
                FigureWindow/PlottablesEditWidget.h \
				FigureWindow/ColorSelectComboBox.h \
				FigureWindow/AxisEditWidget.h \
				FigureWindow/LegendEditWidget.h \
				FigureWindow/TitleEditWidget.h \
				FigureWindow/DockTitleBarWidget.h \	
				FigureWindow/FigureSubplotWindow.h \
				FigureWindow/FigureWidget.h \
				FigureWindow/ImageSaveWidget.h \
				FigureWindow/SimpleEditWidget.h 
				
SOURCES      += FigureWindow/figurewindow.cpp \
				FigureWindow/PlottablesEditWidget.cpp \
				FigureWindow/ColorSelectComboBox.cpp \
				FigureWindow/AxisEditWidget.cpp \
				FigureWindow/LegendEditWidget.cpp \
				FigureWindow/TitleEditWidget.cpp \
				FigureWindow/DockTitleBarWidget.cpp \
				FigureWindow/FigureSubplotWindow.cpp \
				FigureWindow/FigureWidget.cpp \
				FigureWindow/ImageSaveWidget.cpp \
				FigureWindow/SimpleEditWidget.cpp 

				
SOURCES      += demo.cpp
                



