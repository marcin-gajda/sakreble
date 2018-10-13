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

#include <stdlib.h>
#include <stdio.h>

#include "qwindefs.h"
#include "qpushbt.h"
#include "qlined.h"
#include "qlabel.h"
#include "qbttngrp.h"
#include "qradiobt.h"
#include "qgrpbox.h"
#include "valid.h"
#include "qcombo.h"
#include "qmsgbox.h"
#include "qfile.h"

#include "player.h"

Player::Player()
{
	name = NULL;
	setName( DEFAULT_NAME );

	profitability = 0;
	min_word_length = 2;
	max_word_length = 15;

	type = VIRTUAL_PLAYER;

	initialize();
}

Player::Player( const char *i , int o , int min , int max )
{
	name = NULL;
	setName( i );

	profitability = o;
	max_word_length = max;
	min_word_length = min;

	type = VIRTUAL_PLAYER;

	initialize();
}

Player::Player( const char *i )
{
	name = NULL;
	setName( i );

	profitability = 0;
	min_word_length = 2;
	max_word_length = 15;

	type = REAL_PLAYER;

	initialize();
}

Player::Player( const Player &g )
{
	setName( g.name );

	for( int i=0; i<NUMBER_OF_GAMES_FOR_SCALPS; i++ )
		scalps[i] = g.scalps[i];

	profitability = g.profitability;
	max_word_length = g.max_word_length;
	min_word_length = g.min_word_length;

	number_of_scalps  = g.number_of_scalps;
	which_scalp  = g.which_scalp;
	scalp_sum = g.scalp_sum;

	type = g.type;
}

Player& Player::operator=( const Player &g )
{
	setName( g.name );

	for( int i=0; i<NUMBER_OF_GAMES_FOR_SCALPS; i++ )
		scalps[i] = g.scalps[i];

	profitability = g.profitability;
	max_word_length = g.max_word_length;
	min_word_length = g.min_word_length;

	number_of_scalps  = g.number_of_scalps;
	which_scalp  = g.which_scalp;
	scalp_sum = g.scalp_sum;

	type = g.type;

	return *this;
}

Player::~Player()
{
	delete name;
}

void Player::initialize()
{
	for( int i=0; i<NUMBER_OF_GAMES_FOR_SCALPS; i++ )
		scalps[i] = 0;

	number_of_scalps = 0;
	scalp_sum = 0;
	which_scalp = 0;
}

void Player::setName( const char *i )
{
	if( name != NULL )
		delete name;

	name = new char[ strlen( i ) + 1 ];	// Jedynka na znak konca ciagu

	strcpy( name , i );
}

void Player::addScalp( int s )
{
	if( number_of_scalps < NUMBER_OF_GAMES_FOR_SCALPS )
	{
		scalps[ number_of_scalps++ ] = s;
		scalp_sum += s;
		which_scalp = number_of_scalps;
	}
	else
	{
		which_scalp == NUMBER_OF_GAMES_FOR_SCALPS ? which_scalp = 0 : which_scalp++;

		scalp_sum -= scalps[which_scalp];
		scalps[which_scalp] = s;
		scalp_sum += s;
	}
}

double Player::rankValue()
{
	if( number_of_scalps < 12 )
		return 100.0;

	return ( scalp_sum / ( number_of_scalps * 1.0 ) );
}

QTextStream & operator<<( QTextStream &wy , Player &g )
{
	wy << g.name << endl;
	wy << g.number_of_scalps << " ";
	wy << g.which_scalp << " ";
	wy << g.profitability << " ";
	wy << g.min_word_length << " ";
	wy << g.max_word_length << " ";
	wy << g.type << " ";

	if( g.number_of_scalps > 0 )
		wy << g.scalps[0];

	for( int i=1; i<g.number_of_scalps; i++ )
		wy << " " << g.scalps[i];

	return wy;
}

QTextStream & operator>>( QTextStream &we , Player &g )
{
	QString im;

	im = we.readLine(); we.skipWhiteSpace();
	g.setName( im.ascii() );

	we >> g.number_of_scalps; we.skipWhiteSpace();
	we >> g.which_scalp; we.skipWhiteSpace();

	we >> g.profitability; we.skipWhiteSpace();
	we >> g.min_word_length; we.skipWhiteSpace();
	we >> g.max_word_length; we.skipWhiteSpace();
	we >> g.type; we.skipWhiteSpace();

	g.scalp_sum = 0;

	for( int i=0; i<g.number_of_scalps; i++ )
	{
		we >> g.scalps[i]; we.skipWhiteSpace();
		g.scalp_sum += g.scalps[i];
	}

	/*
	 * Pozostale wypelniamy zerami
	 */
	for( int i=g.number_of_scalps; i<NUMBER_OF_GAMES_FOR_SCALPS; i++ )
		g.scalps[i] = 0;

	return we;
}

/*
 * Funkcja kontrolna, napisana, bo nie chce dzialac cout
 */
void Player::printAboutPlayer()
{
	printf( "Imiê: %s\n" , name );
	printf( "Typ : %d " , type );
	printf( "Oplacalnosc: %d " , profitability );
	printf( "MinSlowa: %d " , min_word_length );
	printf( "MaxSlowa: %d\n" , max_word_length );
	printf( "IleSkalpow: %d " , number_of_scalps );
	printf( "SumaSkalpow: %d " , scalp_sum );
	printf( "KtorySkalp: %d " , which_scalp );
	printf( "Ranking: %f\n" , rankValue() );

	for( int i=0; i<number_of_scalps; i++ )
		printf( "%d " , scalps[i] );

	printf( "\n" );
}



















/*
 * Klasa Players
 */

Players::Players( QWidget *parent , const char *name ) : QSemiModal( parent , name , true , WStyle_DialogBorder )
{
	QFontMetrics fm = fontMetrics();
	QString s;
	int max_size;

	setMaximumSize( 300 , 200 );
	setMinimumSize( 300 , 200 );

	addButton = new QPushButton( "&Ok", this, "dodaj" );
	addButton->setGeometry(  width() / 2 - 65 , height() - 25 , 60 , 20 );

	cancelButton = new QPushButton( "&Cancel", this, "anuluj" );
	cancelButton->setGeometry( width() / 2 + 5 , height() - 25 , 60 , 20 );

	labels[ ET_NAME ] = new QLabel( "Name:" , this , "et name" );
	labels[ ET_NAME ]->move( 10 , 20 );

	playerName = new QLineEdit( this , "nameplayera" );
	playerName->setMaxLength( 23 );
	playerName->move( labels[ ET_NAME ]->x() + 5 + fm.width( labels[ ET_NAME ]->text() ) , labels[ ET_NAME ]->y() );

	border = new QGroupBox( "Virtual player parameters" , this , "border" );
	border->setGeometry( 10 , 55 , width() - 20 , height() - 55 - 10 - 25 );

	labels[ ET_PROFITABILITY ] = new QLabel( "Blank profitability:" , this , "et profitability" );
	labels[ ET_MIN_WORD_LENGTH ] = new QLabel( "Minimal word length:" , this , "et minslowa" );
	labels[ ET_MAX_WORD_LENGTH ] = new QLabel( "Maximal word length:" , this , "et minslowa" );

	max_size = QMAX( fm.width( labels[ ET_PROFITABILITY ]->text() ) , fm.width( labels[ ET_MIN_WORD_LENGTH ]->text() ) );
	max_size = QMAX( max_size , fm.width( labels[ ET_MAX_WORD_LENGTH ]->text() ) );

	labels[ ET_PROFITABILITY ]->setGeometry( 20 , 70 , max_size , 30 );
	labels[ ET_MIN_WORD_LENGTH ]->setGeometry( 20 , 100 , max_size , 30 );
	labels[ ET_MAX_WORD_LENGTH ]->setGeometry( 20 , 130 , max_size , 30 );

	blankProfitability = new QLineEdit( this , "profitabilityblank" );
	blankProfitability->move( labels[ ET_PROFITABILITY ]->x() + 5 + max_size , labels[ ET_PROFITABILITY ]->y() );

	minWordLength = new QComboBox( this , "minslowa" );
	minWordLength->move( labels[ ET_MIN_WORD_LENGTH ]->x() + 5 + max_size , labels[ ET_MIN_WORD_LENGTH ]->y() );

	maxWordLength = new QComboBox( this , "maxslowa" );
	maxWordLength->move( labels[ ET_MAX_WORD_LENGTH ]->x() + 5 + max_size , labels[ ET_MAX_WORD_LENGTH ]->y() );

	for( int i=2; i<=15; i++ )
	{
		s.setNum( i );

		minWordLength->insertItem( s );
		maxWordLength->insertItem( s );
	}

	validators[ VALID_PROFITABILITY ] = new MyIntValidator( 0 , 32000 , (QObject *)this , "val oplcalnosc" );

	blankProfitability->setValidator( validators[ VALID_PROFITABILITY ] );

	playerType = new QButtonGroup( this );
	playerType->move( 150 , 5 );
	playerType->resize( 140 , 50 );
	playerType->setTitle( "Player type" );
	max_size = QMAX( 80 , fm.width( "Real player" ) );
	max_size = QMAX( max_size , fm.width( "Virtual player" ) );
	max_size += 20;
	QRadioButton *rb = new QRadioButton( "Virtual player" , playerType );
	rb->setGeometry( 5 , 15 , max_size , 15 );
	rb->setChecked( true );
	rb = new QRadioButton( "Real player" , playerType );
	rb->setGeometry( 5 , 30 , max_size , 15 );

	connect( cancelButton , SIGNAL( clicked() ) , SLOT( hide() ) );
	connect( addButton , SIGNAL( clicked() ) , SLOT( tryToAddPlayer() ) );
	connect( playerType , SIGNAL( clicked( int ) ) , SLOT( enableParameters( int ) ) );
	connect( minWordLength , SIGNAL( activated( int ) ) , SLOT( minWordLengthRange( int ) ) );
	connect( maxWordLength , SIGNAL( activated( int ) ) , SLOT( maxWordLengthRange( int ) ) );

	players = NULL;
	initialize( 2 );
}

Players::Players( QWidget *parent , const char *name , int i ) : QSemiModal( parent , name , true , WStyle_DialogBorder )
{
	players = NULL;
	initialize( i );
}

Players::~Players()
{
	delete [] players;
}

void Players::initialize( int how_much )
{
	how_much_players = how_much;

	if( players != NULL )
		delete [] players;

	players = new Player[ how_much_players ];
};

void Players::addPlayer( const Player &g )
{
	Player *pom = new Player[ how_much_players + 1 ];

	for( int i=0; i<how_much_players; i++ )
		pom[i] = players[i];

	pom[how_much_players++] = g;

	int i = how_much_players - 1;

	delete [] players;

	players = pom;

	while( i > 0 )
	{
		QString s1( players[i].getName() );
		QString s2( players[i-1].getName() );
		QString ss1 = s1.lower();
		QString ss2 = s2.lower();

		if( strcmp( ss1 , ss2 ) < 0 )
		{
			Player pom;

			pom = players[i];
			players[i] = players[i-1];
			players[i-1] = pom;
		}

		i--;
	}

}

void Players::addPlayer( const char *im , int o , int min , int max )
{
	addPlayer( Player( im , o , min , max ) );
}

void Players::addPlayer( const char *im )
{
	addPlayer( Player( im ) );
}

Player *Players::findPlayer( const char *im )
{
	for( int i=0; i<how_much_players; i++ )
		if( strcmp( players[i].getName() , im ) == 0 )
			return &players[i];

	return NULL;
}

void Players::readFromFile( QString fname )
{
	QFile file( fname );
	if( file.open( IO_ReadOnly ) )
	{
		QTextStream ts( &file );

		ts >> how_much_players; ts.skipWhiteSpace();

		if( players != NULL )
			delete [] players;

		players = new Player[ how_much_players ];

		int which_player = 0;

		for( int i=0; i<how_much_players; i++ )
			ts >> players[which_player++];

		file.close();
	}
}

void Players::writeToFile( QString fname )
{
	QFile file( fname );

	if( file.open( IO_WriteOnly ) )
	{
		QTextStream ts( &file );

		ts << how_much_players;

		for( int i=0; i<how_much_players; i++ )
			ts << endl << players[i];

		file.close();
	}
}

/*
 * Tu mamy juz na glowie rysowanie
 */
void Players::addPlayer()
{
	setCaption( "Dodaj playera" );

	showWidgets( FOR_PLAYER_ADDING );

	show();
}

void Players::hideWidgets()
{
	addButton->hide();
	cancelButton->hide();
	playerName->hide();
	playerType->hide();

	for( int i=0; i<NUMBER_OF_LABELS; i++ )
		labels[i]->hide();

	blankProfitability->hide();
	maxWordLength->hide();
	minWordLength->hide();
	border->hide();
}

void Players::showWidgets( int for_what )
{
	hideWidgets();

	switch( for_what )
	{
		case FOR_PLAYER_ADDING:
			addButton->show();
			cancelButton->show();
			playerName->show();
			playerType->show();
			border->show();
			labels[ ET_NAME ]->show();
			labels[ ET_PROFITABILITY ]->show();
			labels[ ET_MIN_WORD_LENGTH ]->show();
			labels[ ET_MAX_WORD_LENGTH ]->show();
			blankProfitability->show();
			minWordLength->show();
			maxWordLength->show();

			playerName->setText( tr( DEFAULT_NAME ) );
			blankProfitability->setText( "0" );
			minWordLength->setCurrentItem( 0 );
			maxWordLength->setCurrentItem( 13 );

			break;

		default:;
	}
}

void Players::enableParameters( int what_pressed )
{
	int to_what = true;

	switch( what_pressed )
	{
		case RADIO_VIRTUAL:
			to_what = true;
			break;

		case RADIO_REAL:
			to_what = false;
			break;

		default:;
	}

	blankProfitability->setEnabled( to_what );
	minWordLength->setEnabled( to_what );
	maxWordLength->setEnabled( to_what );
}

void Players::minWordLengthRange( int index )
{
	QString s1 = minWordLength->text( index ), s2 = maxWordLength->currentText();

	if( s1.toInt() > s2.toInt() )
	{
		minWordLength->setCurrentItem( s2.toInt() - 2 );
		return;
	}
}

void Players::maxWordLengthRange( int index )
{
	QString s1 = minWordLength->currentText(), s2 = maxWordLength->text( index );

	if( s1.toInt() > s2.toInt() )
	{
		maxWordLength->setCurrentItem( s1.toInt() - 2 );
		return;
	}
}

void Players::tryToAddPlayer()
{
	QString s = playerName->text();

	if( strlen( playerName->text() ) == 0 )
	{
		QMessageBox::information( this , "Z³e imiê" , "Playerowi nale¿y wybraæ jakie¶ imiê." );
		return;
	}

	if( findPlayer( playerName->text() ) != NULL )
	{
		QMessageBox::information( this , "Z³e imiê" , "Jest ju¿ player o takim nameniu." );
		return;
	}

	if( blankProfitability->isEnabled() )
	{
		QString s1 = blankProfitability->text();

		addPlayer( s , s1.toInt() , minWordLength->currentItem() + 2 , maxWordLength->currentItem() + 2 );
	}
	else
		addPlayer( s );

	hide();

	emit playerAdded();
}

Player& Players::operator[]( int elem )
{
	if( elem >= 0 && elem < how_much_players )
		return players[elem];

	exit(0);
}

void Players::setPlayingPlayers( int which , const QString &im )
{
	for( int i=0; i<how_much_players; i++ )
		if( QString( players[i].getName() ) == im.right( im.length() - 2 ) ) // -2, bo w comboboksach 2 znaczki na beginningatku
		{
			playing_players[which] = i;
			return;
		}
}
