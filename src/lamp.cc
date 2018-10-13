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

#include <qlabel.h>
#include <qframe.h>
#include <qwindefs.h>

#include "lamp.h"

Lamp::Lamp( QWidget *parent , const char *name ) : QWidget( parent , name )
{
	setMaximumSize( 20 , 20 );

	label = new QLabel( this , "lamp" );
	label->setGeometry( 0 , 0 , 20 , 20 );
	label->setFrameStyle( QFrame::Sunken | QFrame::Panel );

	state = TURNED_ON;

	turnOff();
}

void Lamp::turnOn()
{
	if( state == TURNED_ON )
		return;

	label->setBackgroundColor( red );

	state = TURNED_ON;
}

void Lamp::turnOff()
{
	if( state == TURNED_OFF )
		return;

	label->setBackgroundColor( lightGray );

	state = TURNED_OFF;
}
