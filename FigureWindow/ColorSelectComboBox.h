/*
 * ColorSelectComboBox.h
 *
 *  Created on: Jun 9, 2016
 *      Author: htallone
 */

#ifndef SRC_COLORSELECTCOMBOBOX_H_
#define SRC_COLORSELECTCOMBOBOX_H_


#include <QtWidgets>

class ColorSelectComboBox : public QComboBox {

    Q_OBJECT

public:
	ColorSelectComboBox(QWidget *widget = 0);
	virtual ~ColorSelectComboBox();

public:
    QColor color() const;
    void setColor(QColor c);
};

#endif /* SRC_COLORSELECTCOMBOBOX_H_ */
