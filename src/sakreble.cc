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
#include <stdlib.h>
#include <time.h>

#include <qapp.h>
#include <qpushbt.h>
#include <qlabel.h>
#include <qcombo.h>
#include <qframe.h>
#include <qwindefs.h>
#include <qmenubar.h>
#include <qpopmenu.h>

#include "sakreble.h"
#include "gameboard.h"
#include "stand.h"
#include "dupa.h"
#include "points.h"
#include "letters.h"
#include "highlighting.h"
#include "lamp.h"
#include "blank.h"
#include "checker.h"
#include "lettersleft.h"
#include "player.h"
#include "aboutplayers.h"
#include "configparser.h"
#include "rules.h"

Sakreble::Sakreble( QWidget *parent , const char *name ) : QWidget( parent, name )
{
	setMinimumSize( 600 , 540 );
	setMaximumSize( 600 , 540 );

	//    setFont( QFont( "Arial" , 12 , QFont::Normal ) );

	setCaption( "Sakreble!" );

	quit = new QPushButton( "Quit", this, "quit" );
	quit->setFont( QFont( "Times", 18, QFont::Bold ) );
	quit->move( 470 , 430 );

	start = new QPushButton( "Start", this, "start" );
	start->setFont( QFont( "Times", 18, QFont::Bold ) );
	start->move( 470 , 40 );

	nextGame = new QPushButton( tr( "Next game" ) , this, "nextgame" );
	nextGame->move( 470 , 40 );

	checkButton = new QPushButton( "Check", this, "sprawdz" );
	checkButton->move( 470 , 70 );

	confirmButton = new QPushButton( "Ok", this, "zatwierdz" );
	confirmButton->move( 470 , 40 );

	next = new QPushButton( "Next", this, "dalej" );
	next->setFont( QFont( "Times", 18, QFont::Bold ) );
	next->move( 470 , 80 );
	next->hide();

	ok = new QPushButton( "Ok", this, "ok" );
	ok->setGeometry( 470 , 40 , 100 , 28 );
	ok->hide();

	wymien = new QPushButton( "Change", this, "wymien" );
	wymien->setGeometry( 470 , 70 , 100 , 28 );
	wymien->hide();

	pas = new QPushButton( "Pass", this, "pas" );
	pas->setGeometry( 470 , 100 , 100 , 28 );
	pas->hide();

	lettersLeftButton = new QPushButton( "Letters left", this, "pas" );
	lettersLeftButton->setGeometry( 470 , 400 , 100 , 20 );
	lettersLeftButton->hide();

	label = new QLabel( "vs" , this , "label" );
	label->move( 512 , 115 );

	gameboard = new Gameboard( this , "gameboard" );
	gameboard->setGeometry( 10 , 40 , 450 , 450 );
	gameboard->setAlphabet( &alphabet );

	stand = new Stand( this , "stand" );
	stand->setGeometry( 10 , 500 , 450 , 30 );
	stand->setAlphabet( &alphabet );

	lettersLeftLabel = new QLabel( this , "polettersLeftLiter" );
	lettersLeftLabel->setFrameStyle( QFrame::Sunken | QFrame::Panel );
	lettersLeftLabel->setGeometry( 221 , 501 , 28 , 28 );
	lettersLeftLabel->setAlignment( AlignCenter );

	players = new Players( NULL , "players" );

	playerCombo[0] = new QComboBox( this , "pierwszecombo" );
	playerCombo[0]->setGeometry( 470 , 90 , 100 , 30 );

	playerCombo[1] = new QComboBox( this , "drugiecombo" );
	playerCombo[1]->setGeometry( 470 , 140 , 100 , 30 );


	points[0] = new Points( this , "punk0" );
	points[0]->move( 470 , 200 );
	points[0]->hide();
	points[1] = new Points( this , "punk1" );
	points[1]->move( 470 , 250 );
	points[1]->hide();

	this_move = new Points( this , "tenruch" );
	this_move->move( 470 , 320 );
	this_move->setName( "This move" );
	this_move->hide();

	playerLetters[0] = new Letters( this , "lit0" );
	playerLetters[0]->move( 570 , 200 );
	playerLetters[1] = new Letters( this , "lit1" );
	playerLetters[1]->move( 570 , 250 );

	highlighting = new Highlighting;
	highlighting->setStand( stand );
	highlighting->setGameboard( gameboard );

	changeBlank = new ChangeBlank( NULL , "changeblank" );
	changeBlank->setAlphabet( &alphabet );

	lettersLeft = new LettersLeft( NULL , "lettersLeft" );
	lettersLeft->setAlphabet( &alphabet );
	lettersLeft->setPlayerLetters( 0 , playerLetters[0] );
	lettersLeft->setPlayerLetters( 1 , playerLetters[1] );
	lettersLeft->setWhichPlayer( &whichToLettersLeftPreview );

	lamp[0] = new Lamp( this , "lamp0" );
	lamp[0]->setGeometry( 570 , 220 , 20 , 20 );
	lamp[1] = new Lamp( this , "lamp0" );
	lamp[1]->setGeometry( 570 , 270 , 20 , 20 );

	aboutPlayers = new AboutPlayers( NULL , "o playerach" );
	aboutPlayers->setPlayers( players );

	menuGame = new QPopupMenu();
	menu_ids[ GAME_NEW_GAME_ID ] = menuGame->insertItem( "&New" , this , SLOT( choosePlayers() ) );
	menu_ids[ GAME_START_ID ] = menuGame->insertItem( "&Start" , this , SLOT( beginGame() ) );
	menuGame->insertSeparator();
	menu_ids[ GAME_PROGRAM_EXIT_ID ] = menuGame->insertItem( "&Quit" , qApp , SLOT( quit() ) );

	menuMove = new QPopupMenu();
	menu_ids[ MOVE_OK_ID ] = menuMove->insertItem( "&Ok" , this , SLOT( playerClickedOk() ) );
	menu_ids[ MOVE_CHANGE_ID ] = menuMove->insertItem( "&Change" , this , SLOT( playerChangesLetters() ) );
	menu_ids[ MOVE_PASS_ID ] = menuMove->insertItem( "&Pass" , this , SLOT( playerPass() ) );

	menuTools = new QPopupMenu();
	menu_ids[ TOOLS_LETTERS_LEFT_PREVIEW_ID ] = menuTools->insertItem( tr( "&Letters left" ) , this , SLOT( lettersLeftPreviewServiceActions() ) );
	menuTools->setCheckable( true );

	menuPlayer = new QPopupMenu();
	menu_ids[ PLAYER_ADD_ID ] = menuPlayer->insertItem( "&Add..." , players , SLOT( addPlayer() ) );
	menuPlayer->insertSeparator();
	menu_ids[ PLAYER_ABOUT_PLAYERS_ID ] = menuPlayer->insertItem( "&Players..." , this , SLOT( showAboutPlayers() ) );

	menuOptions = new QPopupMenu();

	menu = new QMenuBar( this );
	menu->insertItem( "&Game" , menuGame );
	menu->insertItem( "&Move" , menuMove );
	menu->insertItem( "&Player" , menuPlayer );
	menu->insertItem( QString( tr( "&Tools" ) ) , menuTools );
	menu->insertItem( QString( tr( "&Options" ) ) , menuOptions );

	connect( quit , SIGNAL( clicked() ) , qApp , SLOT( quit() ) );
	connect( start , SIGNAL( clicked() ) , SLOT( beginGame() ) );
	connect( next , SIGNAL( clicked() ) , SLOT( computerMove() ) );
	connect( stand , SIGNAL( forChangeClicked( int ) ) , highlighting , SLOT( forChangeClicked( int ) ) );
	connect( stand , SIGNAL( standClicked( int ) ) , highlighting , SLOT( standClicked( int ) ) );
	connect( gameboard , SIGNAL( gameboardClicked( int , int ) ) , highlighting , SLOT( gameboardClicked( int , int ) ) );
	connect( gameboard , SIGNAL( enableOkButton( int ) ) , SLOT( performOkButtonServiceActions( int ) ) );
	connect( stand , SIGNAL( forChangeClickPossible( int ) ) , SLOT( performChangeButtonServiceActions( int ) ) );
	connect( pas , SIGNAL( clicked() ) , SLOT( playerPass() ) );
	connect( wymien , SIGNAL( clicked() ) , SLOT( playerChangesLetters() ) );
	connect( ok , SIGNAL( clicked() ) , SLOT( playerClickedOk() ) );
	connect( nextGame , SIGNAL( clicked() ) , SLOT( choosePlayers() ) );
	connect( confirmButton , SIGNAL( clicked() ) , SLOT( playersWordConfirmed() ) );
	connect( checkButton , SIGNAL( clicked() ) , SLOT( wordChecking() ) );
	connect( changeBlank , SIGNAL( changeBlankClicked( char ) ) , SLOT( blankChanged( char ) ) );
	connect( lettersLeft , SIGNAL( closeClicked() ) , SLOT( lettersLeftPreviewServiceActions() ) );
	connect( lettersLeftButton , SIGNAL( clicked() ) , SLOT( lettersLeftPreviewServiceActions() ) );
	connect( players , SIGNAL( playerAdded() ) , SLOT( makeCombos() ) );
	connect( playerCombo[0] , SIGNAL( activated( const QString & ) ) , SLOT( combo0ServiceActions( const QString & ) ) );
	connect( playerCombo[1] , SIGNAL( activated( const QString & ) ) , SLOT( combo1ServiceActions( const QString & ) ) );

	showWidgets( FOR_PLAYERS_CHOOSING );


	is_dictionary_path_set = false;
	is_player_data_file_set = false;
	is_configuration_file_path_set = false;

	rules = new Rules;
	
	configParser = new ConfigParser;
	configParser->setAlphabet( &alphabet );
	configParser->setGameboard( gameboard );
	configParser->setRules( rules );

	gameboard->setRules( rules );
}

Sakreble::~Sakreble()
{
#ifdef DEBUG
	printf( "Freeing memory...\n");
#endif
	
	players->writeToFile( player_data_file );

	free( dictionary );

	delete playerLetters[0];
	delete playerLetters[1];

	cleanPatterns();

	delete configParser;
	delete rules;

#ifdef DEBUG
	printf( "Memory fried...\n" );
#endif
}

/*
 * Function sorts word letters in order given in game config file.
 *
 * I used InsertionSort because it is fast for short arrays (here - up to
 * seven elements).
 */
void Sakreble::sortWord( char *word , int size )
{
	int i, j;
	char pom;
	for( i=1; i<size; i++ )
	{
		j = i;
		while( j > 0 && (unsigned char)word[j] < (unsigned char)word[j-1] )
		{
			pom = word[j-1];
			word[j-1] = word[j];
			word[j] = pom;
			j--;
		}
	}
}

/*
 * Gets random letters from sack for player's stand.
 */
void Sakreble::getRandomLetters( char *wylosowane , int ile )
{
	int i, j, letterNumber;

	for( i=0; i<ile; i++ )
	{
		if( wylosowane[i] != EMPTY_FIELD )
			continue;

		if( alphabet.getLettersLeft() == 0 )
			break;

		/*
		 * Get random letter number. This is number of letter in sack,
		 * for example: sack[a(1),b(2),c(0),d(3),e(1)] has letter numbers:
		 * a: 1; b: 2,3; d: 4,5,6; e: 7.
		 */
		letterNumber = ( random() % alphabet.getLettersLeft() ) + 1;

		/*
		 * Get letter from sack.
		 */

		/*
		 * First - substract number of letters left in game in sack.
		 */
		alphabet.substractLettersLeft();

		/*
		 * Find letter with given letter number.
		 */
		j = 0;
		while( letterNumber > 0 )
		{
			/*
			 * There are some letters 'j' in sack?
			 */
			if( alphabet.letter(j) > 0 )
			{
				/*
				 * If number of 'j's in sack is greater or equal
				 * letterNumber, we found letter. Otherwise go to
				 * the next letter.
				 */
				if( alphabet.letter(j) >= letterNumber )
				{
					alphabet.substractLetter(j);
					wylosowane[i] = j;

					letterNumber = 0;
				}
				else
					letterNumber -= alphabet.letter(j++);
			}
			else
			{
				j++;

				/*
				 * Letter number 10 is reserved for word separator.
				 */
				if( j == 10 )
					j++;
			}
		}
	}

	/*
	 * Update info about letters left in game.
	 */
	lettersLeftLabel->setNum( alphabet.getLettersLeft() );

	/*
	 * Update players letter infos.
	 */
	playerLetters[ whichPlayer ]->updateOnPicture();
	playerLetters[ 1-whichPlayer ]->updateOnPicture();

	/*
	 * Repaint letters left window, if it is shown.
	 */
	if( lettersLeft->isVisible() )
		lettersLeft->repaint();
}

void Sakreble::beginGame()
{
	players->setPlayingPlayers( 0 , playerCombo[0]->currentText() );
	players->setPlayingPlayers( 1 , playerCombo[1]->currentText() );

	showWidgets( FOR_GAME_BEGINNING );

	alphabet.initializeLetters();
	initialize_patterns();
	gameboard->clearGameboard();

	whichPlayer = 0;

	number_of_pauses = 0;
	byl_kit = 0;

	points[0]->zeroPoints();
	points[0]->setName( players->getPlayingPlayer(0)->getName() );
	points[1]->zeroPoints();
	points[1]->setName( players->getPlayingPlayer(1)->getName() );

	lamp[0]->turnOn();
	lamp[1]->turnOff();

	srandom( time( NULL ) );

	playerLetters[0]->removeLetters();
	playerLetters[1]->removeLetters();

	getRandomLetters( playerLetters[0]->getLetters() , 7 );
	getRandomLetters( playerLetters[1]->getLetters() , 7 );

	if( players->getPlayingPlayer(0)->realPlayer() || players->getPlayingPlayer(1)->virtualPlayer() )
		stand->setLettersOnStand( playerLetters[0]->getLetters() , 7 );
	else
		stand->setLettersOnStand( playerLetters[1]->getLetters() , 7 );

	if( players->getPlayingPlayer(0)->virtualPlayer() )
	{
		if( players->getPlayingPlayer(1)->realPlayer() )
			whichToLettersLeftPreview = 1;

		showWidgets( FOR_COMPUTER_MOVE );
		computerMove();
	}
	else
	{
		whichToLettersLeftPreview = 0;

		showWidgets( FOR_PLAYER_MOVE );
	}
}

/*
 * Computer's move.
 */
void Sakreble::computerMove()
{
	/*
	 * Sort computer letters for better and faster search.
	 */
	sortWord( playerLetters[ whichPlayer ]->getLetters() , 7 );

	/*
	 * If computer has less than 7 letters, move empty fields
	 * to the computer stand's end.
	 */
	playerLetters[whichPlayer]->moveEmptyFieldsToEnd();

	/*
	 * Check if this move is first move for game.
	 */
	if( gameboard->thereAreLetters() )
		/*
		 * Normal move, not first. Produce patterns from
		 * gameboard.
		 */
		produce_patterns_from_gameboard(7);
	else
		/*
		 * First move in game - create patterns according to
		 * 'first-move-field' rule.
		 */
		first_move();

	/*
	 * Enqueue patterns.
	 */
	enqueue_patterns();

	/*
	 * Produce links for patterns.
	 */
	produce_links();

	/*
	 * Find possible words for created patterns.
	 */
	apply_dictionary_with_patterns( playerLetters[ whichPlayer ]->getLetters() , playerLetters[whichPlayer]->numberOfLetters() );

	/*
	 * Find pattern with the best word.
	 */
	pattern_with_best_word = choose_best_word();

	/*
	 * Place founded word on gameboard.
	 */
	place_added_letters_on_gameboard( pattern_with_best_word );

	/*
	 * Change window style for word confirmation.
	 */
	showWidgets( FOR_WORD_CONFIRMATION );
}

void Sakreble::playersWordConfirmed()
{
	EndGameConditionsRule *rule = (EndGameConditionsRule *)(rules->getRule( RULE_END_GAME_CODITIONS ));

	if( players->getPlayingPlayer(whichPlayer)->virtualPlayer() )
	{
		gameboard->removeAddedLetters();

		place_word_on_gameboard( pattern_with_best_word );

		getRandomLetters( playerLetters[ whichPlayer ]->getLetters() , 7 );

		cleanPatterns();

		/*
		 * Tu mozna poprawic, poniewaz za bardzo nie widac, jak te pauzy
		 * sie zmieniaja
		 */
		if( pattern_with_best_word != NULL )
			number_of_pauses = 0;
		else
		{
			/*
			 * Check if continuous pauses may end the game.
			 */
			if( rule->numberOfContinuousPausesToEnd() > 0 )
				/*
				 * Check if limit of continuous pauses was reached.
				 */
				if( number_of_pauses == rule->numberOfContinuousPausesToEnd() )
					return;
		}


		byl_kit = 0;
	}
	else
	{
		number_of_pauses = 0;
		byl_kit = 0;

		points[whichPlayer]->addPoints( gameboard->getPointValue() );
		gameboard->removeAddingIndicatorFromAddedLetters();

		playerLetters[whichPlayer]->changeLetters( stand->getLettersOnStand() , 7 );
		getRandomLetters( playerLetters[whichPlayer]->getLetters() , 7 );

		lettersLeftLabel->setNum( alphabet.getLettersLeft() );
	}

	if( playerLetters[whichPlayer]->numberOfLetters() > 0 )
	{
		whichPlayer = 1 - whichPlayer;

		if( players->getPlayingPlayer(whichPlayer)->realPlayer() )
			whichToLettersLeftPreview = whichPlayer;

		lamp[whichPlayer]->turnOn();
		lamp[1-whichPlayer]->turnOff();

		if( players->getPlayingPlayer(whichPlayer)->virtualPlayer() )
		{
			stand->setLettersOnStand( playerLetters[1-whichPlayer]->getLetters() , 7 );
			showWidgets( FOR_COMPUTER_MOVE );
			computerMove();
		}
		else
		{
			showWidgets( FOR_PLAYER_MOVE );
			stand->setLettersOnStand( playerLetters[whichPlayer]->getLetters() , 7 );
		}

		if( lettersLeft->isVisible() )
			lettersLeft->repaint();
	}
	else
		gameEnd();
}

void Sakreble::wordChecking()
{
	if( players->getPlayingPlayer(whichPlayer)->virtualPlayer() )
		playersWordConfirmed();
	else
	{
		if( byl_kit )
		{
			byl_kit = 0;
			playerPass();
		}
		else
			playersWordConfirmed();
	}
}

/*
 * Clean up patterns - free all memory occuped by them.
 * After calling this function all patterns should pointed to NULL.
 */
void Sakreble::cleanPatterns()
{
	int i, j, k, l;
	struct link_struct *link_tmp = NULL, *link_prev = NULL;
	struct word_struct *word_tmp = NULL, *word_prev = NULL;

	for( i=0; i<15; i++ )
		for( j=0; j<15; j++ )
			for( k=0; k<7; k++ )
				for( l=0; l<2; l++ )
				{
					if( patterns[i][j][k][l] != NULL )
					{
						link_tmp = patterns[i][j][k][l]->link;
						while( link_tmp != NULL )
						{
							link_prev = link_tmp;
							link_tmp = link_tmp->next;
							free( link_prev );
						}

						word_tmp = patterns[i][j][k][l]->word;
						while( word_tmp != NULL )
						{
							word_prev = word_tmp;
							word_tmp = word_tmp->next;

							if( word_prev->blanks )
								delete word_prev->blanks;

							free( word_prev );
						}

						free( patterns[i][j][k][l] );
						patterns[i][j][k][l] = NULL;
					}
				}
}

/*
 * Counts empty fields.
 */
int Sakreble::countEmptyFields( char fields[15] )
{
	int i, ile = 0;

	for( i=0; i<15; i++ )
		if( fields[i] == EMPTY_FIELD )
			ile++;

	return ile;
}

/*
 * Adds pattern to pattern set.
 */
void Sakreble::add_pattern( int x , int y , char fields[15] , int horizont )
{
	struct pattern_struct *pattern_tmp;
	int i, j;

	/*
	 * Wzorzec mogl juz byc dodany do przywiazania.
	 */
	if( patterns[x][y][countEmptyFields(fields)-1][horizont] == NULL )
	{
		if( ( pattern_tmp = (struct pattern_struct *) malloc( sizeof ( struct pattern_struct ) ) ) == NULL )
			fatal( "malloc, add_pattern" );
	}
	else
		pattern_tmp = patterns[x][y][countEmptyFields(fields)-1][horizont];

	pattern_tmp->length = 0;

	for( i=0; i<15; i++ )
	{
		pattern_tmp->fields[i] = fields[i];
		pattern_tmp->letters[i] = BELOW_THE_GAME;
		if( fields[i] != BELOW_THE_GAME )
			pattern_tmp->length++;
	}

	j = 0;
	for( i=0; i<pattern_tmp->length; i++ )
		if( fields[i] != EMPTY_FIELD )
			pattern_tmp->letters[j++] = i;

	pattern_tmp->number_of_letters = j;

	j = 0;
	for( i=0; i<pattern_tmp->length; i++ )
		if( fields[i] == EMPTY_FIELD )
			pattern_tmp->notletters[j++] = i;

	pattern_tmp->word = NULL;
	pattern_tmp->link = NULL;
	pattern_tmp->next = NULL;
	pattern_tmp->next_same = NULL;

	pattern_tmp->x = x;
	pattern_tmp->y = y;
	pattern_tmp->horizont = horizont;

	patterns[x][y][countEmptyFields( fields )-1][horizont] = pattern_tmp;
}

/*
 * Creates link between patterns.
 */
void Sakreble::add_link( struct pattern_struct *pattern, struct pattern_struct *linked )
{
	struct link_struct *link_tmp;

	if( pattern == NULL )
		fatal( "Co¶ siê pochrzani³o, add_link, pattern" );
	if( linked == NULL )
		fatal( "Co¶ siê pochrzani³o, add_link, dowi±zywany" );

	if( ( link_tmp = (struct link_struct *) malloc( sizeof ( struct link_struct ) ) ) == NULL )
		fatal( "malloc, add_link" );

	link_tmp->pattern = linked;
	link_tmp->next = pattern->link;
	pattern->link = link_tmp;;
}

/*
 * Adds word found by computer to the pattern it satisfies.
 */
void Sakreble::add_word( struct pattern_struct *pattern, char *word, int points, int *where_blanks, int number_of_blanks )
{
	struct word_struct *word_tmp;

	/*
	 * Allocate memory for new word.
	 */
	if( ( word_tmp = (struct word_struct *) malloc( sizeof ( struct word_struct ) ) ) == NULL )
		fatal( "malloc, add_word" );

	/*
	 * Set letters and point value.
	 */
	word_tmp->word = word;
	word_tmp->points = points;

	/*
	 * Set blanks info.
	 */
	if( number_of_blanks == 0 )
		/*
		 * No blanks in word - set NULL. It must be NULL,
		 * because some instructions check word_struct
		 * against NULL in blanks field.
		 */
		word_tmp->blanks = NULL;
	else
	{
		/*
		 * Create blanks info.
		 */
		word_tmp->blanks = new int[ number_of_blanks ];

		for( int i=0; i<number_of_blanks; i++ )
			word_tmp->blanks[i] = where_blanks[i];
	}

	/*
	 * Set number of blanks.
	 */
	word_tmp->number_of_blanks = number_of_blanks;

	/*
	 * Add word to the beginning of pattern's word list.
	 */
	word_tmp->next = pattern->word;
	pattern->word = word_tmp;
}

/*
 * Initializes patterns. Possible patterns are: seven patterns for
 * each gameboard field, for horizontal and vertical direction.
 * So - 3150 patterns for 15x15 gameboard.
 */
void Sakreble::initialize_patterns()
{
	int i, j, k, l;

	for( i=0; i<15; i++ )
		for( j=0; j<15; j++ )
			for( k=0; k<7; k++ )
				for( l=0; l<2; l++ )
					patterns[i][j][k][l] = NULL;
}

/*
 * Produces patterns for first game move. They depend on 'first-move-field'
 * game rule.
 */
void Sakreble::first_move()
{
	char fields[15];
	
	fields[0] = EMPTY_FIELD;

	if( rules->getRule( RULE_START_FIELD )->isActive() )
	{
		FirstMoveRule *rule = (FirstMoveRule *)(rules->getRule( RULE_START_FIELD ));

		/*
		 * First - horizontal patterns.
		 */		
		for( int i=rule->getFirstMoveXPos()-6; i<=rule->getFirstMoveXPos()+6; i++ )
		{
			if( gameboard->isDisabledField( i , rule->getFirstMoveYPos() ) )
				continue;

			/*
			 * If first move field isn't flexible then allow only patterns
			 * beginning exactly in that field.
			 */
			if( !rule->isFlexible() && i != rule->getFirstMoveXPos() )
				continue;

			for( int k=1; k<15;  k++ )
				fields[k] = BELOW_THE_GAME;
			
			for( int k=i+1; k<15 && !gameboard->isDisabledField( k , rule->getFirstMoveYPos() ); k++ )
			{
				fields[k-i] = EMPTY_FIELD;
				
				/*
				 * Pattern must touch start field.
				 */
				if( k >= rule->getFirstMoveXPos() )
					add_pattern( i , rule->getFirstMoveYPos() , fields , HORIZONTAL );
			}
		}
		
		/*
		 * Vertical patterns.
		 */		
		for( int j=rule->getFirstMoveYPos()-6; j<=rule->getFirstMoveYPos()+6; j++ )
		{
			if( gameboard->isDisabledField( rule->getFirstMoveXPos() , j ) )
				continue;

			/*
			 * If first move field isn't flexible then allow only patterns
			 * beginning exactly in that field.
			 */
			if( !rule->isFlexible() && j != rule->getFirstMoveYPos() )
				continue;

			for( int k=1; k<15;  k++ )
				fields[k] = BELOW_THE_GAME;
			
			for( int k=j+1; k<15 && !gameboard->isDisabledField( rule->getFirstMoveXPos() , k ); k++ )
			{
				fields[k-j] = EMPTY_FIELD;
				
				/*
				 * Pattern must touch start field.
				 */
				if( k >= rule->getFirstMoveYPos() )
					add_pattern( rule->getFirstMoveXPos() , j , fields , VERTICAL );
			}
		}
	}
	else
	{
		/*
		 * If rule for first move wasn't set, player
		 * can place his first word everywhere.
		 */
		for( int i=0; i<14; i++ )
			for( int j=0; j<14; j++ )
				if( !gameboard->isDisabledField( i , j ) )
				{
					/*
					 * Horizontal patterns first.
					 */
					for( int k=1; k<15;  k++ )
						fields[k] = BELOW_THE_GAME;

					for( int k=i+1; k<15 && !gameboard->isDisabledField( k , j ); k++ )
					{
						fields[k-i] = EMPTY_FIELD;
						
						add_pattern( i , j , fields , HORIZONTAL );
					}
					
					/*
					 * Vertical patterns.
					 */
					for( int k=1; k<15;  k++ )
						fields[k] = BELOW_THE_GAME;

					for( int k=j+1; k<15 && !gameboard->isDisabledField( i , k ); k++ )
					{
						fields[k-j] = EMPTY_FIELD;
						
						add_pattern( i , j , fields , VERTICAL );
					}
				}
	}
}

/*
 * Function produces patterns from current game state.
 */
void Sakreble::produce_patterns_from_gameboard( char max_number_of_letters )
{
	int i, j, kratka, liter, k;
	char fields[15];
	int empty_field_found, letter_found;

	for( j=0; j<15; j++ )
		for( i=0; i<15; i++ )
		{
			/*
			 * Here horizontal patterns are searched.
			 */
			
			/*
			 * Horizontal pattern should begin in left edge of gameboard
			 * or field lefts to pattern's first field should be empty or disabled.
			 */
			if( gameboard->isPatternStartField( i , j , HORIZONTAL ) )
			{
				kratka = 0;
				liter = max_number_of_letters;
				empty_field_found = 0;
				letter_found = 0;

				for( k=0; k<15; k++ )
					fields[k] = BELOW_THE_GAME;

				while( liter > 0 && kratka + i < 15 )
				{
					fields[kratka] = gameboard->letter( kratka+i , j );

					if( fields[kratka] == EMPTY_FIELD )
					{
						/*
						 * Here we check for possible 'dolepkas'.
						 */
						if( j > 0 && gameboard->isLetterField( kratka+i , j-1 ) ||
								j < 14 && gameboard->isLetterField( kratka+i , j+1 ) )
							letter_found = 1;

						empty_field_found = 1;
						liter--;
					}
					else
						letter_found = 1;

					if( kratka > 0 )	// >=2-letter words are valid
						if( empty_field_found && letter_found )
							if( gameboard->isPatternEndField( kratka+i , j , HORIZONTAL ) )
								add_pattern( i , j , fields , HORIZONTAL );

					kratka++;
				}
			}

			/*
			 * Here vertical patterns are searched.
			 */
			
			/*
			 * Vertical pattern should begin in top edge of gameboard
			 * or field upper to pattern's first field should be empty or disabled.
			 */
			if( gameboard->isPatternStartField( i , j , VERTICAL ) )
			{
				kratka = 0;
				liter = max_number_of_letters;
				empty_field_found = 0;
				letter_found = 0;

				for( k=0; k<15; k++ )
					fields[k] = BELOW_THE_GAME;

				while( liter > 0 && kratka + j < 15 )
				{
					fields[kratka] = gameboard->letter( i , kratka+j );

					if( fields[kratka] == EMPTY_FIELD )
					{
						/*
						 * Here we check for possible 'dolepkas'.
						 */
						if( i > 0 && gameboard->isLetterField( i-1 , kratka+j ) ||
								i < 14 && gameboard->isLetterField( i+1 , kratka+j ) )
							letter_found = 1;

						empty_field_found = 1;
						liter--;
					}
					else
						letter_found = 1;

					if( kratka > 0 )	// >=2-letter words are valid
						if( empty_field_found && letter_found )
							if( gameboard->isPatternEndField( i , kratka+j , VERTICAL ) )
								add_pattern( i , j , fields , VERTICAL );

					kratka++;
				}
			}
		}
}

void Sakreble::produce_links()
{
	struct pattern_struct *pattern_tmp, *pattern_group;
	int j, direction_up, direction_aside;

	for( j=0; j<15; j++ )
		for( pattern_group = first_pattern[j]; pattern_group != NULL; pattern_group = pattern_group->next )
			for( pattern_tmp = pattern_group; pattern_tmp != NULL; pattern_tmp = pattern_tmp->next_same )
			{
				for( int i=0; i<pattern_tmp->length; i++ )
				{
					if( pattern_tmp->horizont == HORIZONTAL )
					{
						if( gameboard->isEmptyField( pattern_tmp->x+i , (int)pattern_tmp->y ) )
						{
							direction_up = 1;

							while( pattern_tmp->y - direction_up >= 0 && gameboard->isLetterField( pattern_tmp->x+i , pattern_tmp->y-direction_up ) )
								direction_up++;

							if( pattern_tmp->y > 0 && gameboard->isLetterField( pattern_tmp->x+i , pattern_tmp->y-1 ) )
								add_link( pattern_tmp , patterns[pattern_tmp->x+i][pattern_tmp->y-direction_up+1][0][VERTICAL] );
							else
								if( pattern_tmp->y < 14 && gameboard->isLetterField( pattern_tmp->x+i , pattern_tmp->y+1 ) )
									add_link( pattern_tmp , patterns[pattern_tmp->x+i][(int)pattern_tmp->y][0][VERTICAL] );
						}
					}
					else
					{
						if( gameboard->isEmptyField( (int)pattern_tmp->x , pattern_tmp->y+i ) )
						{
							direction_aside = 1;

							while( pattern_tmp->x - direction_aside >= 0 && gameboard->isLetterField( pattern_tmp->x-direction_aside , pattern_tmp->y+i ) )
								direction_aside++;

							if( pattern_tmp->x > 0 && gameboard->isLetterField( pattern_tmp->x-1 , pattern_tmp->y+i ) )
								add_link( pattern_tmp , patterns[pattern_tmp->x-direction_aside+1][pattern_tmp->y+i][0][HORIZONTAL] );
							else
								if( pattern_tmp->x < 14 && gameboard->isLetterField( pattern_tmp->x+1 , pattern_tmp->y+i ) )
									add_link( pattern_tmp , patterns[(int)pattern_tmp->x][pattern_tmp->y+i][0][HORIZONTAL] );
						}
					}
				}
			}
}

int Sakreble::compare_pattern_fields( struct pattern_struct *w1 , struct pattern_struct *w2 )
{
	int i;

	if( w1->length != w2->length )
		return 0;

	for( i=0; i<w1->length; i++ )
		if( w1->fields[i] != w2->fields[i] )
			return 0;

	return 1;
}

/*
 * Enqueue patterns in first_pattern array.
 */
void Sakreble::enqueue_patterns()
{
	int i, j, k, l;
	struct pattern_struct *pattern;

	/*
	 * Clear first_pattern array. I belive, that in this point all
	 * memory is fried and links in first_pattern array aren't valid.
	 */
	for( i=0; i<15; i++ )
		first_pattern[i] = NULL;

	/*
	 * Enqueue all patterns.
	 */
	for( i=0; i<15; i++ )
		for( j=0; j<15; j++ )
			for( k=0; k<7; k++ )
				for( l=0; l<2; l++ )
					if( patterns[i][j][k][l] != NULL )
					{
						/*
						 * Get first pattern from list of patterns with length
						 * equal to length of processed pattern.
						 */
						pattern = first_pattern[patterns[i][j][k][l]->length - 1];

						if( pattern == NULL )
							/*
							 * There are no patterns with length equal to length
							 * of processed pattern. In this case just link array
							 * first_pattern with our length to processed pattern.
							 */
							first_pattern[patterns[i][j][k][l]->length - 1] = patterns[i][j][k][l];
						else		
						{	   
							/*
							 * Find patterns same to processed pattern (same
							 * means, that both pattern are equal in terms of
							 * function compare_pattern_fields().
							 */
							while( pattern != NULL )
							{
								/*
								 * Check if processed pattern list contains
								 * patterns same to processed pattern.
								 */
								if( compare_pattern_fields( pattern , patterns[i][j][k][l] ) )
								{
									/*
									 * Pattern list contains same patterns to
									 * the processed pattern. Add this pattern
									 * to the list beginning.
									 */
									patterns[i][j][k][l]->next_same = pattern->next_same;
									pattern->next_same = patterns[i][j][k][l];
									pattern = NULL;
								}
								else
								{
									/*
									 * Pattern list doesn't contains same
									 * patterns to the processed pattern.
									 */
									if( pattern->next == NULL )
									{
										/*
										 * There is no more pattern lists.
										 * Create new list containg processed
										 * pattern.
										 */
										pattern->next = patterns[i][j][k][l];
										pattern = NULL;
									}
									else
										/*
										 * Go to the next pattern list.
										 */
										pattern = pattern->next;
								}
							}
						}
					}
}

/*
 * Sets dictionary path. This function is called by main()
 * function during parsing command line parameters.
 */
void Sakreble::setDictionaryPath( QString p )
{
	dictionary_path = p;

	is_dictionary_path_set = true;

	/*
	 * Dictionary can be read only after configuration file setting.
	 */
	if( is_configuration_file_path_set )
		read_dictionary();
}

/*
 * Reads dictionary from file.
 */
void Sakreble::read_dictionary()
{
	FILE *file;
	size_t size;

	if( ( file = fopen( dictionary_path , "r" ) ) == NULL )
		fatal( "fopen, read_dictionary" );

	if( fseek( file , 0 , SEEK_END ) == -1 )
		fatal( "fseek, read_dictionary" );

	if( ( size = ftell( file ) ) == (unsigned long)-1 )
		fatal( "ftell, read_dictionary" );

	if( fseek( file , 0 , SEEK_SET ) == -1 )
		fatal( "fseek, read_dictionary" );

	if( ( dictionary = (char *) malloc( sizeof( char ) * size ) ) == NULL )
		fatal( "malloc, read_dictionary" );

	dictionary_end = dictionary + size;

	if( fread( dictionary , 1 , size , file ) != size )
		fatal( "fread, read_dictionary" );

	if( fclose( file ) != 0 )
		fatal( "fread, read_dictionary" );

#ifdef DEBUG
	printf( "S³ownik wczytany...\n" );
#endif

	findLengths();

	for( int i=0; i<15; i++ )
		first_pattern[i] = NULL;

	pattern_with_best_word = NULL;

	initialize_patterns();
}

void Sakreble::remove_letter_from_word( char *word , int size , char letter )
{
	int l, m, r, i;

	l = 0;
	r = size - 1;
	m = ( l + r ) / 2;

	while( word[m] != letter )
	{
		if( word[m] < letter )
			l = m + 1;
		else
			r = m - 1;

		m = ( l + r ) / 2;
	}

	for( i=m+1; i<size; i++ )
		word[i-1] = word[i];
}

struct pattern_struct *Sakreble::choose_best_word()
{
	struct pattern_struct *pattern_tmp, *the_best, *pattern_group;
	struct word_struct *word_tmp, *previous, *word_link;
	struct link_struct *link_tmp;
	int max_number_of_points = -1, sum_of_points = 0;
	int obciach, found;
	int j;

	the_best = NULL;

	for( j=0; j<15; j++ )
		for( pattern_group = first_pattern[j]; pattern_group != NULL; pattern_group = pattern_group->next )
			for( pattern_tmp = pattern_group; pattern_tmp != NULL; pattern_tmp = pattern_tmp->next_same )
			{
				word_tmp = pattern_tmp->word;
				previous = NULL;

				while( word_tmp != NULL )
				{
					obciach = 0;
					sum_of_points = word_tmp->points;

					/*
					 * Sprawdzamy powiazania.
					 */
					for( link_tmp = pattern_tmp->link; link_tmp != NULL; link_tmp = link_tmp->next )
					{
						found = 0;

						for( word_link = link_tmp->pattern->word; word_link != NULL; word_link = word_link->next )
						{
							if( pattern_tmp->horizont == HORIZONTAL )
							{
								if( word_tmp->word[link_tmp->pattern->x - pattern_tmp->x] == word_link->word[pattern_tmp->y - link_tmp->pattern->y] )
								{
									sum_of_points += word_link->points;
									found = 1;
								}
							}
							else
							{
								if( word_tmp->word[link_tmp->pattern->y - pattern_tmp->y] == word_link->word[pattern_tmp->x - link_tmp->pattern->x] )
								{
									sum_of_points += word_link->points;
									found = 1;
								}
							}
						}

						if( !found )
							obciach = 1;
					}

					if( !obciach && sum_of_points > max_number_of_points )
					{
						/*
						 * If computer decided to use a blank, check if this
						 * decision makes sense. Blank using profits only if
						 * this move piont value is euqal or higher than
						 * blank profitability setted for this computer player.
						 * Additionaly - if there's no more letters in the sack
						 * blank is always placed.
						 */
						if( word_tmp->blanks && ( players->getPlayingPlayer( whichPlayer )->getBlakProfitability() > sum_of_points ) && alphabet.getLettersLeft() > 0 )
						{
							previous = word_tmp;
							word_tmp = word_tmp->next;
						}
						else
						{
							max_number_of_points = sum_of_points;
							if( previous != NULL )
							{
								previous->next = word_tmp->next;
								word_tmp->next = pattern_tmp->word;
								pattern_tmp->word = word_tmp;
								word_tmp = previous->next;
							}
							else
							{
								previous = word_tmp;
								word_tmp = word_tmp->next;
							}
							the_best = pattern_tmp;
						}
					}
					else
					{
						previous = word_tmp;
						word_tmp = word_tmp->next;
					}
				}
			}

	if( max_number_of_points >= 0 )
		the_best->word->points = max_number_of_points;

	return the_best;
}

void Sakreble::apply_dictionary_with_patterns( char *letters , int number_of_letters )
{
	char *word = dictionary, *beginning = dictionary;
	int word_length = 0, i;
	struct pattern_struct *pattern = NULL, *the_same;
	int aktx, akty, number_of_used_letters, points, word_multiplikator, word_pasuje;
	int number_of_blanks = 0, blanks_left;
	Checker checker;

	int min_length = players->getPlayingPlayer( whichPlayer )->getMinWordLength();
	int max_length = players->getPlayingPlayer( whichPlayer )->getMaxWordLength();

	for( i=0; i<number_of_letters; i++ )
		if( letters[i] == BLANK )
			number_of_blanks++;

	letters += number_of_blanks;
	number_of_letters -= number_of_blanks;

	int where_blank[ number_of_blanks ];
	char blank_memory[ number_of_blanks ];

	checker.setCheckedLetters( letters , number_of_letters );

	while( word < dictionary_end )
	{
		if( *word == 10 )
		{
			if( word_length < min_length || word_length > max_length )
			{
				word++;
				continue;
			}

			for( pattern = first_pattern[word_length-1]; pattern != NULL; pattern = pattern->next )
			{
				word_pasuje = 1;

				/*
				 * Najpierw sprawdzam, czy we patterncu sa jakies letters i czy w slowie tez one sa
				 */
				for( i=0; i<pattern->number_of_letters; i++ )
				{
					if( pattern->fields[(int)pattern->letters[i]] != beginning[(int)pattern->letters[i]] )
					{
						word_pasuje = 0;
						break;
					}
				}

				if( word_pasuje )
				{
					checker.setLetters( letters , number_of_letters );

					blanks_left = number_of_blanks;

					/* Tutaj probujemy dopasowac nasze literki do slowa */
					for( i=0; i < pattern->length - pattern->number_of_letters; i++ )
						if( checker.substractLetter( beginning[(int)pattern->notletters[i]] ) && blanks_left )
						{
							where_blank[ number_of_blanks - blanks_left ] = (int)pattern->notletters[i];
							blanks_left--;
						}

					word_pasuje = checker.goodWord( pattern->length - pattern->number_of_letters - ( number_of_blanks - blanks_left ) );
				}

				if( word_pasuje )
				{
					for( the_same = pattern; the_same != NULL; the_same = the_same->next_same )
					{
						points = 0;	    
						word_multiplikator = 1;
						aktx = the_same->x;
						akty = the_same->y;
						number_of_used_letters = 0;

						for( i=0; i<number_of_blanks - blanks_left; i++ )
						{
							blank_memory[i] = beginning[ where_blank[i] ];
							beginning[ where_blank[i] ] = BLANK;
						}

						for( i=0; i<word_length; i++ )
						{
							if( gameboard->letter( aktx , akty ) == EMPTY_FIELD )
							{
								switch( gameboard->bonus( aktx , akty ) )
								{
									case NORMAL:
										points += alphabet.getLetterPoints( (unsigned char)beginning[i] );
										break;
									case TWO_LETTER_BONUS:
										points += 2 * alphabet.getLetterPoints( (unsigned char)beginning[i] );
										break;
									case THREE_LETTER_BONUS:
										points += 3 * alphabet.getLetterPoints( (unsigned char)beginning[i] );
										break;
									case TWO_WORD_BONUS:
										word_multiplikator *= 2;
										points += alphabet.getLetterPoints( (unsigned char)beginning[i] );
										break;
									case THREE_WORD_BONUS:
										word_multiplikator *= 3;
										points += alphabet.getLetterPoints( (unsigned char)beginning[i] );
										break;
									default:;
								}

								number_of_used_letters++;
							}
							else
							{
								if( !gameboard->isBlankLetter( aktx , akty ) )
									points += alphabet.getLetterPoints( gameboard->letter( aktx , akty ) );
							}

							if( the_same->horizont == HORIZONTAL )
								aktx++;
							else
								akty++;
						}

						points *= word_multiplikator;

						if( number_of_used_letters == 7 )
							points += 50;

						add_word( the_same , beginning , points , where_blank , number_of_blanks-blanks_left );

						for( i=0; i<number_of_blanks - blanks_left; i++ )
							beginning[ where_blank[i] ] = blank_memory[i];
					}
				}
			}

			word_length = 0;
			beginning = word + 1;
		}
		else
			word_length++;

		word++;
	}
}

void Sakreble::place_added_letters_on_gameboard( struct pattern_struct *pattern )
{
	int i, aktx, akty, which_blank = 0;

	if( pattern != NULL )
	{
		aktx = pattern->x;
		akty = pattern->y;

		for( i=0; i<pattern->length; i++ )
		{
			if( gameboard->letter( aktx , akty ) == EMPTY_FIELD )
				if( pattern->word->blanks && which_blank < pattern->word->number_of_blanks )
				{
					if( pattern->word->blanks[which_blank] == i )
					{
						which_blank++;
						gameboard->highlightBlankLetterUstawionaPrzezKomputer( pattern->word->word[i] , aktx , akty );
					}
					else
						gameboard->highlightLetterUstawionaPrzezKomputer( pattern->word->word[i] , aktx , akty );
				}
				else
					gameboard->highlightLetterUstawionaPrzezKomputer( pattern->word->word[i] , aktx , akty );

			if( pattern->horizont == HORIZONTAL )
				aktx++;
			else
				akty++;
		}
	}

	this_move->zeroPoints();

	this_move->addPoints( gameboard->getPointValue() );
}

/*
 * To jest funkcja wywolywana jedynie przy ruchu komputera !!!
 */
void Sakreble::place_word_on_gameboard( struct pattern_struct *pattern )
{
	int i, aktx, akty, which_blank = 0;
	EndGameConditionsRule *rule = (EndGameConditionsRule *)(rules->getRule( RULE_END_GAME_CODITIONS ));

	if( pattern != NULL )
	{
		number_of_pauses = 0;

		aktx = pattern->x;
		akty = pattern->y;

		for( i=0; i<pattern->length; i++ )
		{
			if( gameboard->letter( aktx , akty ) == EMPTY_FIELD )
			{
				if( pattern->word->blanks )
				{
					if( pattern->word->blanks[which_blank] == i )
					{
						which_blank++;
						gameboard->setLetter( pattern->word->word[i] , aktx , akty );
						gameboard->markBlankPlaced( aktx , akty );
						playerLetters[ whichPlayer ]->removeLetter( BLANK );
					}
					else
						gameboard->setLetter( pattern->word->word[i] , aktx , akty );
					playerLetters[ whichPlayer ]->removeLetter( pattern->word->word[i] );
				}
				else
				{
					gameboard->setLetter( pattern->word->word[i] , aktx , akty );
					playerLetters[ whichPlayer ]->removeLetter( pattern->word->word[i] );
				}
			}

			if( pattern->horizont == HORIZONTAL )
				aktx++;
			else
				akty++;
		}

		points[ whichPlayer ]->addPoints( pattern->word->points );
		this_move->setPoints( pattern->word->points );
	}
	else
	{
		/*
		 * Check if continuous pauses may end the game.
		 */
		if( rule->numberOfContinuousPausesToEnd() > 0 )
			/*
			 * Check if limit of continuous pauses was reached.
			 */
			if( ++number_of_pauses == rule->numberOfContinuousPausesToEnd() )
			{
				gameEnd();
				return;
			}

		this_move->setPoints( 0 );
	}

	if( players->getPlayingPlayer(whichPlayer)->realPlayer() )
		playerLetters[ whichPlayer ]->changeLetters( stand->getLettersOnStand() , 7 );
}

/*
 * Finds dictionary parts with with followed lenghts.
 *
 * Results are stored in lenghts array.
 */
void Sakreble::findLengths()
{
	int word_length = 0;
	char *word = dictionary, *beginning = dictionary;

	for( int i=0; i<15; i++ )
		lenghts[i] = NULL;

	while( word < dictionary_end )
	{
		if( *word == 10 )
		{
			if( lenghts[word_length-1] == NULL )
				lenghts[word_length-1] = beginning;

			word_length = 0;
			beginning = word + 1;
		}
		else
			word_length++;

		word++;
	}
}

int Sakreble::compareWords( char *s1 , char *s2 , int d )
{
	for( int i=0; i<d; i++ )
		if( (unsigned char)s1[i] < (unsigned char)s2[i] )
			return -1;
		else
			if( (unsigned char)s1[i] > (unsigned char)s2[i] )
				return 1;

	return 0;
}


int Sakreble::isWordInDictionaryPresent( char *word , int length )
{
	char *left, *mid = NULL, *right;
	int por = 0;

	left = lenghts[length-1];

	if( left == NULL )
		return 0;

	if( length == 15 )
		right = dictionary_end;
	else
	{
		if( lenghts[length] != NULL )
			right = lenghts[length];
		else
			right = dictionary_end;
	}

	while( left < right )
	{
		mid = (char *)(( (unsigned long)left + (unsigned long)right ) / 2);

		while( mid > left && *(mid - 1) != 10 )
			mid--;

		if( ( por = compareWords( word , mid , length) ) == 0 )
			return 1;

		if( por == 1 )
			left = mid + length + 1;
		else
			right = mid;
	}

	return 0;
}

void Sakreble::performOkButtonServiceActions( int enb )
{
	if( !enb )
	{
		ok->setEnabled( false );
		menuMove->setItemEnabled( menu_ids[ MOVE_OK_ID ] , false );

		this_move->zeroPoints();

		if( stand->areForChangeLetters() && !gameboard->thereAreAddedLetters() && alphabet.getLettersLeft() >= 7 )
		{
			wymien->setEnabled( true );
			menuMove->setItemEnabled( menu_ids[ MOVE_CHANGE_ID ] , true );
		}
		else
		{
			wymien->setEnabled( false );
			menuMove->setItemEnabled( menu_ids[ MOVE_CHANGE_ID ] , false );
		}

		return;
	}

	if( stand->areForChangeLetters() )
	{
		wymien->setEnabled( false );
		menuMove->setItemEnabled( menu_ids[ MOVE_CHANGE_ID ] , false );
	}
	else
	{
		ok->setEnabled( true );
		menuMove->setItemEnabled( menu_ids[ MOVE_OK_ID ] , true );

		this_move->zeroPoints();
		this_move->addPoints( gameboard->getPointValue() );
	}
}

void Sakreble::performChangeButtonServiceActions( int enb )
{
	if( !enb )
	{
		wymien->setEnabled( false );
		menuMove->setItemEnabled( menu_ids[ MOVE_CHANGE_ID ] , false );

		if( gameboard->isWordCorrectlyAdded() )
			if( gameboard->letter( 7 , 7 ) != EMPTY_FIELD )
			{
				ok->setEnabled( true );
				menuMove->setItemEnabled( menu_ids[ MOVE_OK_ID ] , true );

				this_move->zeroPoints();
				this_move->addPoints( gameboard->getPointValue() );
			}

		return;
	}

	if( gameboard->thereAreAddedLetters() )
	{
		ok->setEnabled( false );
		menuMove->setItemEnabled( menu_ids[ MOVE_OK_ID ] , false );

		this_move->zeroPoints();
	}
	else
		if( alphabet.getLettersLeft() >= 7 )
		{
			wymien->setEnabled( true );
			menuMove->setItemEnabled( menu_ids[ MOVE_CHANGE_ID ] , true );
		}
}

void Sakreble::playerPass()
{
	EndGameConditionsRule *rule = (EndGameConditionsRule *)(rules->getRule( RULE_END_GAME_CODITIONS ));
	
	highlighting->turnOffHighlighting();

	gameboard->removeAddedLetters();
	stand->removeForChangeLetters();
	stand->setLettersOnStand( playerLetters[whichPlayer]->getLetters() , 7 );

	/*
	 * Check if continuous pauses may end the game.
	 */
	if( rule->numberOfContinuousPausesToEnd() > 0 )
		/*
		 * Check if limit of continuous pauses was reached.
		 */
		if( ++number_of_pauses == rule->numberOfContinuousPausesToEnd() )
		{
			gameEnd();
			return;
		}

	if( players->getPlayingPlayer(1-whichPlayer)->virtualPlayer() )
	{
		showWidgets( FOR_COMPUTER_MOVE );

		whichPlayer = 1 - whichPlayer;

		if( players->getPlayingPlayer(whichPlayer)->realPlayer() )
			whichToLettersLeftPreview = whichPlayer;

		lamp[whichPlayer]->turnOn();
		lamp[1-whichPlayer]->turnOff();

		computerMove();
	}
	else
	{
		whichPlayer = 1 - whichPlayer;

		if( players->getPlayingPlayer(whichPlayer)->realPlayer() )
			whichToLettersLeftPreview = whichPlayer;

		lamp[whichPlayer]->turnOn();
		lamp[1-whichPlayer]->turnOff();

		showWidgets( FOR_PLAYER_MOVE );

		stand->setLettersOnStand( playerLetters[whichPlayer]->getLetters() , 7 );
	}

	if( lettersLeft->isVisible() )
		lettersLeft->repaint();
}

void Sakreble::playerChangesLetters()
{
	highlighting->turnOffHighlighting();

	playerLetters[whichPlayer]->changeLetters( stand->getLettersOnStand() , 7 );
	getRandomLetters( playerLetters[whichPlayer]->getLetters() , 7 );

	for( int i=0; i<7; i++ )
		if( stand->getLetterForChange( i ) != EMPTY_FIELD )
		{
			alphabet.addLetter( stand->getLetterForChange( i ) );
			alphabet.addLettersLeft();
		}
	lettersLeftLabel->setNum( alphabet.getLettersLeft() );

	stand->removeForChangeLetters();

	if( players->getPlayingPlayer(1-whichPlayer)->virtualPlayer() )
	{
		whichPlayer = 1 - whichPlayer;

		if( players->getPlayingPlayer(whichPlayer)->realPlayer() )
			whichToLettersLeftPreview = whichPlayer;

		lamp[whichPlayer]->turnOn();
		lamp[1-whichPlayer]->turnOff();

		showWidgets( FOR_COMPUTER_MOVE );

		stand->setLettersOnStand( playerLetters[1-whichPlayer]->getLetters() , 7 );

		computerMove();
	}
	else
	{
		whichPlayer = 1 - whichPlayer;

		if( players->getPlayingPlayer(whichPlayer)->realPlayer() )
			whichToLettersLeftPreview = whichPlayer;

		lamp[whichPlayer]->turnOn();
		lamp[1-whichPlayer]->turnOff();

		showWidgets( FOR_PLAYER_MOVE );

		stand->setLettersOnStand( playerLetters[whichPlayer]->getLetters() , 7 );
	}

	if( lettersLeft->isVisible() )
		lettersLeft->repaint();
}

void Sakreble::playerClickedOk()
{
	highlighting->turnOffHighlighting();

	if( gameboard->isBlankAdded() )
	{
		gameboard->highlightBlank();
		showWidgets( FOR_BLANK_CHANGE );
	}
	else
		playerSetsWord();
}

void Sakreble::blankChanged( char to_what )
{
	gameboard->turnOffBlank();
	gameboard->changeBlank( to_what );

	if( gameboard->isBlankAdded() )
	{
		gameboard->highlightBlank();
		showWidgets( FOR_BLANK_CHANGE );
	}
	else
		playerSetsWord();
}

void Sakreble::playerSetsWord()
{
	int firstAddedLetterXPos = -1, firstAddedLetterYPos = -1, horizont = HORIZONTAL;
	int direction_up = 0, direction_left = 0, points = 0;
	int second_letter_added = 0;

	byl_kit = 0;

	for( int i=0; i<15; i++ )
		for( int j=0; j<15; j++ )
			if( gameboard->addedLetter( i , j ) )
			{
				if( firstAddedLetterXPos == -1 )
				{
					firstAddedLetterXPos = i;
					firstAddedLetterYPos = j;
				}

				break;
			}

	for( int i=firstAddedLetterXPos+1; i<15; i++ )
		if( gameboard->addedLetter( i , firstAddedLetterYPos ) )
		{
			horizont = HORIZONTAL;
			second_letter_added = 1;
			break;
		}

	if( !second_letter_added )
		for( int j=firstAddedLetterYPos+1; j<15; j++ )
			if( gameboard->addedLetter( firstAddedLetterXPos , j ) )
			{
				horizont = VERTICAL;
				second_letter_added = 1;
				break;
			}

	if( !second_letter_added )
	{
		if( firstAddedLetterXPos > 0 && gameboard->letter( firstAddedLetterXPos - 1 , firstAddedLetterYPos ) != EMPTY_FIELD )
			horizont = HORIZONTAL;

		if( firstAddedLetterXPos < 14 && gameboard->letter( firstAddedLetterXPos + 1 , firstAddedLetterYPos ) != EMPTY_FIELD )
			horizont = HORIZONTAL;

		if( firstAddedLetterYPos > 0 && gameboard->letter( firstAddedLetterXPos , firstAddedLetterYPos - 1 ) != EMPTY_FIELD )
			horizont = VERTICAL;

		if( firstAddedLetterYPos < 14 && gameboard->letter( firstAddedLetterXPos , firstAddedLetterYPos + 1 ) != EMPTY_FIELD )
			horizont = VERTICAL;
	}

	if( horizont == HORIZONTAL )
		while( firstAddedLetterXPos > 0 && gameboard->letter( firstAddedLetterXPos - 1 , firstAddedLetterYPos ) != EMPTY_FIELD )
			firstAddedLetterXPos--;
	else
		while( firstAddedLetterYPos > 0 && gameboard->letter( firstAddedLetterXPos , firstAddedLetterYPos - 1 ) != EMPTY_FIELD )
			firstAddedLetterYPos--;

	if( isWordFromGameboardInDictionaryPresent( firstAddedLetterXPos , firstAddedLetterYPos , horizont ) )
		points += gameboard->getPointValue( firstAddedLetterXPos , firstAddedLetterYPos , horizont );
	else
		byl_kit = 1;

	while( firstAddedLetterXPos < 15 && firstAddedLetterYPos < 15 )
	{
		if( gameboard->letter( firstAddedLetterXPos , firstAddedLetterYPos ) == EMPTY_FIELD )
			break;

		if( gameboard->addedLetter( firstAddedLetterXPos , firstAddedLetterYPos ) )
		{
			if( horizont == HORIZONTAL )
			{
				direction_up = 0;
				while( firstAddedLetterYPos-direction_up-1 >= 0 )
					if( gameboard->letter( firstAddedLetterXPos , firstAddedLetterYPos-direction_up-1 ) != EMPTY_FIELD )
						direction_up++;
					else
						break;

				if( direction_up > 0 || ( firstAddedLetterYPos < 14 && gameboard->letter( firstAddedLetterXPos , firstAddedLetterYPos + 1 ) != EMPTY_FIELD ) )
				{
					if( isWordFromGameboardInDictionaryPresent( firstAddedLetterXPos , firstAddedLetterYPos-direction_up , VERTICAL ) )
						points += gameboard->getPointValue( firstAddedLetterXPos , firstAddedLetterYPos-direction_up , VERTICAL );
					else
						byl_kit = 1;
				}
			}
			else
			{
				direction_left = 0;
				while( firstAddedLetterXPos-direction_left-1 >= 0 )
					if( gameboard->letter( firstAddedLetterXPos-direction_left-1 , firstAddedLetterYPos ) != EMPTY_FIELD )
						direction_left++;
					else
						break;

				if( direction_left > 0 || ( firstAddedLetterXPos < 14 && gameboard->letter( firstAddedLetterXPos + 1 , firstAddedLetterYPos ) != EMPTY_FIELD ) )
				{
					if( isWordFromGameboardInDictionaryPresent( firstAddedLetterXPos-direction_left , firstAddedLetterYPos , HORIZONTAL ) )
						points += gameboard->getPointValue( firstAddedLetterXPos-direction_left , firstAddedLetterYPos , HORIZONTAL );
					else
						byl_kit = 1;
				}
			}
		}

		if( horizont == HORIZONTAL )
			firstAddedLetterXPos++;
		else
			firstAddedLetterYPos++;
	}

	if( players->getPlayingPlayer(1-whichPlayer)->virtualPlayer() )
		wordChecking();
	else
		showWidgets( FOR_WORD_CONFIRMATION );
}

int Sakreble::isWordFromGameboardInDictionaryPresent( int i , int j , int horizont )
{
	char word[15];
	int length = 0;

	while( i < 15 && j < 15 )
	{
		if( gameboard->letter( i , j ) != EMPTY_FIELD )
			word[ length++ ] = gameboard->letter( i , j );
		else
			break;

		if( horizont == HORIZONTAL )
			i++;
		else
			j++;
	}

	return isWordInDictionaryPresent( word , length );
}

void Sakreble::showWidgets( int for_what )
{
	switch( for_what )
	{
		case FOR_PLAYER_MOVE:
			ok->show();
			wymien->show();
			pas->show();
			ok->setEnabled( false );
			wymien->setEnabled( false );
			stand->setEnabled( true );
			next->hide();
			checkButton->hide();
			confirmButton->hide();

			if( !lettersLeft->isVisible() )
				lettersLeftButton->setEnabled( true );
			menuTools->setItemEnabled( menu_ids[ TOOLS_LETTERS_LEFT_PREVIEW_ID ] , true );

			menuMove->setItemEnabled( menu_ids[ MOVE_OK_ID ] , false );
			menuMove->setItemEnabled( menu_ids[ MOVE_CHANGE_ID ] , false );
			menuMove->setItemEnabled( menu_ids[ MOVE_PASS_ID ] , true );

			break;

		case FOR_COMPUTER_MOVE:
			ok->hide();
			wymien->hide();
			pas->hide();
			stand->setEnabled( false );
			next->show();
			checkButton->hide();
			confirmButton->hide();
			lettersLeftButton->setEnabled( false );
			menuTools->setItemEnabled( menu_ids[ TOOLS_LETTERS_LEFT_PREVIEW_ID ] , false );

			menuMove->setItemEnabled( menu_ids[ MOVE_OK_ID ] , false );
			menuMove->setItemEnabled( menu_ids[ MOVE_PASS_ID ] , false );
			menuMove->setItemEnabled( menu_ids[ MOVE_CHANGE_ID ] , false );

			break;

		case FOR_PLAYERS_CHOOSING:
			ok->hide();
			wymien->hide();
			pas->hide();
			next->hide();
			nextGame->hide();
			stand->setEnabled( false );
			gameboard->setEnabled( false );
			for( int i=0; i<2; i++ )
			{
				playerCombo[i]->show();
				playerCombo[i]->setEnabled( true );
				points[i]->hide();
				playerLetters[i]->hide();
				lamp[i]->hide();
			}
			this_move->hide();
			label->show();
			start->show();
			checkButton->hide();
			confirmButton->hide();

			menuGame->setItemEnabled( menu_ids[ GAME_START_ID ] , true );
			menuMove->setItemEnabled( menu_ids[ MOVE_OK_ID ] , false );
			menuMove->setItemEnabled( menu_ids[ MOVE_PASS_ID ] , false );
			menuMove->setItemEnabled( menu_ids[ MOVE_CHANGE_ID ] , false );

			if( menuTools->isItemChecked( menu_ids[ TOOLS_LETTERS_LEFT_PREVIEW_ID ] ) )
			{
				menuTools->setItemChecked( menu_ids[ TOOLS_LETTERS_LEFT_PREVIEW_ID ] , false );
				lettersLeft->hide();
				lettersLeftButton->hide();
			}
			menuTools->setItemEnabled( menu_ids[ TOOLS_LETTERS_LEFT_PREVIEW_ID ] , false );

			break;

		case FOR_GAME_BEGINNING:
			for( int i=0; i<2; i++ )
			{
				playerCombo[i]->hide();
				playerCombo[i]->setEnabled( false );
				points[i]->show();
				playerLetters[i]->show();
				lamp[i]->show();
			}
			this_move->show();
			lettersLeftButton->show();

			if( players->getPlayingPlayer(0)->virtualPlayer() )
			{
				lettersLeftButton->setEnabled( true );
				menuTools->setItemEnabled( menu_ids[ TOOLS_LETTERS_LEFT_PREVIEW_ID ] , true );
			}
			else
			{
				lettersLeftButton->setEnabled( false );
				menuTools->setItemEnabled( menu_ids[ TOOLS_LETTERS_LEFT_PREVIEW_ID ] , false );
			}

			lettersLeft->hide();
			label->hide();
			start->hide();
			gameboard->setEnabled( true );
			checkButton->hide();
			confirmButton->hide();

			menuGame->setItemEnabled( menu_ids[ GAME_START_ID ] , false );

			break;

		case FOR_GAME_END:
			ok->hide();
			wymien->hide();
			pas->hide();
			next->hide();
			nextGame->show();
			stand->setEnabled( false );
			gameboard->setEnabled( false );
			checkButton->hide();
			confirmButton->hide();
			lettersLeftButton->hide();

			if( menuTools->isItemChecked( menu_ids[ TOOLS_LETTERS_LEFT_PREVIEW_ID ] ) )
			{
				menuTools->setItemChecked( menu_ids[ TOOLS_LETTERS_LEFT_PREVIEW_ID ] , false );
				lettersLeft->hide();
			}
			menuTools->setItemEnabled( menu_ids[ TOOLS_LETTERS_LEFT_PREVIEW_ID ] , false );

			menuMove->setItemEnabled( menu_ids[ MOVE_OK_ID ] , false );
			menuMove->setItemEnabled( menu_ids[ MOVE_PASS_ID ] , false );
			menuMove->setItemEnabled( menu_ids[ MOVE_CHANGE_ID ] , false );

			break;

		case FOR_WORD_CONFIRMATION:
			ok->hide();
			wymien->hide();
			pas->hide();
			next->hide();
			stand->setEnabled( false );
			checkButton->show();
			confirmButton->show();

			menuMove->setItemEnabled( menu_ids[ MOVE_OK_ID ] , false );
			menuMove->setItemEnabled( menu_ids[ MOVE_PASS_ID ] , false );
			menuMove->setItemEnabled( menu_ids[ MOVE_CHANGE_ID ] , false );

			break;

		case FOR_BLANK_CHANGE:
			changeBlank->show();

		default:;
	}
}

void Sakreble::gameEnd()
{
	int lettersLeft[2];
	int rankValue[2];
	int victory = 1;

	lamp[whichPlayer]->turnOff();
	lamp[1-whichPlayer]->turnOff();

	showWidgets( FOR_GAME_END );

	lettersLeft[0] = lettersLeft[1] = 0;

	for( int gr=0; gr<2; gr++ )
		for( int i=0; i<7; i++ )
			if( playerLetters[gr]->letter(i) != EMPTY_FIELD )
				lettersLeft[gr] += alphabet.getLetterPoints( playerLetters[gr]->letter(i) );

	rankValue[0] = (int) players->getPlayingPlayer(0)->rankValue();
	rankValue[1] = (int) players->getPlayingPlayer(1)->rankValue();

	if( playerLetters[whichPlayer]->numberOfLetters() == 0 )
	{
		points[whichPlayer]->addPoints( lettersLeft[1-whichPlayer] );
		points[1-whichPlayer]->addPoints( -lettersLeft[1-whichPlayer] );
	}
	else
	{
		points[whichPlayer]->addPoints( -lettersLeft[whichPlayer] );
		points[1-whichPlayer]->addPoints( -lettersLeft[1-whichPlayer] );
	}

	/*
	 * Uaktualniamy playerom rankValuei
	 */
	if( points[whichPlayer]->getPoints() < points[1-whichPlayer]->getPoints() )
		whichPlayer = 1-whichPlayer;
	else
	{
		if( points[whichPlayer]->getPoints() == points[1-whichPlayer]->getPoints() )
			victory = 0; // Byl remis
		else
			victory = 1; // Czyjas victory
	}

	if( rankValue[whichPlayer] - rankValue[1-whichPlayer] <= 50 )
	{
		players->getPlayingPlayer( whichPlayer )->addScalp( rankValue[1-whichPlayer] + 50*victory );
		players->getPlayingPlayer( 1-whichPlayer )->addScalp( rankValue[whichPlayer] - 50*victory );
	}
	else
	{
		players->getPlayingPlayer( whichPlayer )->addScalp( rankValue[whichPlayer] );
		players->getPlayingPlayer( 1-whichPlayer )->addScalp( rankValue[1-whichPlayer] );
	}
}

void Sakreble::choosePlayers()
{
	showWidgets( FOR_PLAYERS_CHOOSING );
}

void Sakreble::lettersLeftPreviewServiceActions()
{
	if( menuTools->isItemChecked( menu_ids[ TOOLS_LETTERS_LEFT_PREVIEW_ID ] ) )
	{
		menuTools->setItemChecked( menu_ids[ TOOLS_LETTERS_LEFT_PREVIEW_ID ] , false );
		lettersLeft->hide();
		lettersLeftButton->setEnabled( true );
	}
	else
	{
		menuTools->setItemChecked( menu_ids[ TOOLS_LETTERS_LEFT_PREVIEW_ID ] , true );
		lettersLeft->show();
		lettersLeftButton->setEnabled( false );
	}
}

/*
 * Sets player data file.
 *
 * Reads also players data from this file.
 */
void Sakreble::setPlayerDataFile( QString p )
{
	is_player_data_file_set = true;

	player_data_file = p;
	
	players->readFromFile( player_data_file );

	addPlayerNamesIntoCombos();
	if( players->getNumberOfPlayers() > 1 )
		playerCombo[1]->setCurrentItem(1);

	makeCombos();
}

void Sakreble::addPlayerNamesIntoCombo( int which_one )
{
	playerCombo[which_one]->clear();

	for( int i=0; i<players->getNumberOfPlayers(); i++ )
	{
		QString s;

		if( (*players)[i].virtualPlayer() )
			s.sprintf( "* %s" , (*players)[i].getName() );
		else
			s.sprintf( "  %s" , (*players)[i].getName() );

		playerCombo[which_one]->insertItem( s , -1 );
	}
}

void Sakreble::addPlayerNamesIntoCombos()
{
	addPlayerNamesIntoCombo( 0 );
	addPlayerNamesIntoCombo( 1 );
}

/*
 * Tutaj dbam o to, by player nie mogl grac sam ze soba - po prostu nie bedzie
 * mial do wyboru samego siebie
 */
void Sakreble::comboServiceActions( int whichPlayer , const QString &name )
{
	QString com1 = playerCombo[1-whichPlayer]->currentText();
	QString com0 = playerCombo[whichPlayer]->currentText();

	addPlayerNamesIntoCombos();

	/*
	 * Najpierw obu playerom usuwam ich adwersarzy
	 */
	for( int i=0; i<playerCombo[whichPlayer]->count(); i++ )
		if( playerCombo[whichPlayer]->text(i) == com1 )
		{
			playerCombo[whichPlayer]->removeItem(i);

			break;
		}

	for( int i=0; i<playerCombo[1-whichPlayer]->count(); i++ )
		if( playerCombo[1-whichPlayer]->text(i) == com0 )
		{
			playerCombo[1-whichPlayer]->removeItem(i);

			break;
		}

	/*
	 * Potem temu drugiemu ustawiam, co tam mial na beginningatku
	 */
	for( int i=0; i<playerCombo[1-whichPlayer]->count(); i++ )
		if( playerCombo[1-whichPlayer]->text(i) == com1 )
		{
			playerCombo[1-whichPlayer]->setCurrentItem(i);

			break;
		}

	/*
	 * A na koncu w tym, co sie wybor zmienil ustawiam na to, co wybrano
	 */
	for( int i=0; i<playerCombo[whichPlayer]->count(); i++ )
		if( playerCombo[whichPlayer]->text(i) == name )
		{
			playerCombo[whichPlayer]->setCurrentItem(i);

			break;
		}
}

void Sakreble::combo0ServiceActions( const QString &name )
{
	comboServiceActions( 0 , name );
}

void Sakreble::combo1ServiceActions( const QString &name )
{
	comboServiceActions( 1 , name );
}

void Sakreble::makeCombos()
{
	comboServiceActions( 0 , playerCombo[0]->currentText() );
}

void Sakreble::showAboutPlayers()
{
	aboutPlayers->show();
}

/*
 * Sets configuration data file path.
 */
void Sakreble::setConfigurationFilePath( QString p )
{
	configuration_file_path = p;

	is_configuration_file_path_set = configParser->parseFile( p );

	/*
	 * Dictionary can be read only after configuration file setting.
	 */
	if( is_configuration_file_path_set && is_dictionary_path_set )
			read_dictionary();
}

