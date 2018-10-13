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

#include <qlabel.h>
#include <qframe.h>
#include <qwindefs.h>
#include <stdio.h>

#include "dupa.h"
#include "letters.h"

Letters::Letters( QWidget *parent , const char *name ) : QWidget( parent , name )
{
	setMaximumSize( 20 , 20 );

	showNumberOfLettersLabel = new QLabel( this , "pokaz" );
	showNumberOfLettersLabel->setGeometry( 0 , 0 , 20 , 20 );
	showNumberOfLettersLabel->setFrameStyle( QFrame::Sunken | QFrame::Panel );
	showNumberOfLettersLabel->setAlignment( AlignCenter );

	number_of_letters = 7;
	letters = new char[7];

	removeLetters();

	showNumberOfLettersLabel->setNum( numberOfLetters() );
}

Letters::Letters( int  ile , QWidget *parent , const char *name ) : QWidget( parent , name )
{
	showNumberOfLettersLabel = new QLabel( this , "pokaz" );
	showNumberOfLettersLabel->setGeometry( 0 , 0 , 10 , 10 );
	showNumberOfLettersLabel->setFrameStyle( QFrame::Sunken | QFrame::Panel );
	showNumberOfLettersLabel->setAlignment( AlignTop | AlignHCenter );

	number_of_letters = ile;

	if( number_of_letters <= 0 )
		number_of_letters = 7;

	letters = new char[number_of_letters];

	removeLetters();

	showNumberOfLettersLabel->setNum( numberOfLetters() );
}

Letters::~Letters()
{
	if( number_of_letters > 0 )
		delete letters;
}

void Letters::removeLetters()
{
	for( int i=0; i<number_of_letters; i++ )
		letters[i] = EMPTY_FIELD;

	showNumberOfLettersLabel->setNum( numberOfLetters() );
}

void Letters::changeLetter( char l , int where )
{
	letters[where] = l;

	showNumberOfLettersLabel->setNum( numberOfLetters() );
}

void Letters::removeLetter( int where )
{
	letters[where] = EMPTY_FIELD;

	showNumberOfLettersLabel->setNum( numberOfLetters() );
}

void Letters::changeLetters( char *pattern , int ile )
{
	for( int i=0; i<ile; i++ )
		letters[i] = pattern[i];

	showNumberOfLettersLabel->setNum( numberOfLetters() );
}

void Letters::removeLetter( char l )
{
	for( int i=0; i<number_of_letters; i++ )
		if( letters[i] == l )
		{
			letters[i] = EMPTY_FIELD;
			break;
		}

	showNumberOfLettersLabel->setNum( numberOfLetters() );
}

int Letters::numberOfLetters()
{
	int ile = 0;

	for( int i=0; i<number_of_letters; i++ )
		if( letters[i] != EMPTY_FIELD )
			ile++;

	return ile;
}

void Letters::updateOnPicture()
{
	showNumberOfLettersLabel->setNum( numberOfLetters() );
}

void Letters::moveEmptyFieldsToEnd()
{
	for( int i=number_of_letters-1; i>=0; i-- )
		if( letters[i] == EMPTY_FIELD )
			for( int j=i; j<number_of_letters-1; j++ )
			{
				letters[j] = letters[j+1];
				letters[j+1] = EMPTY_FIELD;
			}
}

int Letters::getLetterCount( char c )
{
	int result = 0;

	for( int i=0; i<number_of_letters; i++ )
		if( letters[i] == c )
			result++;

	return result;
}
