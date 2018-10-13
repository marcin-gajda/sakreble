/*
 * Sakreble! - open word game.
 * Copyright (C) 2004 Marcin Gajda
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef POINTS_H
#define POINTS_H

#include <qwidget.h>

class QLabel;

class Points : public QWidget
{
    Q_OBJECT

    public:
	Points( QWidget *parent = 0 , const char *name = 0 );

	void zeroPoints();
	void addPoints( int );
	int getPoints();
	void setName( const char * );
	void setPoints( int );

    public slots:

    protected:
//	void paintEvent( QPaintEvent * );

    private:
	QLabel *kto, *ile;
	int points;
};

#endif
