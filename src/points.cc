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

#include <qframe.h>
#include <qlabel.h>
#include <qwindefs.h>

#include "points.h"

Points::Points( QWidget *parent , const char *name ) : QWidget( parent, name )
{
    points = 0;

    setMinimumSize( 100 , 40 );
    setMaximumSize( 100 , 40 );

    kto = new QLabel( this , "kto" );
    kto->setGeometry( 0 , 0 , width() , height() );
    kto->setFrameStyle( QFrame::Sunken | QFrame::Panel );
    kto->setAlignment( AlignTop | AlignHCenter );

    ile = new QLabel( this , "ile" );
    ile->setFrameStyle( QFrame::Sunken | QFrame::Panel );
    ile->setGeometry( 10 , 20 , width() - 20 , height() - 25 );
    ile->setAlignment( AlignCenter );
}

void Points::zeroPoints()
{
    points = 0;

    ile->setText( "0" );
}

void Points::addPoints( int plus )
{
    points += plus;

    ile->setNum( points );
}

int Points::getPoints()
{
    return points;
}

void Points::setName( const char *name )
{
    kto->setText( name );
}

void Points::setPoints( int number_of_points )
{
    points = number_of_points;
    ile->setNum( number_of_points );
}
