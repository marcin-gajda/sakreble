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

#include "highlighting.h"
#include "stand.h"
#include "gameboard.h"
#include "dupa.h"

Highlighting::Highlighting( QWidget *parent , const char *name ) : QWidget( parent , name )
{
	where = NOWHERE;
	stand = NULL;
	gameboard = NULL;
}

void Highlighting::standClicked( int nr )
{
	char l;

	switch( where )
	{
		case NOWHERE:
			if( stand->getLetterOnStand( nr ) != EMPTY_FIELD )
			{
				where = STAND;
				x = nr;
				stand->highlightLetterOnStand( nr );
			}
			else
			{
				for( int i=0; i<15; i++ )
					for( int j=0; j<15; j++ )
						if( gameboard->addedLetter( i , j ) )
							for( int k=0; k<7; k++ )
							{
								if( stand->getLetterOnStand( k ) != EMPTY_FIELD )
									continue;

								stand->setLetterOnStand( gameboard->letter( i , j ) , k );
								break;
							}

				gameboard->removeAddedLetters();
			}

			break;

		case STAND:
			where = NOWHERE;
			stand->turnOffStandLetter();
			stand->changeStandLetters( x , nr );

			break;

		case FOR_CHANGE:
			where = NOWHERE;
			stand->turnOffForChangeLetter();
			l = stand->getLetterForChange( x );

			if( stand->getLetterOnStand( nr ) == EMPTY_FIELD )
				stand->setForChangeLetter( EMPTY_FIELD , x );
			else
				stand->setForChangeLetter( stand->getLetterOnStand( nr ) , x );

			stand->setLetterOnStand( l , nr );

			break;

		case GAMEBOARD:
			where = NOWHERE;
			gameboard->turnOffLetter();
			l = gameboard->letter( x , y );

			if( stand->getLetterOnStand( nr ) == EMPTY_FIELD )
				gameboard->removeAddedLetter( x , y );
			else
				gameboard->setAddedLetter( stand->getLetterOnStand( nr ) , x , y );

			stand->setLetterOnStand( l , nr );

			break;

		default:;
	}
}

void Highlighting::forChangeClicked( int nr )
{
	char l;

	switch( where )
	{
		case NOWHERE:
			if( stand->getLetterForChange( nr ) != EMPTY_FIELD )
			{
				where = FOR_CHANGE;
				x = nr;
				stand->highlightForChangeLetter( nr );
			}
			break;

		case FOR_CHANGE:
			where = NOWHERE;
			stand->turnOffForChangeLetter();
			stand->changeForChangeLetters( x , nr );

			break;

		case STAND:
			where = NOWHERE;
			stand->turnOffStandLetter();
			l = stand->getLetterOnStand( x );

			if( stand->getLetterForChange( nr ) == EMPTY_FIELD )
				stand->setLetterOnStand( EMPTY_FIELD , x );
			else
				stand->setLetterOnStand( stand->getLetterForChange( nr ) , x );

			stand->setForChangeLetter( l , nr );

			break;

		case GAMEBOARD:
			where = NOWHERE;
			gameboard->turnOffLetter();
			l = gameboard->letter( x , y );

			if( stand->getLetterForChange( nr ) == EMPTY_FIELD )
				gameboard->removeAddedLetter( x , y );
			else
				gameboard->setAddedLetter( stand->getLetterForChange( nr ) , x , y );

			stand->setForChangeLetter( l , nr );

			break;

		default:;
	}
}

void Highlighting::gameboardClicked( int nrx , int nry )
{
	char l;

	switch( where )
	{
		case NOWHERE:
			if( gameboard->addedLetter( nrx , nry ) && stand->isEnabled() )
			{
				where = GAMEBOARD;
				x = nrx;
				y = nry;
				gameboard->highlightLetter( x , y );
			}
			else
				if( gameboard->isEmptyField( nrx , nry ) && stand->areLettersOnStand() && stand->isEnabled() )
				{
					for( int i=0; i<7; i++ )
					{
						if( ( l = stand->getLetterOnStand( i ) ) == EMPTY_FIELD )
							continue;

						stand->setLetterOnStand( EMPTY_FIELD , i );
						gameboard->setAddedLetter( l , nrx , nry );

						break;
					}
				}
			break;

		case STAND:
			if( gameboard->isEmptyField( nrx , nry ) )
			{
				where = NOWHERE;
				stand->turnOffStandLetter();
				l = stand->getLetterOnStand( x );
				stand->setLetterOnStand( EMPTY_FIELD , x );
				gameboard->setAddedLetter( l , nrx , nry );
			}
			else
				if( gameboard->addedLetter( nrx , nry ) )
				{
					where = NOWHERE;
					stand->turnOffStandLetter();
					l = stand->getLetterOnStand( x );
					stand->setLetterOnStand( gameboard->letter( nrx , nry ) , x );
					gameboard->setAddedLetter( l , nrx , nry );
				}
			break;

		case FOR_CHANGE:
			if( gameboard->isEmptyField( nrx , nry ) )
			{
				where = NOWHERE;
				stand->turnOffForChangeLetter();
				l = stand->getLetterForChange( x );
				stand->setForChangeLetter( EMPTY_FIELD , x );
				gameboard->setAddedLetter( l , nrx , nry );
			}
			else
				if( gameboard->addedLetter( nrx , nry ) )
				{
					where = NOWHERE;
					stand->turnOffForChangeLetter();
					l = stand->getLetterForChange( x );
					stand->setForChangeLetter( gameboard->letter( nrx , nry ) , x );
					gameboard->setAddedLetter( l , nrx , nry );
				}
			break;

		case GAMEBOARD:
			if( gameboard->isEmptyField( nrx , nry ) )
			{
				where = NOWHERE;
				gameboard->turnOffLetter();
				l = gameboard->letter( x , y );
				gameboard->removeAddedLetter( x , y );
				gameboard->setAddedLetter( l , nrx , nry );
			}
			else
				if( gameboard->addedLetter( nrx , nry ) )
				{
					where = NOWHERE;
					gameboard->turnOffLetter();
					l = gameboard->letter( x , y );
					gameboard->removeAddedLetter( x , y );
					gameboard->setAddedLetter( gameboard->letter( nrx , nry ) , x , y );
					gameboard->setAddedLetter( l , nrx , nry );
				}
			break;

		default:;    
	}
}

void Highlighting::turnOffHighlighting()
{
	switch( where )
	{
		case NOWHERE:
			break;

		case GAMEBOARD:
			where = NOWHERE;
			gameboard->turnOffLetter();
			break;
	
		case STAND:
			where = NOWHERE;
			stand->turnOffStandLetter();
			break;

		case FOR_CHANGE:
			where = NOWHERE;
			stand->turnOffForChangeLetter();
			break;

		default:;
	}
}

