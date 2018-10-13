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

#ifndef PLAYER_H
#define PLAYER_H

#include <qtstream.h>
#include <qsemimodal.h>

#include "dupa.h"

#define NUMBER_OF_GAMES_FOR_SCALPS 200
#define DEFAULT_NAME "¯ó³todziób"

class Player
{
	friend QTextStream& operator<<( QTextStream& , Player& );
	friend QTextStream& operator>>( QTextStream& , Player& );

	public:
	Player();
	Player( const char * );
	Player( const char * , int , int , int );
	Player( const Player & );
	~Player();

	Player& operator=( const Player & );

	void setName( const char * );
	char *getName() { return name; }
	int getMinWordLength() { return min_word_length; }
	int getMaxWordLength() { return max_word_length; }
	int getBlakProfitability() { return profitability; }
	void addScalp( int );
	double rankValue();
	void printAboutPlayer();
	int virtualPlayer() { return type == VIRTUAL_PLAYER; }
	int realPlayer() { return type == REAL_PLAYER; }

	private:
	void initialize();

	char *name;
	int scalps[ NUMBER_OF_GAMES_FOR_SCALPS ];
	int number_of_scalps, scalp_sum, which_scalp;
	int profitability, min_word_length, max_word_length;
	int type;
};

#define FOR_PLAYER_ADDING 0

#define NUMBER_OF_LABELS 4
#define ET_NAME 0
#define ET_PROFITABILITY 1
#define ET_MIN_WORD_LENGTH 2
#define ET_MAX_WORD_LENGTH 3

#define RADIO_VIRTUAL 0
#define RADIO_REAL 1

#define NUMBER_OF_VALIDATORS 1
#define VALID_PROFITABILITY 0

class QPushButton;
class QLineEdit;
class QLabel;
class QButtonGroup;
class QGroupBox;
class QComboBox;
class MyIntValidator;

class Players : public QSemiModal
{
	Q_OBJECT

	public:
		Players( QWidget * , const char * );
		Players( QWidget * , const char * , int );
		~Players();

		Player& operator[]( int );

		void addPlayer( const char * );
		void addPlayer( const char * , int , int , int );
		void addPlayer( const Player & );
		Player *findPlayer( const char * );
		void writeToFile( QString );
		void readFromFile( QString );
		int getNumberOfPlayers() { return how_much_players; }
		Player *getPlayer( int which_one ) { return &players[which_one]; }
		Player *getPlayingPlayer( int which_one ) { return &players[ playing_players[which_one] ]; }
		void setPlayingPlayers( int which , int kto ) { playing_players[which] = kto; }
		void setPlayingPlayers( int , const QString & );

		void showWidgets( int );
		void hideWidgets();

	public slots:
		void addPlayer();
		void enableParameters( int );
		void minWordLengthRange( int );
		void maxWordLengthRange( int );
		void tryToAddPlayer();

	signals:
		void playerAdded();

	private:
		void initialize( int );

		QPushButton *addButton, *cancelButton;
		QLineEdit *playerName, *blankProfitability;
		QComboBox *minWordLength, *maxWordLength;
		QLabel *labels[ NUMBER_OF_LABELS ];
		MyIntValidator *validators[ NUMBER_OF_VALIDATORS ];
		QButtonGroup *playerType;
		QGroupBox *border;

		Player *players;
		int how_much_players;
		int playing_players[2];	// Ktorzy players wlasnie tocza boj
};
#endif
