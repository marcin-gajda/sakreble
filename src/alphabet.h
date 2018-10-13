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

#ifndef ALPHABET_H
#define ALPHABET_H

#include <qwidget.h>
#include <libxml/xmlstring.h>

struct Letter
{
	/*
	 * Point value of letter.
	 */
	int pointValue;

	/*
	 * Number of letters when game begins.
	 */
	int count;

	/*
	 * Number of letters during game.
	 */
	int inGame;

	/*
	 * Letter name (in UTF).
	 */
	xmlChar *name;

	/*
	 * Letter shape to display (in UTF).
	 */
	xmlChar *show;
};

class Alphabet;

/*
 * Class for iterating with alphabet letters.
 * It allows to go through all alphabet letters omitting
 * reserved integers (eg. 10).
 */
class AlphabetIterator
{
	public:
		AlphabetIterator( Alphabet * );

		int next();
		int getValue();
		void start();
	
	private:
		Alphabet *alphabet;
		int current;
};

class Alphabet
{
	public:
		Alphabet();

		int getLetterPoints( char c ) { return alphabetLetters[(unsigned char)c].pointValue; }
		void initializeLetters();
		int letter( char i ) { return alphabetLetters[(unsigned char)i].inGame; }
		int getLettersLeft() { return lettersLeft; }
		void setLettersLeft( int p ) { lettersLeft = p; }
		void substractLettersLeft() { lettersLeft--; }
		void addLettersLeft() { lettersLeft++; }
		void substractLetter( char c ) { alphabetLetters[(unsigned char)c].inGame--; }
		void addLetter( char c ) { alphabetLetters[(unsigned char)c].inGame++; }
		void setLetter( char c , int ile ) { alphabetLetters[(unsigned char)c].inGame = ile; }
		xmlChar *alphabetLetter( int which_f ) { return alphabetLetters[which_f].name; }
		xmlChar *alphabetLetterToShow( int letter_number ) { return alphabetLetters[letter_number].show; }
		int findLetterIndex( xmlChar * );
		void setNumberOfBlanks( int );
		int getNumberOfLetters();
		int getNumberOfLettersWithBlank();
		int getLetterInternalNumber( int );
		int getLetterTableSize() { return number_of_letters; }
		AlphabetIterator getIterator();

		void addAlphabetLetter( xmlChar * , int , int , xmlChar * );

	private:
		Letter alphabetLetters[256];
		
		/*
		 * Overall number of letters left during game.
		 */
		int lettersLeft;

		/*
		 * Number of unique letters.
		 */
		int number_of_letters;
};
#endif
