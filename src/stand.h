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

#ifndef STAND_H
#define STAND_H

#include <qwidget.h>

class Alphabet;

class Stand : public QWidget
{
	Q_OBJECT

	public:
		Stand( QWidget *parent = 0 , const char *name = 0 );

		char *getLettersOnStand() { return letters; }
		char *getForChangeLetters() { return for_change; }
		char getLetterOnStand( int i ) { return letters[i]; }
		char getLetterForChange( int i ) { return for_change[i]; }
		void setLetterOnStand( char , int );
		void setLettersOnStand( char * , int );
		void setForChangeLetter( char , int );
		void setAlphabet( Alphabet *a ) { alphabet = a; }
		void highlightLetterOnStand( int );
		void highlightForChangeLetter( int );
		void turnOffStandLetter();
		void turnOffForChangeLetter();
		void changeStandLetters( int , int );
		void changeForChangeLetters( int , int );
		int areForChangeLetters();
		int areLettersOnStand();
		void removeForChangeLetters();

	signals:
		void standClicked( int );
		void forChangeClicked( int );
		void forChangeClickPossible( int );

	protected:
		void paintEvent( QPaintEvent * );
		void mousePressEvent( QMouseEvent * );

	private:
		char letters[7];
		char for_change[7];
		int isHighlighted;
		Alphabet *alphabet;
};

#endif
