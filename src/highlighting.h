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

#ifndef HIGHLIGHTING_H
#define HIGHLIGHTING_H

#include "qwidget.h"

#define GAMEBOARD 0
#define STAND 1
#define FOR_CHANGE 2
#define NOWHERE 3

class Stand;
class Gameboard;

class Highlighting : public QWidget
{
	Q_OBJECT

	public:
		Highlighting( QWidget *parent = 0 , const char *name = 0 );
		void setStand( Stand *s ) { stand = s; }
		void setGameboard( Gameboard *p ) { gameboard = p; }
		void turnOffHighlighting();

	public slots:
		void standClicked( int );
		void forChangeClicked( int );
		void gameboardClicked( int , int );

	private:
		int x, y, where;
		Stand *stand;
		Gameboard *gameboard;
};

#endif
