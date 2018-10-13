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

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <qwidget.h>

class Alphabet;
class Rules;

struct Field
{
	/*
	 * Letter placed on the field.
	 */
    char letter;

    int bonus;

	/*
	 * Is this letter placed on the gameboard now (in current move)?
	 */
    int added;

	/*
	 * Is letter changed with blank?
	 */
	int blank;
};

class Gameboard : public QWidget
{
	Q_OBJECT

	public:
		Gameboard( QWidget *parent = 0 , const char *name = 0 );

		void setRegion( int , int , int , int , int );
		int bonus( int , int );
		char letter( int , int );
		int checkBounds( int , int );
		int isEmptyField( int , int );
		int isLetterField( int , int );
		int isDisabledField( int , int );
		int isPatternStartField( int , int , int );
		int isPatternEndField( int , int , int );
		int isBonusField( int , int );
		int isBlankLetter( int , int );
		void setLetter( char , int , int );
		void setBonus( int , int , int );
		void markBlankPlaced( int , int );
		char letterPoint( char );
		void setAlphabet( Alphabet *a ) { alphabet = a; }
		void setRules( Rules *r ) { rules = r; }
		void setAddedLetter( char , int , int );
		void removeAddedLetter( int , int );
		int addedLetter( int i , int j ) { return fields[i][j].added; }
		void highlightLetter( int , int );
		void highlightBlank();
		void turnOffBlank();
		void turnOffLetter();
		int isWordCorrectlyAdded();
		int thereAreAddedLetters();
		int thereAreOnlyAddedLetters();
		void removeAddedLetters();
		int thereAreLetters();
		int getPointValue( int , int , int );
		int getPointValue();
		void removeAddingIndicatorFromAddedLetters();
		void clearGameboard();
		int isBlankAdded();
		void changeBlank( char );
		void highlightLetterUstawionaPrzezKomputer( char , int , int );
		void highlightBlankLetterUstawionaPrzezKomputer( char , int , int );

	signals:
		void gameboardClicked( int , int );
		void enableOkButton( int );

	protected:
		void paintEvent( QPaintEvent * );
		void mousePressEvent( QMouseEvent * );

	private:
		Field fields[15][15];
		Alphabet *alphabet;
		Rules *rules;
		int highlightingXPos, highlightingYPos;
};

#endif
