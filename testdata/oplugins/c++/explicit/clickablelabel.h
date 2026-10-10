/*
    SPDX-FileCopyrightText: 2010 José Manuel Santamaría Lema <panfaust@gmail.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>

class QWidget;
class QString;
class QMouseEvent;


class ClickableLabel : public QLabel
{
Q_OBJECT

public:
	ClickableLabel( QWidget * parent = 0, Qt::WindowFlags f = 0 ); //issue
	ClickableLabel( const QString & text, QWidget * parent = 0, Qt::WindowFlags f = 0 ); //issue

signals:
	void clicked();

protected:
	virtual void mousePressEvent( QMouseEvent * event );
	virtual void mouseReleaseEvent( QMouseEvent * event );

private:
	bool down;

};

#endif //CLICKABLELABEL_H
