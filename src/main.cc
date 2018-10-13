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

#include <qapplication.h>
#include <qtextcodec.h>
#include <qstring.h>

#include <string.h>

#include "sakreble.h"
#include "dupa.h"


int main( int argc, char **argv )
{
    QApplication::setColorSpec( QApplication::CustomColor );
    QApplication a( argc, argv );

    Sakreble s;

	/*
	 * Analyzes and sets program options.
	 */
	int i = 1;

	while( i<argc )
	{
		if( strncmp( argv[i] , "--dictionary-path=" , 18 ) == 0 )
		{
			s.setDictionaryPath( QString( argv[i] + 18 ) );

			i++;

			continue;
		}

		if( strncmp( argv[i] , "--player-data-file=" , 19 ) == 0 )
		{
			s.setPlayerDataFile( QString( argv[i] + 19 ) );

			i++;

			continue;
		}

		if( strncmp( argv[i] , "--config-file=" , 14 ) == 0 )
		{
			s.setConfigurationFilePath( QString( argv[i] + 14 ) );
	
			i++;

			continue;
		}

		/*
		 * Reaching of this point means that program was called
		 * with bad option.
		 */
		printf( "Sakreble! - open word game\n\n" );
		printf( "Usage:\n" );
		printf( "$ Sakreble [options]\n\n" );
		printf( "where valid options are:\n" );
		printf( "\t--help, -h                 - show this help\n" );
		printf( "\t--dictionary-path=path     - path to dictionary file\n" );
		printf( "\t                             (./slownik default)\n" );
		printf( "\t--player-data-file=path    - path to players data file\n" );
		printf( "\t                             (./dane_graczy default)\n" );
		printf( "\t--config-file=path         - path to configuration file\n" );
		printf( "\t                             (./sakreble-config default)\n" );
		
		exit(2);
	}

	/*
	 * Default options, if not set.
	 */
	if( !s.isDictionaryPathSet() )
		s.setDictionaryPath( QString( "./slownik" ) );

	if( !s.isPlayerDataFileSet() )
		s.setPlayerDataFile( QString( "./dane_graczy" ) );

	if( !s.isConfigurationFilePathSet() )
		s.setConfigurationFilePath( QString( "./sakreble-config" ) );

	/*
	 * Last check if everything is ok.
	 */
	if( !s.isConfigurationFilePathSet() )
	{
		printf( "Error in config file or config file not present.\n" );

		exit(2);
	}

	/*
	 * Qt initialization stuff.
	 */
	QTextCodec::setCodecForTr( QTextCodec::codecForName( QTextCodec::locale() ) );
	
    s.setGeometry( 0, 0, 600 , 510 );
    a.setMainWidget( &s );
    s.show();

	/*
	 * Let's go!
	 */
    return a.exec();
}

