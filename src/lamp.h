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

#ifndef LAMP_H
#define LAMP_H

#include <qwidget.h>

#define TURNED_OFF 0
#define TURNED_ON 1

class QLabel;

class Lamp : public QWidget
{
    Q_OBJECT

    public:
	Lamp( QWidget * , const char * );

	void turnOff();
	void turnOn();

    private:
	QLabel *label;

	int state;
};

#endif
