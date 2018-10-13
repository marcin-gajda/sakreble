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

#include "lettersleft.h"
#include "alphabet.h"
#include "letters.h"

#include "qpainter.h"
#include "qwindefs.h"
#include "qpushbt.h"

LettersLeft::LettersLeft( QWidget *parent , const char *name ) : QWidget( parent , name , WStyle_DialogBorder )
{
	alphabet = NULL;

	setMinimumSize( 510 , 160 );
	setMaximumSize( 510 , 160 );

	setCaption( "Letters left in the sack" );

	ok = new QPushButton( "&Close", this, "ok" );
	ok->setGeometry( 260 , 135 , 100 , 20 );

	withMyLetters = new QPushButton( "&With my letters", this, "zmoimi" );
	withMyLetters->setGeometry( 150 , 135 , 100 , 20 );

	with_me = 0;
	whichPlayer = NULL;

	connect( ok , SIGNAL( clicked() ) , SIGNAL( closeClicked() ) );
	connect( withMyLetters , SIGNAL( clicked() ) , SLOT( changeWithMyLetters() ) );
}

void LettersLeft::paintEvent( QPaintEvent * )
{
	QPainter paint;
	QString s;
	int ile;

	paint.begin( this );

	paint.setPen( black );
	paint.setFont( QFont( "Times" , 18 , QFont::Bold ) );

	AlphabetIterator iter = alphabet->getIterator();
	iter.start();
	
	for( int j=0; j < ( alphabet->getNumberOfLettersWithBlank() - 1 ) / 17 + 1; j++ )
		for( int i=0; i<17; i++ )
		{
			if( !iter.next() )
				break;

			ile =  alphabet->letter( iter.getValue() );

			if( with_me )
				ile += playerLetters[*whichPlayer]->getLetterCount( iter.getValue() );
			
			ile += playerLetters[1-(*whichPlayer)]->getLetterCount( iter.getValue() );

			if( ile > 0 )
				paint.setBrush( gray );
			else
				paint.setBrush( darkGray );

			paint.drawRect( i*30 + 1 , j*70 + 1 , 28 , 28 );
			paint.drawRect( i*30 + 1 , j*70 + 31 , 28 , 28 );

			s.sprintf( "%s" , alphabet->alphabetLetterToShow( iter.getValue() ) );
			paint.drawText( i*30 + 1 , j*70 + 1 , 28 , 28 , AlignCenter , s , 1 );

			s.sprintf( "%d" , ile );
			paint.drawText( i*30 + 1 , j*70 + 31 , 28 , 28 , AlignCenter , s , 1 );
		}

	paint.end();
}

void LettersLeft::changeWithMyLetters()
{
	if( with_me )
		withMyLetters->setText( "With my letters" );
	else
		withMyLetters->setText( "Without my letters" );

	with_me = !with_me;

	repaint();
}
