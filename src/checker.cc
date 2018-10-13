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

#include "checker.h"

Checker::Checker()
{
	number_of_letters = 0;
	number_of_checked_letters = 0;

	zeroLetters();
}

void Checker::zeroLetters()
{
	for( int i=0; i<256; i++ )
		letters[i] = 0;
}

void Checker::setLetters( char *word , int length )
{
	for( int i=0; i<length; i++ )
		letters[ (unsigned char) word[i] ] = 0;

	for( int i=0; i<length; i++ )
		letters[ (unsigned char) word[i] ]++;
}

void Checker::setCheckedLetters( char *word , int length )
{
	/*
	 * Zakladam, ze ta funkcja bedzie rzeczywiscie rzadko wywolywana.
	 * Dobrze by bylo, gdyby zachodzilo length > 0.
	 * A jeszcze lepiej, gdyby word na wejsciu bylo posortowane.
	 */
	number_of_checked_letters = 1;

	checked_letters[0] = word[0];

	for( int i=1; i<length; i++ )
		if( word[i] != word[i-1] )
			checked_letters[number_of_checked_letters++] = word[i];

	number_of_letters = length;
}

void Checker::zeroByCheckedLetters()
{
	for( int i=0; i<number_of_letters; i++ )
		letters[ (unsigned char) checked_letters[i] ] = 0;
}

int Checker::checkAgainstWord( char *word , int length )
{
	for( int i=0; i<length; i++ )
		letters[ (unsigned char) word[i] ]--;

	for( int i=0; i<number_of_letters; i++ )
		if( letters[ (unsigned char) checked_letters[i] ] > 0 )
			return 0;

	return 1;
}

int Checker::substractLetter( char c )
{
	if( letters[ (unsigned char) c ] > 0 )
	{
		letters[ (unsigned char) c ]--;
		return false;
	}
	else
		return true;
}

int Checker::goodWord( int length )
{
	int sum = 0;
	
	for( int i=0; i<number_of_checked_letters; i++ )
		sum += letters[ (unsigned char) checked_letters[i] ];
	
	return ( sum == number_of_letters - length );
}

