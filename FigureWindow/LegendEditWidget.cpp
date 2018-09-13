/*
 * LegendEditWidget.cpp
 *
 *  Created on: Jun 12, 2016
 *      Author: htallone
 */

#include "LegendEditWidget.h"

LegendEditWidget::LegendEditWidget(QWidget* parent) : QWidget(parent) {
	// TODO Auto-generated constructor stub

}

LegendEditWidget::LegendEditWidget(QWidget* parent, QCustomPlot *qcustomplotp)
									: QWidget(parent)
{
	qcustomplot = qcustomplotp;

	legendPositionComboBox = new QComboBox;
	fontSelectDialog = new QFontDialog(this);

	alignmentIntMap.clear();
	alignmentIntMap[Qt::AlignTop    |    Qt::AlignLeft		] = 1;
	alignmentIntMap[Qt::AlignTop    |    Qt::AlignHCenter	] = 2;
	alignmentIntMap[Qt::AlignTop    |    Qt::AlignRight		] = 3;
	alignmentIntMap[Qt::AlignLeft   |    Qt::AlignVCenter	] = 4;
	alignmentIntMap[Qt::AlignCenter        					] = 5;
	alignmentIntMap[Qt::AlignRight  |    Qt::AlignVCenter	] = 6;
	alignmentIntMap[Qt::AlignBottom |    Qt::AlignLeft		] = 7;
	alignmentIntMap[Qt::AlignBottom |    Qt::AlignHCenter	] = 8;
	alignmentIntMap[Qt::AlignBottom |    Qt::AlignRight		] = 9;

	alignmentVec.clear();
	alignmentVec.push_back(Qt::AlignTop    |    Qt::AlignLeft		);
	alignmentVec.push_back(Qt::AlignTop    |    Qt::AlignHCenter	);
	alignmentVec.push_back(Qt::AlignTop    |    Qt::AlignRight		);
	alignmentVec.push_back(Qt::AlignLeft   |    Qt::AlignVCenter	);
	alignmentVec.push_back(Qt::AlignCenter        					);
	alignmentVec.push_back(Qt::AlignRight  |    Qt::AlignVCenter	);
	alignmentVec.push_back(Qt::AlignBottom |    Qt::AlignLeft		);
	alignmentVec.push_back(Qt::AlignBottom |    Qt::AlignHCenter	);
	alignmentVec.push_back(Qt::AlignBottom |    Qt::AlignRight		);


	legendPositionComboBox->setToolTip(tr("Legend Alignment"));
	legendPositionComboBox->setEditable(false);
	legendPositionComboBox->setMinimumWidth(40);
	legendPositionComboBox->setIconSize(QSize(16,16));

	QPixmap pix(16,16);
	pix.fill(QColor("transparent"));
	legendPositionComboBox->addItem(QIcon(pix),tr("Free"));
	legendPositionComboBox->addItem(QIcon("./icon/alignmentTopLeft.png"),tr("TopLeft"));
	legendPositionComboBox->addItem(QIcon("./icon/alignmentTopCenter.png"),tr("TopCenter"));
	legendPositionComboBox->addItem(QIcon("./icon/alignmentTopRight.png"),tr("TopRight"));
	legendPositionComboBox->addItem(QIcon("./icon/alignmentLeftCenter.png"),tr("LeftCenter"));
	legendPositionComboBox->addItem(QIcon("./icon/alignmentCenter.png"),tr("Center"));
	legendPositionComboBox->addItem(QIcon("./icon/alignmentRightCenter.png"),tr("RightCenter"));
	legendPositionComboBox->addItem(QIcon("./icon/alignmentBottomLeft.png"),tr("BottomLeft"));
	legendPositionComboBox->addItem(QIcon("./icon/alignmentBottomCenter.png"),tr("BottomCenter"));
	legendPositionComboBox->addItem(QIcon("./icon/alignmentBottomRight.png"),tr("BottomRight"));

	connect(legendPositionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotLegendAlignmentSelected(int)));

	QLabel * legendAlignmentLabel = new QLabel("Legend Alignment:");
	QLabel * legendFontLabel = new QLabel("Legend Font:");

	QHBoxLayout * legendFontLayoutH = new QHBoxLayout;
	QPushButton * legendFontSelectButton = new QPushButton(QIcon("./icon/font-select.png"),"Select Font");
	connect(legendFontSelectButton, SIGNAL(clicked()), this, SLOT(slotLegendFontSelected()));
	legendFontSelectButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	legendFontLayoutH->addWidget(legendFontSelectButton);
	legendFontLayoutH->addStretch(1);

	QGridLayout *legendEditLayoutG = new QGridLayout;
	legendEditLayoutG->addWidget(legendAlignmentLabel, 0, 0);
	legendEditLayoutG->addWidget(legendPositionComboBox, 0, 1);
	legendEditLayoutG->addWidget(legendFontLabel, 1, 0);
	legendEditLayoutG->addLayout(legendFontLayoutH, 1, 1);

	QHBoxLayout *legendEditStretchLayoutH = new QHBoxLayout;
	legendEditStretchLayoutH->addLayout(legendEditLayoutG);
	legendEditStretchLayoutH->addStretch(1);
	QVBoxLayout *legendEditStretchLayoutV = new QVBoxLayout;
	legendEditStretchLayoutV->addLayout(legendEditStretchLayoutH);
	legendEditStretchLayoutV->addStretch(1);

	this->setLayout(legendEditStretchLayoutV);


	if(qcustomplot->axisRect()->insetLayout()->insetPlacement(0) == QCPLayoutInset::ipFree)
	{
		if(legendPositionComboBox->currentIndex() != 0)
			legendPositionComboBox->setCurrentIndex(0);
	}
	else if(qcustomplot->axisRect()->insetLayout()->insetPlacement(0) == QCPLayoutInset::ipBorderAligned)
	{
		Qt::Alignment legendAli = qcustomplot->axisRect()->insetLayout()->insetAlignment(0);
		if(legendPositionComboBox->currentIndex() != alignmentIntMap[legendAli])
			legendPositionComboBox->setCurrentIndex(alignmentIntMap[legendAli]);
	}

}
LegendEditWidget::~LegendEditWidget() {
	// TODO Auto-generated destructor stub
}


void LegendEditWidget::slotLegendAlignmentSelected(int alignmentIndex)
{
	if(alignmentIndex == 0)
	{

	}
	else
	{
		qcustomplot->axisRect()->insetLayout()->setInsetPlacement(0,QCPLayoutInset::ipBorderAligned);
		qcustomplot->axisRect()->insetLayout()->setInsetAlignment(0,alignmentVec[alignmentIndex-1]);
		qcustomplot->replot();
	}

}
void LegendEditWidget::slotLegendFontSelected()
{
	QFont fontSelected;

	fontSelectDialog->setCurrentFont(qcustomplot->legend->font());
	if(fontSelectDialog->exec())
	{
		fontSelected = fontSelectDialog->selectedFont();
		if(fontSelected != qcustomplot->legend->font())
		{
			qcustomplot->legend->setFont(fontSelected);
			qcustomplot->replot();
		}
	}

	this->setFocus();
}







