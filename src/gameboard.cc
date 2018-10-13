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

#include "gameboard.h"
#include "dupa.h"
#include "alphabet.h"
#include "rules.h"

Gameboard::Gameboard( QWidget *parent , const char *name ) : QWidget( parent, name )
{
	int i, j;

	for( i=0; i<15; i++ )
		for( j=0; j<15; j++ )
		{
			fields[i][j].letter = EMPTY_FIELD;
			fields[i][j].bonus = DISABLED_FIELD;
			fields[i][j].added = false;
			fields[i][j].blank = false;
		}
	
	highlightingXPos = highlightingYPos = -1;
}

/*
 * Fills gameboard region with typed field type.
 */
void Gameboard::setRegion( int left , int top , int width , int height , int type )
{
	for( int i=left; i<left+width; i++ )
		for( int j=top; j<top+height; j++ )
		{
			setBonus( type , i , j );

			setLetter( EMPTY_FIELD , i , j );
		}
}

const QRect gameboardRect( 0 , 0 , 450 , 450 );

void Gameboard::paintEvent( QPaintEvent *e )
{
	QPainter paint;
	QString s;
	QRect updateR = e->rect();

	paint.begin( this );

	paint.setPen( black );

	for( int i=0; i<15; i++ )
		for( int j=0; j<15; j++ )
		{
			if(	!updateR.intersects( QRect( i*30+1 , j*30+1 , 28 , 28 ) ) )
				continue;

			if( isEmptyField( i , j ) )
				switch( fields[i][j].bonus )
				{
					case TWO_LETTER_BONUS:
						paint.setBrush( blue );
						break;
					case THREE_LETTER_BONUS:
						paint.setBrush( darkBlue );
						break;
					case TWO_WORD_BONUS:
						paint.setBrush( red );
						break;
					case THREE_WORD_BONUS:
						paint.setBrush( darkRed );
						break;
					default:
						paint.setBrush( darkGreen );
						break;
				}
			else
			{
				if( highlightingXPos == i && highlightingYPos == j )
					paint.setBrush( yellow );
				else
				{
					if( fields[i][j].added )
						paint.setBrush( lightGray );
					else
						paint.setBrush( darkGray );
				}
			}

			paint.drawRect( i*30 + 1 , j*30 + 1 , 28 , 28 );

			if( isLetterField( i , j ) )
			{
				s.sprintf( "%s" , alphabet->alphabetLetterToShow( fields[i][j].letter ) );
				paint.setFont( QFont( "Times" , 18 , QFont::Bold ) );
				paint.drawText( i*30 + 1 , j*30 + 1 , 26 , 26 , AlignCenter , s , 1 );

				if( alphabet->getLetterPoints( fields[i][j].letter ) > 0 )
				{
					s.sprintf( "%i" , alphabet->getLetterPoints( fields[i][j].letter ) );
					paint.setFont( QFont( "Times" , 10 , QFont::Bold ) );
					paint.drawText( i*30 + 19 , j*30 + 16 , 10 , 10 , AlignCenter , s , 1 );
				}
			}
			else
			{
				if( isBonusField( i , j ) )
				{
					paint.setFont( QFont( "Arial" , 10 , QFont::Bold ) );
					switch( fields[i][j].bonus )
					{
						case TWO_LETTER_BONUS:
							s.sprintf( "2L" );
							paint.drawText( i*30+1 , j*30+1 , 28 , 28 , AlignCenter , s , 2 );
							break;
						case THREE_LETTER_BONUS:
							s.sprintf( "3L" );
							paint.drawText( i*30+1 , j*30+1 , 28 , 28 , AlignCenter , s , 2 );
							break;
						case TWO_WORD_BONUS:
							s.sprintf( "2W" );
							paint.drawText( i*30+1 , j*30+1 , 28 , 28 , AlignCenter , s , 2 );
							break;
						case THREE_WORD_BONUS:
							s.sprintf( "3W" );
							paint.drawText( i*30+1 , j*30+1 , 28 , 28 , AlignCenter , s , 2 );
							break;
						default:;
					}
				}
			}
		}

	paint.end();
}

void Gameboard::mousePressEvent( QMouseEvent *e )
{
	if ( e->button() != LeftButton )
		return;

	emit gameboardClicked( e->x() / 30 , e->y() / 30 );
}

/*
 * Tu rzeczy nie zwiazane z rysowaniem
 */

/*
 * Returns true if coordinates are in gameboard.
 */
int Gameboard::checkBounds( int i , int j )
{
	return i >= 0 && j >= 0 && i < 15 && j <15;
}

int Gameboard::bonus( int i , int j )
{
	return fields[i][j].bonus;
}

char Gameboard::letter( int i , int j )
{
	return fields[i][j].letter;
}

/*
 * Returns true when specified field is empty.
 * "Empty" means that there is no letter on field and
 * it is not disabled.
 */
int Gameboard::isEmptyField( int i , int j )
{
	return checkBounds( i , j ) && fields[i][j].letter == EMPTY_FIELD && fields[i][j].bonus != DISABLED_FIELD;
}

/*
 * Returns true if specified field contains a letter.
 */
int Gameboard::isLetterField( int i , int j )
{
	return checkBounds( i , j ) && fields[i][j].letter != EMPTY_FIELD && fields[i][j].bonus != DISABLED_FIELD;
}

/*
 * Returns true if specified field is disabled.
 */
int Gameboard::isDisabledField( int i , int j )
{
	return !checkBounds( i , j ) || fields[i][j].bonus == DISABLED_FIELD;
}

/*
 * Returns true if specified field is good for pattern begin.
 */
int Gameboard::isPatternStartField( int i , int j , int horizont )
{
	if( isDisabledField( i , j ) )
		return false;

	if( horizont == HORIZONTAL )
		return i == 0 || isDisabledField( i-1 , j ) || isEmptyField( i-1 , j );
	else
		return j == 0 || isDisabledField( i , j-1 ) || isEmptyField( i , j-1 );
}

/*
 * Returns true if specified field is good for pattern end.
 */
int Gameboard::isPatternEndField( int i , int j , int horizont )
{
	if( isDisabledField( i , j ) )
		return false;

	if( horizont == HORIZONTAL )
		return i == 14 || isDisabledField( i+1 , j ) || isEmptyField( i+1 , j );
	else
		return j == 14 || isDisabledField( i , j+1 ) || isEmptyField( i , j+1 );
}

/*
 * Returns true if specified field contains letter, which was changed
 * with blank.
 */
int Gameboard::isBlankLetter( int i , int j )
{
	return fields[i][j].blank;
}

/*
 * Returns true if specified field is special (eg. bonus field).
 */
int Gameboard::isBonusField( int i , int j )
{
	return checkBounds( i , j ) && fields[i][j].bonus != NORMAL;
}

void Gameboard::setLetter( char l , int i , int j )
{
	if( fields[i][j].letter == l )
		return;

	fields[i][j].letter = l;
	fields[i][j].blank = l == BLANK;

	repaint( QRect( i*30+1 , j*30+1 , 28 , 28 ) );
}

void Gameboard::setBonus( int p , int i , int j )
{
	fields[i][j].bonus = p;
}

/*
 * Marks field with letter, that this was blank letter.
 */
void Gameboard::markBlankPlaced( int i , int j )
{
	if( isLetterField( i , j ) )
		fields[i][j].blank = true;
}

/*
 * Checks, if new letters placed on the gameboard are correctly added.
 */
int Gameboard::isWordCorrectlyAdded()
{
	int firstAddedLetterXPos = -1, firstAddedLetterYPos = -1;
	int currentAddedLetterXPos, currentAddedLetterYPos;
	int found_not_added_letter = 0;
	int numberOfAddedLetters = 0, number_of_added_letters = 0;
	int horizont;

	/*
	 * Find first added letter and number of added letters.
	 */
	for( int i=0; i<15; i++ )
		for( int j=0; j<15; j++ )
			if( fields[i][j].added )
			{
				if( firstAddedLetterXPos == -1 )
				{
					firstAddedLetterXPos = i;
					firstAddedLetterYPos = j;
				}

				numberOfAddedLetters++;
			}

	horizont = VERTICAL;

	/*
	 * Determine, in which direction word was added.
	 */
	for( int i=firstAddedLetterXPos+1; i<15; i++ )
	{
		if( fields[i][firstAddedLetterYPos].added )
			horizont = HORIZONTAL;
	}

	currentAddedLetterXPos = firstAddedLetterXPos;
	currentAddedLetterYPos = firstAddedLetterYPos;

	while( currentAddedLetterXPos < 15 && currentAddedLetterYPos < 15 )
	{
		if( fields[currentAddedLetterXPos][currentAddedLetterYPos].letter == EMPTY_FIELD )
			break;

		if( currentAddedLetterXPos > 0 )
			if( !fields[currentAddedLetterXPos-1][currentAddedLetterYPos].added && fields[currentAddedLetterXPos-1][currentAddedLetterYPos].letter != EMPTY_FIELD )
				found_not_added_letter = 1;

		if( currentAddedLetterYPos > 0 )
			if( !fields[currentAddedLetterXPos][currentAddedLetterYPos-1].added && fields[currentAddedLetterXPos][currentAddedLetterYPos-1].letter != EMPTY_FIELD )
				found_not_added_letter = 1;

		if( currentAddedLetterXPos < 14 )
			if( !fields[currentAddedLetterXPos+1][currentAddedLetterYPos].added && fields[currentAddedLetterXPos+1][currentAddedLetterYPos].letter != EMPTY_FIELD )
				found_not_added_letter = 1;

		if( currentAddedLetterYPos < 14 )
			if( !fields[currentAddedLetterXPos][currentAddedLetterYPos+1].added && fields[currentAddedLetterXPos][currentAddedLetterYPos+1].letter != EMPTY_FIELD )
				found_not_added_letter = 1;

		if( fields[currentAddedLetterXPos][currentAddedLetterYPos].added )
			number_of_added_letters++;

		if( horizont == HORIZONTAL )
			currentAddedLetterXPos++;
		else
			currentAddedLetterYPos++;
	}

	if( number_of_added_letters == numberOfAddedLetters )
	{
		if( found_not_added_letter )
			return 1;
		else
		{
			/*
			 * Possibly this is fist move in the game.
			 */
			if( thereAreOnlyAddedLetters() )
			{
				/*
				 * Yes! This is first move in the game - only letters marked as
				 * added are present. Check rules to determine, if this is
				 * correct move.
				 */

				/*
				 * Two or more letters have to be placed.
				 */
				if( numberOfAddedLetters > 1 )
				{
					if( rules->getRule( RULE_START_FIELD )->isActive() )
					{
						FirstMoveRule *rule = (FirstMoveRule *)(rules->getRule( RULE_START_FIELD ));

						if( rule->isFlexible() )
							return isLetterField( rule->getFirstMoveXPos() , rule->getFirstMoveYPos() );
						else
							return isLetterField( rule->getFirstMoveXPos() , rule->getFirstMoveYPos() ) &&
									rule->getFirstMoveXPos() == firstAddedLetterXPos &&
									rule->getFirstMoveYPos() == firstAddedLetterYPos;
					}
					else
						/*
						 * Rule isn't active - user may begin game everywhere.
						 */
						return true;
				}
			}
		}
	}

	return 0;
}

void Gameboard::highlightLetterUstawionaPrzezKomputer( char l , int i , int j )
{
	fields[i][j].letter = l;
	fields[i][j].added = true;
	fields[i][j].blank = l == BLANK;

	repaint( QRect( i*30+1 , j*30+1 , 28 , 28 ) );
}

void Gameboard::highlightBlankLetterUstawionaPrzezKomputer( char l , int i , int j )
{
	fields[i][j].letter = l;
	fields[i][j].added = true;
	fields[i][j].blank = true;

	repaint( QRect( i*30+1 , j*30+1 , 28 , 28 ) );
}

void Gameboard::setAddedLetter( char l , int i , int j )
{
	fields[i][j].letter = l;
	fields[i][j].added = true;
	fields[i][j].blank = l == BLANK;


	repaint( QRect( i*30+1 , j*30+1 , 28 , 28 ) );

	if( isWordCorrectlyAdded() )
		emit enableOkButton( true );
	else
		emit enableOkButton( false );
}

void Gameboard::removeAddedLetter( int i , int j )
{
	if( !fields[i][j].added )
		return;

	fields[i][j].letter = EMPTY_FIELD;
	fields[i][j].added = false;
	fields[i][j].blank = false;

	repaint( QRect( i*30+1 , j*30+1 , 28 , 28 ) );

	if( isWordCorrectlyAdded() )
		emit enableOkButton( true );
	else
		emit enableOkButton( false );
}

void Gameboard::removeAddedLetters()
{
	for( int i=0; i<15; i++ )
		for( int j=0; j<15; j++ )
			if( fields[i][j].added )
				removeAddedLetter( i , j );
}

void Gameboard::highlightLetter( int i , int j )
{
	if( fields[i][j].letter == EMPTY_FIELD )
		return;

	if( i == highlightingXPos && j == highlightingYPos )
		return;

	if( !fields[i][j].added )
		return;

	highlightingXPos = i;
	highlightingYPos = j;

	repaint( QRect( i*30+1 , j*30+1 , 28 , 28 ) );
}

void Gameboard::turnOffLetter()
{
	int i, j;

	i = highlightingXPos;
	j = highlightingYPos;

	highlightingXPos = -1;
	highlightingYPos = -1;

	repaint( QRect( i*30+1 , j*30+1 , 28 , 28 ) );
}

/*
 * Returns true if gameboard contains added letters (letters placed but
 * not confirmed).
 */
int Gameboard::thereAreAddedLetters()
{
	for( int i=0; i<15; i++ )
		for( int j=0; j<15; j++ )
			if( fields[i][j].added )
				return true;

	return false;
}

/*
 * Returns true in gameboard contains only added letters (letters placed but
 * not confirmed). Empty gameboard returns true also.
 */
int Gameboard::thereAreOnlyAddedLetters()
{
	for( int i=0; i<15; i++ )
		for( int j=0; j<15; j++ )
			if( isLetterField( i , j ) && !fields[i][j].added )
				return false;

	return true;
}

int Gameboard::thereAreLetters()
{
	for( int i=0; i<15; i++ )
		for( int j=0; j<15; j++ )
			if( isLetterField( i , j ) )
				return true;

	return false;
}

int Gameboard::getPointValue( int i , int j , int horizont )
{
	int points = 0;
	int number_of_added_letters = 0;
	int word_multiplikator = 1;

	while( i<15 && j<15 )
	{
		if( fields[i][j].letter == EMPTY_FIELD )
			break;

		if( !fields[i][j].added )
		{
			if( !fields[i][j].blank )
				points += alphabet->getLetterPoints( fields[i][j].letter );
		}
		else
		{
			number_of_added_letters++;

			switch( fields[i][j].bonus )
			{
				case TWO_LETTER_BONUS:
					if( !fields[i][j].blank )
						points += 2 * alphabet->getLetterPoints( fields[i][j].letter );
					break;

				case THREE_LETTER_BONUS:
					if( !fields[i][j].blank )
						points += 3 * alphabet->getLetterPoints( fields[i][j].letter );
					break;

				case TWO_WORD_BONUS:
					if( !fields[i][j].blank )
						points += alphabet->getLetterPoints( fields[i][j].letter );
					word_multiplikator *= 2;
					break;

				case THREE_WORD_BONUS:
					if( !fields[i][j].blank )
						points += alphabet->getLetterPoints( fields[i][j].letter );
					word_multiplikator *= 3;
					break;

				default:
					if( !fields[i][j].blank )
						points += alphabet->getLetterPoints( fields[i][j].letter );
					break;
			}
		}

		if( horizont == HORIZONTAL )
			i++;
		else
			j++;
	}

	points *= word_multiplikator;

	if( number_of_added_letters == 7 )
		points += 50;

	return points;
}

int Gameboard::getPointValue()
{
	int firstAddedLetterXPos = -1, firstAddedLetterYPos = -1, direction_up = 0, direction_left = 0;
	int horizont = HORIZONTAL;
	int points = 0, second_letter_added = 0;

	if( isWordCorrectlyAdded() )
	{
		for( int i=0; i<15; i++ )
			for( int j=0; j<15; j++ )
				if( fields[i][j].added )
				{
					if( firstAddedLetterXPos == -1 )
					{
						firstAddedLetterXPos = i;
						firstAddedLetterYPos = j;
					}
					break;
				}

		for( int i=firstAddedLetterXPos+1; i<15; i++ )
			if( addedLetter( i , firstAddedLetterYPos ) )
			{
				horizont = HORIZONTAL;
				second_letter_added = 1;
				break;
			}

		if( !second_letter_added )
			for( int j=firstAddedLetterYPos+1; j<15; j++ )
				if( addedLetter( firstAddedLetterXPos , j ) )
				{
					horizont = VERTICAL;
					second_letter_added = 1;
					break;
				}

		if( !second_letter_added )
		{
			if( firstAddedLetterXPos > 0 && letter( firstAddedLetterXPos - 1 , firstAddedLetterYPos ) != EMPTY_FIELD )
				horizont = HORIZONTAL;

			if( firstAddedLetterXPos < 14 && letter( firstAddedLetterXPos + 1 , firstAddedLetterYPos ) != EMPTY_FIELD )
				horizont = HORIZONTAL;

			if( firstAddedLetterYPos > 0 && letter( firstAddedLetterXPos , firstAddedLetterYPos - 1 ) != EMPTY_FIELD )
				horizont = VERTICAL;

			if( firstAddedLetterYPos < 14 && letter( firstAddedLetterXPos , firstAddedLetterYPos + 1 ) != EMPTY_FIELD )
				horizont = VERTICAL;
		}

		if( horizont == HORIZONTAL )
			while( firstAddedLetterXPos > 0 && fields[firstAddedLetterXPos-1][firstAddedLetterYPos].letter != EMPTY_FIELD )
				firstAddedLetterXPos--;
		else
			while( firstAddedLetterYPos > 0 && fields[firstAddedLetterXPos][firstAddedLetterYPos-1].letter != EMPTY_FIELD )
				firstAddedLetterYPos--;

		points += getPointValue( firstAddedLetterXPos , firstAddedLetterYPos , horizont );

		while( firstAddedLetterXPos < 15 && firstAddedLetterYPos < 15 )
		{
			if( fields[ firstAddedLetterXPos ][ firstAddedLetterYPos ].letter == EMPTY_FIELD )
				break;

			if( addedLetter( firstAddedLetterXPos , firstAddedLetterYPos ) )
			{
				if( horizont == HORIZONTAL )
				{
					direction_up = 0;
					while( firstAddedLetterYPos-direction_up-1 >= 0 )
						if( letter( firstAddedLetterXPos , firstAddedLetterYPos-direction_up-1 ) != EMPTY_FIELD )
							direction_up++;
						else
							break;

					if( direction_up > 0 || ( firstAddedLetterYPos < 14 && letter( firstAddedLetterXPos , firstAddedLetterYPos + 1 ) != EMPTY_FIELD ) )
						points += getPointValue( firstAddedLetterXPos , firstAddedLetterYPos-direction_up , VERTICAL );
				}
				else
				{
					direction_left = 0;
					while( firstAddedLetterXPos-direction_left-1 >= 0 )
						if( letter( firstAddedLetterXPos-direction_left-1 , firstAddedLetterYPos ) != EMPTY_FIELD )
							direction_left++;
						else
							break;

					if( direction_left > 0 || ( firstAddedLetterXPos < 14 && letter( firstAddedLetterXPos + 1 , firstAddedLetterYPos ) != EMPTY_FIELD ) )
						points += getPointValue( firstAddedLetterXPos-direction_left , firstAddedLetterYPos , HORIZONTAL );
				}
			}

			if( horizont == HORIZONTAL )
				firstAddedLetterXPos++;
			else
				firstAddedLetterYPos++;
		}
	}

	return points;
}

void Gameboard::removeAddingIndicatorFromAddedLetters()
{
	for( int i=0; i<15; i++ )
		for( int j=0; j<15; j++ )
			if( fields[i][j].added )
			{
				fields[i][j].added = false;
				repaint( QRect( i*30+1 , j*30+1 , 28 , 28 ) );
			}	
}

void Gameboard::clearGameboard()
{
	for( int i=0; i<15; i++ )
		for( int j=0; j<15; j++ )
		{
			fields[i][j].letter = EMPTY_FIELD;
			fields[i][j].added = false;
			fields[i][j].blank = false;
		}

	repaint( QRect( 0 , 0 , 450 , 450 ) );
}

int Gameboard::isBlankAdded()
{
	for( int i=0; i<15; i++ )
		for( int j=0; j<15; j++ )
			if( fields[i][j].letter == BLANK )
				return 1;

	return 0;
}

void Gameboard::changeBlank( char to_what )
{
	for( int i=0; i<15; i++ )
		for( int j=0; j<15; j++ )
			if( fields[i][j].letter == BLANK )
			{
				fields[i][j].letter = to_what;
				repaint( QRect( i*30+1 , j*30+1 , 28 , 28 ) );
				return;
			}

}

void Gameboard::highlightBlank()
{
	for( int i=0; i<15; i++ )
		for( int j=0; j<15; j++ )
			if( fields[i][j].letter == BLANK )
			{
				highlightLetter( i , j );
				return;
			}

}

void Gameboard::turnOffBlank()
{
	turnOffLetter();
}

