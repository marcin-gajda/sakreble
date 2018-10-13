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

#include "blank.h"
#include "alphabet.h"
#include "qpainter.h"
#include "qwindefs.h"

ChangeBlank::ChangeBlank( QWidget *parent , const char *name ) : QSemiModal( parent , name , true , WStyle_DialogBorder )
{
	alphabet = NULL;

	setMinimumSize( 240 , 120 );
	setMaximumSize( 240 , 120 );

	setCaption( "Change blank" );
}

void ChangeBlank::paintEvent( QPaintEvent * )
{
	QPainter paint;
	QString s;

	paint.begin( this );

	paint.setPen( black );
	paint.setBrush( darkGray );
	paint.setFont( QFont( "Times" , 18 , QFont::Bold ) );

	AlphabetIterator iter = alphabet->getIterator();
	iter.start();

	/*
	 * Omit blank.
	 */
	iter.next();
	
	for( int j=0; j< ( alphabet->getNumberOfLetters() - 1 ) / 4 + 1; j++ )
		for( int i=0; i<8; i++ )
		{
			if( !iter.next() )
				break;
			
			paint.drawRect( i*30 + 1 , j*30 + 1 , 28 , 28 );

			s.sprintf( "%s" , alphabet->alphabetLetterToShow( iter.getValue() ) );
			paint.drawText( i*30 + 1 , j*30 + 1 , 28 , 28 , AlignCenter , s , 1 );
		}

	paint.end();
}

void ChangeBlank::mousePressEvent( QMouseEvent *e )
{
	if ( e->button() != LeftButton )
		return;

	hide();

	emit changeBlankClicked( alphabet->getLetterInternalNumber( e->x()/30 + 1 + (e->y()/30)*8 ) );
}

