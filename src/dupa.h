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

#ifndef DUPA_H
#define DUPA_H


#define NORMAL 0
#define TWO_LETTER_BONUS 1
#define THREE_LETTER_BONUS 2
#define TWO_WORD_BONUS 3
#define THREE_WORD_BONUS 4
#define DISABLED_FIELD 5

#define BLANK '\0'
#define EMPTY_FIELD '\1'
#define BELOW_THE_GAME '\2'

#define HORIZONTAL 0
#define VERTICAL 1

#define MAX_WORD_LENGTH 15

#define VIRTUAL_PLAYER 0
#define REAL_PLAYER 1

struct link_struct;
struct word_struct;

/*
 * Pattern struct. It describes single slot on gameboard, where computer
 * may put its letters to get valid word.
 */
struct pattern_struct
{
	/*
	 * Letters in pattern - these are letters from gameboard, placed
	 * in earlier moves. EMPTY_FIELD means field, where computer can
	 * put its letter. BELOW_THE_GAME indicates end of pattern.
	 */
    char fields[MAX_WORD_LENGTH];

	/*
	 * Fast access to fields, where letters from gameboard are in pattern.
	 */
    char letters[MAX_WORD_LENGTH];

	/*
	 * Fast access to fields, where letter from gameboard aren't present in
	 * pattern.
	 */
	char notletters[MAX_WORD_LENGTH];
	
	/*
	 * Pattern start coordinates.
	 */
    char x, y;

	/*
	 * Pattern direction - HORIZONTAL or VERTICAL.
	 */
	char horizont;
	
	/*
	 * Pattern length - nuber of letters in fields to first BELOW_THE_GAME.
	 */
	char length;
	
	/*
	 * Number of letters from gameboard in pattern.
	 */
	char number_of_letters;
	
	/*
	 * Link to patterns linked with this pattern.
	 */
	
    struct link_struct *link;

	/*
	 * Link to words found by computer, satisfying this pattern.
	 */
    struct word_struct *word;

	/*
	 * Link to the next pattern in pattern group.
	 */
    struct pattern_struct *next;

	/*
	 * Link to the next same pattern. Two patterns are same if and only
	 * if there are equal in terms of function compare_pattern_fields()
	 * from Sakreble class object.
	 */
    struct pattern_struct *next_same;
};

/*
 * Struct for defining links between patterns.
 */
struct link_struct
{
	/*
	 * Pattern, which is linked to pattern containing this struct.
	 */
    struct pattern_struct *pattern;

	/*
	 * Next link. if pattern containg this struct is linked to
	 * other patterns.
	 */
    struct link_struct *next;
};

/*
 * Struct for word found by computer during searching
 * words on gameborad during computer's move.
 */
struct word_struct
{
	/*
	 * Letters in word.
	 */
    char *word;
	
	/*
	 * Blank positions in word.
	 */
    int *blanks;

	/*
	 * Number of blanks in word.
	 */
    int number_of_blanks;

	/*
	 * Word's point value. It contains total word value
	 * with word/letter bonus included (and others).
	 */
    int points;

	/*
	 * Link to the next word.
	 */
    struct word_struct *next;
};

#endif
