#
# Sakreble! - open word game.
# Copyright (C) 2004 Marcin Gajda
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#  
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

INCDIR	=  /usr/include/qt/
CFLAGS	= -O2 -fno-strength-reduce -Wall -W -I/usr/X11R6/include -I/usr/include/libxml2
LFLAGS  = -lqt-mt -lxml2
SHELL   = /bin/sh
CC	= gcc
MOC	= /usr/bin/moc

####### Files

HEADERS = \
	sakreble.h \
	gameboard.h \
	stand.h \
	dupa.h \
	alphabet.h \
	points.h \
	letters.h \
	highlighting.h \
	player.h \
	lamp.h \
	blank.h \
	checker.h \
	lettersleft.h \
	valid.h \
	aboutplayers.h \
	configparser.h \
	rules.h

SOURCES = \
	sakreble.cc \
	gameboard.cc \
	stand.cc \
	main.cc \
	dupa.c alphabet.cc \
	points.cc \
	letters.cc \
	highlighting.cc \
	player.cc \
	lamp.cc \
	blank.cc \
	checker.cc \
	lettersleft.cc \
	valid.cc \
	aboutplayers.cc \
	configparser.cc \
	rules.cc

OBJECTS = \
	sakreble.o \
	gameboard.o \
	stand.o \
	main.o \
	dupa.o alphabet.o \
	points.o \
	letters.o \
	highlighting.o \
	player.o \
	lamp.o \
	blank.o \
	checker.o \
	lettersleft.o \
	valid.o \
	aboutplayers.o \
	configparser.o \
	rules.o

SRCMETA = \
	msakreble.cc \
	mgameboard.cc \
	mstand.cc \
	mpoints.cc \
	mhighlighting.cc \
	mletters.cc \
	mlamp.cc \
	mblank.cc \
	mlettersleft.cc \
	mplayer.cc \
	mvalid.cc \
	maboutplayers.cc \

OBJMETA = \
	msakreble.o \
	mgameboard.o \
	mstand.o \
	mpoints.o \
	mhighlighting.o \
	mletters.o \
	mlamp.o \
	mblank.o \
	mlettersleft.o \
	mplayer.o \
	mvalid.o \
	maboutplayers.o \

TARGET	=	Sakreble

####### Implicit rules

.SUFFIXES: .cc

.cc.o:
	$(CC) -c $(CFLAGS) -I$(INCDIR) $<

####### Build rules

all: $(TARGET)

$(TARGET): $(OBJECTS) $(OBJMETA)
	$(CC) $(OBJECTS) $(OBJMETA) -o $(TARGET) $(LFLAGS) -lm

depend:
	@makedepend -I$(INCDIR) $(SOURCES) 2> /dev/null

showfiles:
	@echo $(HEADERS) $(SOURCES) Makefile

clean:
	rm -f *.o *.bak *~ *% #*
	rm -f $(SRCMETA) $(TARGET)

####### Meta classes

msakreble.cc: sakreble.h
	$(MOC) -o $@ sakreble.h

mgameboard.cc: gameboard.h
	$(MOC) -o $@ gameboard.h

mstand.cc: stand.h
	$(MOC) -o $@ stand.h

mpoints.cc: points.h
	$(MOC) -o $@ points.h

mhighlighting.cc: highlighting.h
	$(MOC) -o $@ highlighting.h

mletters.cc: letters.h
	$(MOC) -o $@ letters.h

mlamp.cc: lamp.h
	$(MOC) -o $@ lamp.h

mblank.cc: blank.h
	$(MOC) -o $@ blank.h

mlettersleft.cc: lettersleft.h
	$(MOC) -o $@ lettersleft.h

mplayer.cc: player.h
	$(MOC) -o $@ player.h

mvalid.cc: valid.h
	$(MOC) -o $@ valid.h

maboutplayers.cc: aboutplayers.h
	$(MOC) -o $@ aboutplayers.h

