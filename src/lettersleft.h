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

#ifndef LETTERS_LEFT_H
#define LETTERS_LEFT_H

#include <qwidget.h>

class QPushButton;

class Alphabet;
class Letters;

class LettersLeft : public QWidget
{
	Q_OBJECT

	public:
		LettersLeft( QWidget * , const char * );

		void setAlphabet( Alphabet *a ) { alphabet = a; }
		void setPlayerLetters( int which , Letters *l ) { playerLetters[which] = l; }
		void setWhichPlayer( int *kg ) { whichPlayer = kg; }

		public slots:
			void changeWithMyLetters();

	signals:
		void closeClicked();

	protected:
		void paintEvent( QPaintEvent * );

	private:
		Alphabet *alphabet;
		QPushButton *ok, *withMyLetters;
		Letters *playerLetters[2];
		int with_me;
		int *whichPlayer;
};

#endif
