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

#ifndef ABOUT_PLAYERS_H
#define ABOUT_PLAYERS_H

#include <qtstream.h>
#include <qsemimodal.h>

#include "player.h"

class QPushButton;
class QListBox;
class Players;

class AboutPlayers : public QSemiModal
{
    Q_OBJECT

    public:
	AboutPlayers( QWidget * , const char * );
	void setPlayers( Players *g ) { players = g; }
	void show();

    private:
	QListBox *table;
	QPushButton *closeButton;
	Players *players;
};
#endif
