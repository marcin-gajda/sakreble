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

#ifndef RULES_H
#define RULES_H

#include <libxml/tree.h>

#define NUMBER_OF_RULES 2

#define RULE_START_FIELD 0
#define RULE_END_GAME_CODITIONS 1

/*
 * Basic class for all rules.
 */
class Rule
{
	public:
		Rule();
		void setName( xmlChar * );
		xmlChar *getName() { return name; }
		int isActive() { return active; }

	protected:
		/*
		 * Rule's name.
		 */
		xmlChar *name;

		/*
		 * Indicates whenever rule is active.
		 */
		int active;
};

class FirstMoveRule : Rule
{
	public:
		void setFirstMoveField( int , int , int );
		int getFirstMoveXPos() { return x; }
		int getFirstMoveYPos() { return y; }
		int isFlexible() { return flexible; }
	
	protected:
		/*
		 * Field coordinates for first move.
		 */
		int x, y;

		/*
		 * Is field for first move flexible?
		 */
		int flexible;
};

class EndGameConditionsRule : Rule
{
	public:
		void setEndGameConditions( int , int );
		int endWithFullGameboard() { return fullGameboard; }
		int numberOfContinuousPausesToEnd() { return continuousPauses; }
	
	protected:
		/*
		 * Indicated whenever game should end after filling entire gameboard.
		 */
		int fullGameboard;

		/*
		 * Number of continuous pauses to end game. 0 indicates no limit.
		 */
		int continuousPauses;
};

class Rules
{
	public:
		Rules();
		~Rules();

		Rule *getRule( int rule_number ) { return rules[ rule_number ]; }

	private:
		Rule *rules[ NUMBER_OF_RULES ];
};


#endif

