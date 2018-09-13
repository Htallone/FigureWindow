/*
 * TitleEditWidget.cpp
 *
 *  Created on: Jun 14, 2016
 *      Author: htallone
 */

#include "TitleEditWidget.h"

TitleEditWidget::TitleEditWidget(QWidget* parent)   : QWidget(parent) {
	// TODO Auto-generated constructor stub

}

TitleEditWidget::TitleEditWidget(QWidget* parent, QCustomPlot *qcustomplotp)  : QWidget(parent) {

	qcustomplot = qcustomplotp;
	titleNameLineEdit = new QLineEdit;
	fontSelectDialog = new QFontDialog(this);

	titleNameLineEdit->setMaximumWidth(200);
	connect(titleNameLineEdit, SIGNAL(editingFinished()), this, SLOT(slotTitleNameEditingFinished()));

	QLabel * titleNameLabel = new QLabel("Title Name:");
	QLabel * titleFontLabel = new QLabel("Title Font:");

	QHBoxLayout * titleFontLayoutH = new QHBoxLayout;
	QPushButton * titleFontSelectButton = new QPushButton(QIcon("./icon/font-select.png"),"Select Font");
	connect(titleFontSelectButton, SIGNAL(clicked()), this, SLOT(slotTitleFontSelected()));
	titleFontSelectButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	titleFontLayoutH->addWidget(titleFontSelectButton);
	titleFontLayoutH->addStretch(1);

	QGridLayout *titleEditLayoutG = new QGridLayout;
	titleEditLayoutG->addWidget(titleNameLabel, 0, 0);
	titleEditLayoutG->addWidget(titleNameLineEdit, 0, 1);
	titleEditLayoutG->addWidget(titleFontLabel, 1, 0);
	titleEditLayoutG->addLayout(titleFontLayoutH, 1, 1);

	QHBoxLayout *titleEditStretchLayoutH = new QHBoxLayout;
	titleEditStretchLayoutH->addLayout(titleEditLayoutG);
	titleEditStretchLayoutH->addStretch(1);
	QVBoxLayout *titleEditStretchLayoutV = new QVBoxLayout;
	titleEditStretchLayoutV->addLayout(titleEditStretchLayoutH);
	titleEditStretchLayoutV->addStretch(1);

	this->setLayout(titleEditStretchLayoutV);

	title = new QCPPlotTitle(qcustomplot);

	QCPPlotTitle * title1 = qobject_cast<QCPPlotTitle *> (qcustomplot->plotLayout()->element(0,0));
	if(title1 )
	{
		title->setText(title1->text());
		titleNameLineEdit->setText( title1->text());
		qcustomplot->plotLayout()->remove(title1);
		qcustomplot->plotLayout()->simplify();

		qcustomplot->plotLayout()->insertRow(0);
		qcustomplot->plotLayout()->addElement(0, 0, title);
	}
	else
	{
		title->setText("NoTitle");
		titleNameLineEdit->setText( "NoTitle");
		qcustomplot->plotLayout()->simplify();
	}

}

TitleEditWidget::~TitleEditWidget() {
	// TODO Auto-generated destructor stub
}


void TitleEditWidget::slotTitleNameEditingFinished()
{
	QCPPlotTitle * title1 = qobject_cast<QCPPlotTitle *> (qcustomplot->plotLayout()->element(0,0));
	if((titleNameLineEdit->text() != "") && (titleNameLineEdit->text() != "NoTitle"))
	{
		title->setText(titleNameLineEdit->text());
		if(title1 )
		{
			if(titleNameLineEdit->text() != title1->text())
				title1->setText(titleNameLineEdit->text());
		}
		else
		{
			qcustomplot->plotLayout()->simplify();
			qcustomplot->plotLayout()->insertRow(0);
			qcustomplot->plotLayout()->addElement(0, 0, title);
		}
	}
	else
	{
		title->setText("NoTitle");
		if(title1 )
		{
			qcustomplot->plotLayout()->remove(title1);
			qcustomplot->plotLayout()->simplify();
		}
		else
		{
			qcustomplot->plotLayout()->simplify();
		}
	}
	qcustomplot->replot();
}
void TitleEditWidget::slotTitleFontSelected()
{
	QFont fontSelected;
	QCPPlotTitle * title1 = qobject_cast<QCPPlotTitle *> (qcustomplot->plotLayout()->element(0,0));
	if(title1)
		fontSelectDialog->setCurrentFont(title1->font());

	if(fontSelectDialog->exec())
	{
		fontSelected = fontSelectDialog->selectedFont();
		title->setFont(fontSelected);
		if((titleNameLineEdit->text() != "") && (titleNameLineEdit->text() != "NoTitle"))
		{
			title->setText(titleNameLineEdit->text());
			if(title1 )
			{
				if(titleNameLineEdit->text() != title1->text())
					title1->setText(titleNameLineEdit->text());
				title1->setFont(fontSelected);
			}
			else
			{
				qcustomplot->plotLayout()->simplify();
				qcustomplot->plotLayout()->insertRow(0);
				qcustomplot->plotLayout()->addElement(0, 0, title);
			}


		}
		else
		{
			title->setText("NoTitle");
			if(title1 )
			{
				qcustomplot->plotLayout()->remove(title1);
				qcustomplot->plotLayout()->simplify();
			}
			else
			{
				qcustomplot->plotLayout()->simplify();
			}
		}
	}
	qcustomplot->replot();
	this->setFocus();
}

