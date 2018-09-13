/*
 * ColorSelectComboBox.cpp
 *
 *  Created on: Jun 9, 2016
 *      Author: htallone
 */

#include "ColorSelectComboBox.h"

ColorSelectComboBox::ColorSelectComboBox(QWidget *widget) : QComboBox(widget) {
    QStringList colorNames = QColor::colorNames();

    setIconSize(QSize(20,20));

    QStringList colorUsedFrequntly;
    colorUsedFrequntly<<"transparent"<<"white"<<"black"<<"red"<<"blue"<<"lime"
    		<<"magenta"<<"cyan"<<"green"<<"yellow"<<"purple";
    for(int i=0;i<colorUsedFrequntly.size();i++)
    {
        QColor color(colorUsedFrequntly[i]);
        QPixmap px(20,20);
        px.fill(color);
        QIcon icon(px);
        insertItem(i, icon, colorUsedFrequntly[i]);
    }

    for (int i = 0; i < colorNames.size(); ++i) {

    	bool flag = false;
    	for(int j=0;j<colorUsedFrequntly.size();j++)
    	{
    		if(colorUsedFrequntly[j] == colorNames[i])
    		{
    			flag = true;
    			break;
    		}
    	}

    	if(flag)
    		continue;

        QColor color(colorNames[i]);
        QPixmap px(20,20);
        px.fill(color);
        QIcon icon(px);

        insertItem(i+colorUsedFrequntly.size(), icon, colorNames[i]);
        //setItemData(i, color, Qt::DecorationRole);
    }

}

ColorSelectComboBox::~ColorSelectComboBox() {
	// TODO Auto-generated destructor stub
}

QColor ColorSelectComboBox::color() const
{
	return QColor(itemText(currentIndex()));
	//return qvariant_cast<QColor>(itemData(currentIndex(), Qt::DecorationRole));
}
void ColorSelectComboBox::setColor(QColor color)
{
	int index = -1;
	for(int i=0;i<count();i++)
	{
		if(color == QColor(itemText(i)))
		{
			index = i;
			break;
		}
	}
	//int index = findData(color, int(Qt::DecorationRole));
	if(index == -1)
	{
        QPixmap px(20,20);
        px.fill(color);
        QIcon icon(px);
        addItem(icon, color.name());
       // setItemData(count()-1, color, Qt::DecorationRole);
        setCurrentIndex(count()-1);
	}
	else
	{
		setCurrentIndex(index);
	}

}

