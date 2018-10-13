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

#include "valid.h"

MyIntValidator::MyIntValidator( QObject *parent , const char *name ) :
	QIntValidator( parent , name ) {}

MyIntValidator::MyIntValidator( int b , int t , QObject *parent , const char *name ) :
	QIntValidator( b , t , parent , name ) {}

void MyIntValidator::fixup( QString &input )
{
	if( input.toInt() > top() )
		input.setNum( top() );
	else
	{
		if( input.toInt() < bottom() )
			input.setNum( bottom() );
	}
}

QValidator::State MyIntValidator::validate( QString &input , int &pos )
{
	unsigned int oldlen = strlen( input );

	input.setNum( input.toInt() );	// To usuwa literki

	if( oldlen != strlen( input ) )
		pos = strlen( input );

	if( input.toInt() >= bottom() && input.toInt() <= top() )
		return Valid;

	if( input.toInt() > top() )
	{
		fixup( input );
		return Valid;
	}

	if( input.toInt() < 0 && bottom() >= 0 )
	{
		fixup( input );
		return Valid;
	}

	if( input.toInt() > 0 && top() <= 0 )
	{
		fixup( input );
		return Valid;
	}

	return Acceptable;
}
