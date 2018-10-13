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

#include <stdio.h>

#include <qpainter.h>
#include <qwindefs.h>
#include <qfont.h>

#include "stand.h"
#include "dupa.h"
#include "alphabet.h"

Stand::Stand( QWidget *parent , const char *name ) : QWidget( parent, name )
{
	for( int i=0; i<7; i++ )
	{
		letters[i] = EMPTY_FIELD;
		for_change[i] = EMPTY_FIELD;
	}

	isHighlighted = -1;
}

void Stand::paintEvent( QPaintEvent *e )
{
	QPainter paint;
	QString s;
	QRect updateR = e->rect();

	paint.begin( this );

	paint.setPen( black );

	for( int i=0; i<15; i++ )
	{
		if( !updateR.intersects( QRect( i*30 + 1 , 1 , 28 , 28 ) ) )
			continue;

		if( i != 7 )
		{
			if( i < 7 )
			{
				if( letters[i] != EMPTY_FIELD )
				{
					if( isHighlighted == i )
						paint.setBrush( yellow );
					else
						paint.setBrush( darkGray );

					paint.drawRect( i*30 + 1 , 1 , 28 , 28 );

					s.sprintf( "%s" , alphabet->alphabetLetterToShow( letters[i] ) );
					paint.setFont( QFont( "Times" , 18 , QFont::Bold ) );
					paint.drawText( i*30 + 1 , 1 , 26 , 26 , AlignCenter , s , 1 );

					if( alphabet->getLetterPoints( letters[i] ) > 0 )
					{
						s.sprintf( "%i" , alphabet->getLetterPoints( letters[i] ) );
						paint.setFont( QFont( "Times" , 10 , QFont::Bold ) );
						paint.drawText( i*30 + 19 , 16 , 10 , 10 , AlignCenter , s , 1 );
					}
				}
				else
				{
					paint.setBrush( gray );
					paint.drawRect( i*30 + 1 , 1 , 28 , 28 );
				}
			}
			else
			{
				if( for_change[i-8] != EMPTY_FIELD )
				{
					if( isHighlighted == i )
						paint.setBrush( yellow );
					else
						paint.setBrush( darkGray );

					paint.drawRect( i*30 + 1 , 1 , 28 , 28 );

					s.sprintf( "%s" , alphabet->alphabetLetterToShow( for_change[i-8] ) );
					paint.setFont( QFont( "Times" , 18 , QFont::Bold ) );
					paint.drawText( i*30 + 1 , 1 , 26 , 26 , AlignCenter , s , 1 );

					if( alphabet->getLetterPoints( for_change[i-8] ) > 0 )
					{
						s.sprintf( "%i" , alphabet->getLetterPoints( for_change[i-8] ) );
						paint.setFont( QFont( "Times" , 10 , QFont::Bold ) );
						paint.drawText( i*30 + 19 , 16 , 10 , 10 , AlignCenter , s , 1 );
					}
				}
				else
				{
					paint.setBrush( gray );
					paint.drawRect( i*30 + 1 , 1 , 28 , 28 );
				}
			}
		}
	}

	paint.end();
}

void Stand::mousePressEvent( QMouseEvent *e )
{
	if ( e->button() != LeftButton )
		return;

	if( e->x() / 30 == 7 )
		return;

	if( e->x() / 30 < 7 )
		emit standClicked( e->x() / 30 );
	else
		emit forChangeClicked( e->x() / 30 - 8 );
}

void Stand::setLetterOnStand( char l , int where )
{
	if( letters[where] != l )
	{
		letters[where] = l;
		repaint( QRect( where*30 + 1 , 1 , 28 , 28 ) );
	}
}

void Stand::setForChangeLetter( char l , int where )
{
	if( for_change[where] != l )
	{
		for_change[where] = l;
		repaint( QRect( ( where + 8 )*30 + 1 , 1 , 28 , 28 ) );

		for( int i=0; i<7; i++ )
			if( for_change[i] != EMPTY_FIELD )
			{
				emit forChangeClickPossible( true );
				return;
			}

		emit forChangeClickPossible( false );
	}
}

/*
 * Ustawia letters od lewej, pozostale miejsca wypuszcza
 */
void Stand::setLettersOnStand( char *l , int ile )
{
	for( int i=0; i<7; i++ )
		if( i<ile )
			letters[i] = l[i];
		else
			letters[i] = EMPTY_FIELD;

	repaint( QRect( 0 , 0 , 220 , 30 ) );
}

void Stand::highlightLetterOnStand( int which_f )
{
	if( letters[which_f] == EMPTY_FIELD )
		return;

	if( isHighlighted == which_f )
		return;

	isHighlighted = which_f;

	repaint( QRect( which_f*30 + 1 , 1 , 28 , 28 ) );
}

void Stand::highlightForChangeLetter( int which_f )
{
	if( for_change[which_f] == EMPTY_FIELD )
		return;

	if( isHighlighted == which_f )
		return;

	isHighlighted = which_f + 8;

	repaint( QRect( ( which_f + 8 )*30 + 1 , 1 , 28 , 28 ) );
}

void Stand::turnOffStandLetter()
{
	int which_f = isHighlighted;
	isHighlighted = -1;

	repaint( QRect( which_f * 30 + 1 , 1 , 28 , 28 ) );
}

void Stand::turnOffForChangeLetter()
{
	int which_f = isHighlighted;
	isHighlighted = -1;

	repaint( QRect( which_f * 30 + 1 , 1 , 28 , 28 ) );
}

void Stand::changeStandLetters( int i , int j )
{
	char pom;

	if( i == j )
		return;

	pom = letters[i];
	letters[i] = letters[j];
	letters[j] = pom;

	repaint( QRect( i*30 + 1 , 1 , 28 , 28 ) );
	repaint( QRect( j*30 + 1 , 1 , 28 , 28 ) );
}

void Stand::changeForChangeLetters( int i , int j )
{
	char pom;

	if( i == j )
		return;

	pom = for_change[i];
	for_change[i] = for_change[j];
	for_change[j] = pom;

	repaint( QRect( (i+8)*30 + 1 , 1 , 28 , 28 ) );
	repaint( QRect( (j+8)*30 + 1 , 1 , 28 , 28 ) );
}

int Stand::areForChangeLetters()
{
	for( int i=0; i<7; i++ )
		if( for_change[i] != EMPTY_FIELD )
			return 1;

	return 0;
}

int Stand::areLettersOnStand()
{
	for( int i=0; i<7; i++ )
		if( letters[i] != EMPTY_FIELD )
			return 1;

	return 0;
}

void Stand::removeForChangeLetters()
{
	for( int i=0; i<7; i++ )
		if( for_change[i] != EMPTY_FIELD )
			setForChangeLetter( EMPTY_FIELD , i );
}

