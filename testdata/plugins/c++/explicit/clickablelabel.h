/***************************************************************************
*   Copyright © 2010 José Manuel Santamaría Lema <panfaust@gmail.com>     *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

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
	ClickableLabel( QWidget * parent = 0, Qt::WindowFlags f = 0 );
	ClickableLabel( const QString & text, QWidget * parent = 0, Qt::WindowFlags f = 0 );

signals:
	void clicked();

protected:
	virtual void mousePressEvent( QMouseEvent * event );
	virtual void mouseReleaseEvent( QMouseEvent * event );

private:
	bool down;

};

#endif //CLICKABLELABEL_H
