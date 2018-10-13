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

#ifndef CHECKER_H
#define CHECKER_H

class Checker
{
	public:
		Checker();

		void setLetters( char * , int );
		void setCheckedLetters( char * , int );
		void zeroLetters();
		void zeroByCheckedLetters();
		int checkAgainstWord( char * , int );
		int substractLetter( char );	// odpowiedz - czy trzeba blank
		int goodWord( int );

	private:
		int letters[256];
		char checked_letters[15];	// Max na planszy
		int number_of_letters, number_of_checked_letters;
};


#endif
