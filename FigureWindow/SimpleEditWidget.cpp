/*
 * SimpleEditWidget.cpp
 *
 *  Created on: Jun 11, 2016
 *      Author: htallone
 */

#include "SimpleEditWidget.h"
#include "AxisEditWidget.h"

SimpleEditWidget::SimpleEditWidget(QWidget* parent) : QWidget(parent){
	// TODO Auto-generated constructor stub

}

SimpleEditWidget::SimpleEditWidget(QWidget* parent, QCustomPlot *qcustomplotp): QWidget(parent)
{
	qcustomplot = qcustomplotp;

	simpleEditTreeView = new QTreeView;

	titleEdit = new TitleEditWidget(this, qcustomplot);
	axisEdit = new AxisEditWidget(this, qcustomplot, 0);
	legendEdit = new LegendEditWidget(this, qcustomplot);
	plottablesEdit = new PlottablesEditWidget(this, qcustomplot, 0);
	blankWidget = new QWidget;

	stackedWidget = new QStackedWidget;

	QStandardItemModel *standardModel = new QStandardItemModel ;
	QStandardItem *rootNode = standardModel->invisibleRootItem();

	//defining a couple of items
	QStandardItem *titleItem = new QStandardItem("Title");
	QStandardItem *axisXItem = new QStandardItem("Axis X");
	QStandardItem *axisYItem = new QStandardItem("Axis Y");
	QStandardItem *legendItem =  new QStandardItem("Legend");
	QStandardItem *plottablesItem =  new QStandardItem("Plottables");

	//building up the hierarchy
	rootNode->    appendRow(titleItem);
	rootNode->    appendRow(axisXItem);
	rootNode->    appendRow(axisYItem);
	rootNode->    appendRow(legendItem);
	rootNode->    appendRow(plottablesItem);


	for(int i=0;i<qcustomplot->plottableCount();i++)
	{
		if(qcustomplot->plottable(i)->name() == "")
			plottablesItem->   appendRow(new QStandardItem("NoName"));
		else
		{
			plottablesItem->   appendRow(new QStandardItem(qcustomplot->plottable(i)->name()));
		}
	}


	simpleEditTreeView->setModel(standardModel);

	QItemSelectionModel *selectionModel= simpleEditTreeView->selectionModel();
	connect(selectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
	            this, SLOT(slotSimpleEditTreeViewSelectionChanged(const QItemSelection &, const QItemSelection &)));

	simpleEditTreeView->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
	simpleEditTreeView->setFixedWidth(160);
	simpleEditTreeView->setHeaderHidden(true);
	simpleEditTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	connect(plottablesEdit, SIGNAL(signalPlottablesNameChanged(int ,const QString &)), this, SLOT(slotSimpleEditTreeViewNodeNameChanged(int,const QString &)));

	stackedWidget->addWidget(blankWidget);
	stackedWidget->addWidget(titleEdit);
	stackedWidget->addWidget(axisEdit);
	stackedWidget->addWidget(legendEdit);
	stackedWidget->addWidget(plottablesEdit);

	stackedWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

	QPushButton * widgetSimpleEditOK = new QPushButton;
	connect(widgetSimpleEditOK, SIGNAL(clicked()), this, SLOT(slotSimpleEditOKClicked()));
	widgetSimpleEditOK->setText(tr("OK"));

	QHBoxLayout *hlayoutOK = new QHBoxLayout;
	hlayoutOK->addStretch(1);
	hlayoutOK->addWidget(widgetSimpleEditOK);

	QVBoxLayout *vlayoutOKStackedWidget = new QVBoxLayout;
	vlayoutOKStackedWidget->addWidget(stackedWidget);
	vlayoutOKStackedWidget->addStretch(1);
	vlayoutOKStackedWidget->addLayout(hlayoutOK);

	QHBoxLayout *hlayoutTreeOKStackedWidget = new QHBoxLayout;
	hlayoutTreeOKStackedWidget->addWidget(simpleEditTreeView);
	hlayoutTreeOKStackedWidget->addLayout(vlayoutOKStackedWidget);

	this->setLayout(hlayoutTreeOKStackedWidget);
	this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	this->setFixedHeight(200);
	this->setFocusPolicy(Qt::StrongFocus);

}
SimpleEditWidget::~SimpleEditWidget() {
	// TODO Auto-generated destructor stub
}

void SimpleEditWidget::slotSimpleEditTreeViewSelectionChanged(const QItemSelection &, const QItemSelection &)
{
    const QModelIndex index = simpleEditTreeView->selectionModel()->currentIndex();
    QString selectedText = index.data(Qt::DisplayRole).toString();

    QModelIndex parentIndex = index.parent();
    QString parentText;

    if(parentIndex == QModelIndex())
    {
    	if(selectedText != "Plottables")
    	{
    		plottablesEdit->unselectAllPlotttables();
    		qcustomplot->replot();

    	}

    	if(selectedText == "Title")
    	{
    		stackedWidget->setCurrentWidget(titleEdit);
    	}
    	else if(selectedText == "Axis X")
    	{
    		stackedWidget->setCurrentWidget(axisEdit);
    		axisEdit->changeAxisIndex(0);
    	}
    	else if(selectedText == "Axis Y")
    	{
    		stackedWidget->setCurrentWidget(axisEdit);
    		axisEdit->changeAxisIndex(1);
    	}
    	else if(selectedText == "Legend")
    	{
    		stackedWidget->setCurrentWidget(legendEdit);
    	}
    	else if(selectedText == "Plottables")
    	{
    		QStandardItemModel * itemModel = qobject_cast<QStandardItemModel *> (simpleEditTreeView->model());
    	    QStandardItem * item = itemModel->itemFromIndex(index);
    		if(item->hasChildren())
    		{
				simpleEditTreeView->expand(index);
				simpleEditTreeView->setCurrentIndex(index.child(0,0));
				stackedWidget->setCurrentWidget(plottablesEdit);
				plottablesEdit->changePlottableIndex(0);
    		}
    		else
    		{
    			stackedWidget->setCurrentWidget(blankWidget);
    		}
    	}
    	else
    	{
    		stackedWidget->setCurrentWidget(blankWidget);
    	}
    }
    else
    {
    	parentText = parentIndex.data(Qt::DisplayRole).toString();

    	if(parentText == "Plottables")
    	{
    		stackedWidget->setCurrentWidget(plottablesEdit);
    		plottablesEdit->unselectAllPlotttables();
    		qcustomplot->plottable(index.row())->setSelected(true);
    		plottablesEdit->changePlottableIndex(index.row());
    		qcustomplot->replot();
    	}
    	else
    	{

    	}

    }

}


void SimpleEditWidget::slotSimpleEditTreeViewNodeNameChanged(int plotIndex,const QString & newName)
{
	QStandardItemModel * itemModel = qobject_cast<QStandardItemModel *> (simpleEditTreeView->model());
    QStandardItem * item = itemModel->findItems("Plottables",Qt::MatchExactly,0).at(0);
    const QModelIndex index = itemModel->indexFromItem(item);

	if((newName == "") ||(newName == "NoName"))
		itemModel->itemFromIndex(index.child(plotIndex,0))->setText("NoName");
	else
		itemModel->itemFromIndex(index.child(plotIndex,0))->setText(newName);


}
void SimpleEditWidget::slotSimpleEditOKClicked()
{
	simpleEditTreeView->setCurrentIndex(QModelIndex());
	stackedWidget->setCurrentWidget(blankWidget);
	this->setFocus();
	plottablesEdit->unselectAllPlotttables();
	qcustomplot->replot();
}

void SimpleEditWidget::updateSimpleEdit()
{
	QStandardItemModel * itemModel = qobject_cast<QStandardItemModel *> (simpleEditTreeView->model());
    QStandardItem * item = itemModel->findItems("Plottables",Qt::MatchExactly,0).at(0);
    item->removeRows(0,item->rowCount());
    simpleEditTreeView->update(itemModel->indexFromItem(item));
	for(int i=0;i<qcustomplot->plottableCount();i++)
	{
		if(qcustomplot->plottable(i)->name() == "")
			item->   appendRow(new QStandardItem("NoName"));
		else
		{
			item->   appendRow(new QStandardItem(qcustomplot->plottable(i)->name()));
		}
	}

    simpleEditTreeView->update();
}

