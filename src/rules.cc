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

#include "rules.h"

/*
 * Functions for abstract Rule class.
 */

Rule::Rule()
{
	active = false;
	name = NULL;
}

void Rule::setName( xmlChar *n )
{
	name = xmlStrdup( n );
}

/*
 * Functions for FirstMoveRule class.
 */

void FirstMoveRule::setFirstMoveField( int xx , int yy , int fl )
{
	x = xx;
	y = yy;
	flexible = fl;

	active = true;
}

/*
 * Functions for EndGameConditionsRule class.
 */

void EndGameConditionsRule::setEndGameConditions( int ewfg , int cp )
{
	fullGameboard = ewfg;
	continuousPauses = cp;

	active = true;
}

/*
 * Functions for Rules class.
 */

Rules::Rules()
{
	rules[ RULE_START_FIELD ] = (Rule *)(new FirstMoveRule());
	rules[ RULE_END_GAME_CODITIONS ] = (Rule *)(new EndGameConditionsRule());
}

Rules::~Rules()
{
	for( int i=0; i<RULE_START_FIELD; i++ )
		delete rules[i];
}

