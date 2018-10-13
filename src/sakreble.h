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

#ifndef SAKREBLE_H
#define SAKREBLE_H

#include <qwidget.h>

#include "alphabet.h"

#define FOR_PLAYER_MOVE 0
#define FOR_COMPUTER_MOVE 1
#define FOR_PLAYERS_CHOOSING 2
#define FOR_GAME_BEGINNING 3
#define FOR_GAME_END 4
#define FOR_WORD_CONFIRMATION 5
#define FOR_BLANK_CHANGE 6

#define ILE_IDOW_W_MENU 10

#define GAME_NEW_GAME_ID 0
#define MOVE_OK_ID 1
#define MOVE_CHANGE_ID 2
#define MOVE_PASS_ID 3
#define GAME_PROGRAM_EXIT_ID 4
#define TOOLS_LETTERS_LEFT_PREVIEW_ID 5
#define GAME_START_ID 6
#define PLAYER_ADD_ID 7
#define PLAYER_ABOUT_PLAYERS_ID 8

class QPushButton;
class QLabel;
class QComboBox;
class QMenuBar;
class QPopupMenu;
class QString;

class Gameboard;
class Stand;
class Points;
class Letters;
class Highlighting;
class Lamp;
class ChangeBlank;
class LettersLeft;
class Players;
class AboutPlayers;
class ConfigParser;
class Rules;

struct pattern_struct;

class Sakreble : public QWidget
{
	Q_OBJECT

	public:
		Sakreble( QWidget *parent = 0 , const char *name = 0 );
		void setDictionaryPath( QString p );
		int isDictionaryPathSet() { return is_dictionary_path_set; }
		void setPlayerDataFile( QString p );
		int isPlayerDataFileSet() { return is_player_data_file_set; }
		void setConfigurationFilePath( QString p );
		int isConfigurationFilePathSet() { return is_configuration_file_path_set; }
		~Sakreble();

	public slots:
		void beginGame();
		void computerMove();
		void performOkButtonServiceActions( int );
		void performChangeButtonServiceActions( int );
		void playerPass();
		void playerChangesLetters();
		void choosePlayers();
		void playersWordConfirmed();
		void wordChecking();
		void playerClickedOk();
		void blankChanged( char );
		void lettersLeftPreviewServiceActions();
		void addPlayerNamesIntoCombos();
		void combo0ServiceActions( const QString & );
		void combo1ServiceActions( const QString & );
		void showAboutPlayers();
		void makeCombos();

	private:
		void sortWord( char * , int );
		void getRandomLetters( char * , int );
		void cleanPatterns();
		int countEmptyFields( char * );
		void add_pattern( int , int , char * , int );
		void add_link( struct pattern_struct * , struct pattern_struct * );
		void add_word( struct pattern_struct * , char * , int , int * , int );
		void initialize_patterns();
		void initialize_letters();
		void first_move();
		void produce_patterns_from_gameboard( char );
		void produce_links();
		int compare_pattern_fields( struct pattern_struct * , struct pattern_struct * );
		void enqueue_patterns();
		void read_dictionary();
		void remove_letter_from_word( char * , int , char );
		struct pattern_struct *choose_best_word();
		void apply_dictionary_with_patterns( char * , int );
		void place_word_on_gameboard( struct pattern_struct * );
		int isWordInDictionaryPresent( char * , int );
		int isWordFromGameboardInDictionaryPresent( int , int , int );
		void findLengths();
		int compareWords( char * , char * , int );
		void showWidgets( int );
		void playerSetsWord();
		void gameEnd();
		void place_added_letters_on_gameboard( struct pattern_struct * );
		void addPlayerNamesIntoCombo( int );
		void comboServiceActions( int , const QString & );

		QPushButton *quit, *start, *next, *ok, *wymien, *pas, *nextGame, *checkButton;
		QPushButton *confirmButton, *lettersLeftButton;
		QComboBox *playerCombo[2];
		QLabel *label, *lettersLeftLabel;
		QMenuBar *menu;
		QPopupMenu *menuGame, *menuMove, *menuTools, *menuPlayer, *menuOptions;
		Gameboard *gameboard;
		Stand *stand;
		Alphabet alphabet;
		Points *points[2], *this_move;
		Letters *playerLetters[2];
		Highlighting *highlighting;
		Lamp *lamp[2];
		ChangeBlank *changeBlank;
		LettersLeft *lettersLeft;
		Players *players;
		AboutPlayers *aboutPlayers;
		ConfigParser *configParser;
		Rules *rules;

		/*
		 * Pointers to the start and end of the dictionary im memory.
		 */
		char *dictionary, *dictionary_end;
		
		/*
		 * Patterns describing possible slots to place words on gameboard.
		 * pattern[x][y][k][h] indicates pattern beginning in (x,y) field,
		 * with k empty fields, and direction h (horizontal or vertical).
		 */
		struct pattern_struct *patterns[15][15][7][2];

		/*
		 * Patterns are groupped by theirs lengths. first_pattern[i] contains
		 * link to reach all patterns with length i. This link follows to the
		 * list of pattern lists. Each of these lists constains same patterns
		 * (same means thart function compare_pattern_fields() return true for
		 * compared patterns).
		 */
		struct pattern_struct *first_pattern[15];

		struct pattern_struct *pattern_with_best_word;
		int whichPlayer;	// Ktory player teraz ma sie ruszyc
		int whichToLettersLeftPreview; // Jak gra czlowiek z komputerem, to to nie bedzie sie zmieniac
		char *lenghts[15];	// Pointers do dictionary parts with word lenghts same with array index
		int number_of_pauses;
		int byl_kit;
		int menu_ids[ ILE_IDOW_W_MENU ];
		QString dictionary_path;
		int is_dictionary_path_set;
		QString player_data_file;
		int is_player_data_file_set;
		QString configuration_file_path;
		int is_configuration_file_path_set;
};

#endif
