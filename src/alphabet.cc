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

#include "alphabet.h"
#include "dupa.h"

Alphabet::Alphabet()
{
	for( int i=0; i<256; i++ )
	{
		alphabetLetters[i].inGame = 0;
		alphabetLetters[i].pointValue = 0;
		alphabetLetters[i].count = 0;
		alphabetLetters[i].name = NULL;
		alphabetLetters[i].show = NULL;
	}

	/*
	 * Start from 3, because 0 is reserved for blanks, 1 for empty fields
	 * and 2 for fields below the game.
	 */
	number_of_letters = 3;

	initializeLetters();
}

void Alphabet::initializeLetters()
{
	lettersLeft = 0;
	for( int i=0; i<256; i++ )
	{
		alphabetLetters[i].inGame = alphabetLetters[i].count;
		lettersLeft += alphabetLetters[i].inGame;
	}
}

/*
 * Searches for index of specified letter. Not efective.
 *
 * Returns -1 if not found.
 */
int Alphabet::findLetterIndex( xmlChar *letterName )
{
	for( int i=0; i<number_of_letters; i++ )
		if( xmlStrEqual( alphabetLetters[i].name , letterName ) )
			return i;

	return -1;
}

/*
 * Sets single alphabet letter.
 *
 * Function should be called only by config data file parser (probably).
 */
void Alphabet::addAlphabetLetter( xmlChar *name , int count , int value , xmlChar *show )
{
	alphabetLetters[number_of_letters].count = count;
	alphabetLetters[number_of_letters].inGame = 0;
	alphabetLetters[number_of_letters].pointValue = value;

	alphabetLetters[number_of_letters].name = xmlStrdup( name );

	/*
	 * Parameter 'show' in config file is optioal. If not present,
	 * 'name' parameter is used.
	 */
	alphabetLetters[number_of_letters].show = show != NULL ? xmlStrdup( show ) : xmlStrdup( name );

	number_of_letters++;

	/*
	 * Letter number 10 is reserverd for word separator.
	 */
	if( number_of_letters == 10 )
		number_of_letters++;
}

/*
 * Sets number of blanks.
 */
void Alphabet::setNumberOfBlanks( int count )
{
	alphabetLetters[0].count = count;
	alphabetLetters[0].inGame = 0;
	alphabetLetters[0].pointValue = 0;

	alphabetLetters[0].name = xmlCharStrdup( " " );
	alphabetLetters[0].show = xmlCharStrdup( " " );
}

/*
 * Returns number of unique letters in alphabet.
 */
int Alphabet::getNumberOfLetters()
{
	/*
	 * First three indices are reserved. Index 10 too.
	 */
	return number_of_letters > 10 ? number_of_letters - 4 : number_of_letters - 3;
}

/*
 * Returns number of unique letters in alphabet. Blank is included.
 */
int Alphabet::getNumberOfLettersWithBlank()
{
	/*
	 * First three indices are reserved. Index 10 too.
	 */
	return number_of_letters > 10 ? number_of_letters - 3 : number_of_letters - 2;
}

/*
 * Returns letter index in letters table. Gets letter number counting
 * from zero (blank), without holes.
 */
int Alphabet::getLetterInternalNumber( int n )
{
	switch( n )
	{
		case 0:
			return 0;

		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			return n + 2;

		default:
			return n + 3;
	}
}

AlphabetIterator Alphabet::getIterator()
{
	return AlphabetIterator( this );
}

/*
 * Metods for AlphabetIterator class.
 */

/*
 * Constructor.
 */
AlphabetIterator::AlphabetIterator( Alphabet *a )
{
	alphabet = a;

	current = -1;
}

/*
 * Rewinds iterator to the beginning.
 */
void AlphabetIterator::start()
{
	current = -1;
}

/*
 * Returns letter number in alphabet for current iterator position.
 */
int AlphabetIterator::getValue()
{
	return current;
}

/*
 * Goes to the next iterator position. Returns false if there is no
 * more letters to iterate.
 */
int AlphabetIterator::next()
{
	if( current >= alphabet->getLetterTableSize() )
		return false;

	current++;

	if( current == 1 )
		current = 3;
	else
	{
		if( current == 10 )
			current = 11;
	}

	return current < alphabet->getLetterTableSize();
}

