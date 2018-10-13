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

#include <stdlib.h>
#include <stdio.h>

#include "qwindefs.h"
#include "qpushbt.h"
#include "qlistbox.h"

#include "aboutplayers.h"

AboutPlayers::AboutPlayers( QWidget *parent , const char *name ) : QSemiModal( parent , name , true , WStyle_DialogBorder )
{
	setCaption( "About players" );

	setMinimumSize( 300 , 250 );
	setMaximumSize( 300 , 250 );

	table = new QListBox( this , "tabelka" );
	table->setGeometry( 10 , 10 , 280 , 200 );

	closeButton = new QPushButton( "&Close" , this , "zamknij_button" );
	closeButton->setGeometry( 120 , 220 , 60 , 20 );

	players = NULL;

	connect( closeButton , SIGNAL( clicked() ) , SLOT( hide() ) );
}

void AboutPlayers::show()
{
	QString s;

	if( players == NULL )
		return;

	table->clear();

	for( int i=0; i<players->getNumberOfPlayers(); i++ )
	{
		s = tr( "%1: %2, rankValue: %3" ).
			arg( (*players)[i].getName() ).
			arg( ((*players)[i].realPlayer()) ? "Human" : "Computer" ).
			arg( (*players)[i].rankValue() );

		table->insertItem( s );
	}

	QSemiModal::show();
}
