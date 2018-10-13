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

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "configparser.h"
#include "alphabet.h"
#include "gameboard.h"
#include "rules.h"
#include "dupa.h"

ConfigParser::ConfigParser()
{
	zeroAttributes();
}

ConfigParser::ConfigParser( QString fname )
{
	zeroAttributes();
	
	parseFile( fname );
}

/*
 * Destructor.
 */
ConfigParser::~ConfigParser()
{
	clearAttributes();
}

/*
 * Zeroes class attributes. Should be called only by
 * class contructors. For other methods use clearAttributes
 * instead.
 */
void ConfigParser::zeroAttributes()
{
	is_correct_config_file_parsed = false;
}

/*
 * Clears and zeroes class attributes.
 */
void ConfigParser::clearAttributes()
{
	zeroAttributes();
}

/*
 * Parses config file.
 */
int ConfigParser::parseFile( QString fname )
{
	xmlDoc *doc = NULL;
	xmlNode *rootElement = NULL;
	xmlNode *currentNode = NULL;
	
	/*
	 * We belive that file is correct. If it isn't, proper function
	 * will set is_correct_config_file_parsed to false.
	 */
	clearAttributes();

	is_correct_config_file_parsed = true;
	
    /*
	 * This initialize the library and check potential ABI mismatches between
	 * the version it was compiled for and the actual shared library used.
	 */
	LIBXML_TEST_VERSION

    /*
	 * Parse the file and get the DOM
	 */
	if( ( doc = xmlParseFile( fname ) ) == NULL )
	{
		is_correct_config_file_parsed = false;
		
		return false;
	}

	/*
	 * TODO: Here will be check against DTD. But first DTD
	 * neeeds to be made ;).
	 */

	/*
	 * Get root element.
	 */
	rootElement = xmlDocGetRootElement( doc );

	/*
	 * Read data and push it into proper objects.
	 */
	for( currentNode = rootElement->children; currentNode; currentNode = currentNode->next )
	{
		if( currentNode->type != XML_ELEMENT_NODE )
			continue;

		if( xmlStrEqual( currentNode->name , xmlCharStrdup( "Alphabet" ) ) )
		{
			is_correct_config_file_parsed &= parseAlphabetPart( currentNode );

			continue;
		}
		
		if( xmlStrEqual( currentNode->name , xmlCharStrdup( "Gameboard" ) ) )
		{
			is_correct_config_file_parsed &= parseGameboardPart( currentNode );

			continue;
		}
		
		if( xmlStrEqual( currentNode->name , xmlCharStrdup( "Rules" ) ) )
		{
			is_correct_config_file_parsed &= parseRulesPart( currentNode );

			continue;
		}
	}

	
    /*
	 * Free the document
	 */
    xmlFreeDoc( doc );

    /*
	 * Free the global variables that may have been allocated by the parser.
	 */
    xmlCleanupParser();

	return is_correct_config_file_parsed;
}

/*
 * Parses alphabet part of XML data file.
 */
int ConfigParser::parseAlphabetPart( xmlNode *node )
{
	/*
	 * First - parse letters.
	 */
	for( xmlNode *currentNode = node->children; currentNode; currentNode = currentNode->next )
	{
		xmlChar *name = NULL;
		xmlChar *show = NULL;
		int count = -1, value = -1;

		if( currentNode->type != XML_ELEMENT_NODE )
			continue;

		for( xmlAttr *currentAttr = currentNode->properties; currentAttr; currentAttr = currentAttr->next )
		{
			if( xmlStrEqual( currentAttr->name , xmlCharStrdup( "name" ) ) )
			{
				name = currentAttr->children->content;

				continue;
			}

			if( xmlStrEqual( currentAttr->name , xmlCharStrdup( "show" ) ) )
			{
				show = currentAttr->children->content;

				continue;
			}

			if( xmlStrEqual( currentAttr->name , xmlCharStrdup( "count" ) ) )
			{
				count = atoi( (const char *)currentAttr->children->content );

				continue;
			}

			if( xmlStrEqual( currentAttr->name , xmlCharStrdup( "value" ) ) )
			{
				value = atoi( (const char *)currentAttr->children->content );

				continue;
			}
		}

		alphabet->addAlphabetLetter( name , count , value , show );
	}

	/*
	 * Parse blank number.
	 */
	for( xmlAttr *currentAttr = node->properties; currentAttr; currentAttr = currentAttr->next )
	{
		if( xmlStrEqual( currentAttr->name , xmlCharStrdup( "number-of-blanks" ) ) )
		{
			alphabet->setNumberOfBlanks( atoi( (const char *)currentAttr->children->content ) );

			continue;
		}
	}

	return true;
}

/*
 * Parses gameboard part of XML config file.
 */
int ConfigParser::parseGameboardPart( xmlNode *node )
{
	int left = 0, top = 0, width = 15, height = 15, type = NORMAL;
	xmlAttr *currentAttr;

	
	for( xmlNode *currentNode = node->children; currentNode; currentNode = currentNode->next )
	{
		if( currentNode->type != XML_ELEMENT_NODE )
			continue;

		for( currentAttr = currentNode->properties; currentAttr; currentAttr = currentAttr->next )
		{
			if( xmlStrEqual( currentAttr->name , xmlCharStrdup( "left" ) ) )
			{
				left = atoi( (const char *)currentAttr->children->content );

				continue;
			}

			if( xmlStrEqual( currentAttr->name , xmlCharStrdup( "top" ) ) )
			{
				top = atoi( (const char *)currentAttr->children->content );

				continue;
			}
			
			if( xmlStrEqual( currentAttr->name , xmlCharStrdup( "width" ) ) )
			{
				width = atoi( (const char *)currentAttr->children->content );

				continue;
			}
			
			if( xmlStrEqual( currentAttr->name , xmlCharStrdup( "height" ) ) )
			{
				height = atoi( (const char *)currentAttr->children->content );

				continue;
			}
			
			if( xmlStrEqual( currentAttr->name , xmlCharStrdup( "type" ) ) )
			{
				if( xmlStrEqual( currentAttr->children->content , xmlCharStrdup( "empty-field" ) ) )
				{
					type = NORMAL;

					continue;
				}

				if( xmlStrEqual( currentAttr->children->content , xmlCharStrdup( "two-word-bonus" ) ) )
				{
					type = TWO_WORD_BONUS;

					continue;
				}

				if( xmlStrEqual( currentAttr->children->content , xmlCharStrdup( "three-word-bonus" ) ) )
				{
					type = THREE_WORD_BONUS;

					continue;
				}

				if( xmlStrEqual( currentAttr->children->content , xmlCharStrdup( "two-letter-bonus" ) ) )
				{
					type = TWO_LETTER_BONUS;

					continue;
				}

				if( xmlStrEqual( currentAttr->children->content , xmlCharStrdup( "three-letter-bonus" ) ) )
				{
					type = THREE_LETTER_BONUS;

					continue;
				}
				
				if( xmlStrEqual( currentAttr->children->content , xmlCharStrdup( "disabled" ) ) )
				{
					type = DISABLED_FIELD;

					continue;
				}

				continue;
			}
		}
		
		gameboard->setRegion( left , top , width , height , type );
	}

	return true;
}

/*
 * Parses rules part of XML config file.
 */
int ConfigParser::parseRulesPart( xmlNode *node )
{
	xmlAttr *ruleNameAttr = NULL, *currentAttr = NULL;
	xmlChar *ruleParameterName = NULL, *ruleParameterValue = NULL;

	for( xmlNode *currentNode = node->children; currentNode; currentNode = currentNode->next )
	{
		if( currentNode->type != XML_ELEMENT_NODE )
			continue;

		if( !xmlStrEqual( currentNode->name , xmlCharStrdup( "Rule" ) ) )
			return false;

		/*
		 * Parse single rule.
		 */

		/*
		 * First - find rule name.
		 */
		for( currentAttr = currentNode->properties; currentAttr; currentAttr = currentAttr->next )
			if( xmlStrEqual( currentAttr->name , xmlCharStrdup( "name" ) ) )
				ruleNameAttr = currentAttr;

		/*
		 * Rule for determining field for first move, if any.
		 */
		if( xmlStrEqual( ruleNameAttr->children->content , xmlCharStrdup( "start-field-required" ) ) )
		{
			int x = -1, y = -1, flexible = false;

			/*
			 * Parse rule parameters. This rule contains three parameters:
			 * x and y start field coordinates, and boolean value indicated
			 * whenever start field is flexible. Flexible start field means,
			 * that first move should go through this field. In other case
			 * first move should begin on this field.
			 */
			for( xmlNode *ruleParameterNode = currentNode->children; ruleParameterNode; ruleParameterNode = ruleParameterNode->next )
			{
				if( ruleParameterNode->type != XML_ELEMENT_NODE )
					continue;

				if( !xmlStrEqual( ruleParameterNode->name , xmlCharStrdup( "RuleParameter" ) ) )
					return false;

				for( currentAttr = ruleParameterNode->properties; currentAttr; currentAttr = currentAttr->next )
				{
					if( xmlStrEqual( currentAttr->name , xmlCharStrdup( "name" ) ) )
					{
						ruleParameterName = currentAttr->children->content;

						continue;
					}
					
					if( xmlStrEqual( currentAttr->name , xmlCharStrdup( "value" ) ) )
					{
						ruleParameterValue = currentAttr->children->content;

						continue;
					}
				}

				if( xmlStrEqual( ruleParameterName , xmlCharStrdup( "start-field-x-position" ) ) )
				{
					x = atoi( (const char *)ruleParameterValue );

					continue;
				}
				
				if( xmlStrEqual( ruleParameterName , xmlCharStrdup( "start-field-y-position" ) ) )
				{
					y = atoi( (const char *)ruleParameterValue );

					continue;
				}
				
				if( xmlStrEqual( ruleParameterName , xmlCharStrdup( "start-field-flexible" ) ) )
				{
					flexible = xmlStrEqual( ruleParameterValue , xmlCharStrdup( "yes" ) );

					continue;
				}
			}

			if( x == -1 || y == -1 )
				return false;

			((FirstMoveRule *)(rules->getRule( RULE_START_FIELD )))->setFirstMoveField( x , y , flexible );
		}
		
		/*
		 * Rule for determining end game coditions.
		 */
		if( xmlStrEqual( ruleNameAttr->children->content , xmlCharStrdup( "end-game-conditions" ) ) )
		{
			int endWithFullGameboard = false, continuousPauses = 0;

			/*
			 * Parse rule parameters. This rule contains two parameters:
			 * boolead value full-gameboard indicating if game should end when
			 * entire gamboard if full and continuous-pauses, which contains
			 * number of continuous pauses during game needed to break the game.
			 */
			for( xmlNode *ruleParameterNode = currentNode->children; ruleParameterNode; ruleParameterNode = ruleParameterNode->next )
			{
				if( ruleParameterNode->type != XML_ELEMENT_NODE )
					continue;

				if( !xmlStrEqual( ruleParameterNode->name , xmlCharStrdup( "RuleParameter" ) ) )
					return false;

				for( currentAttr = ruleParameterNode->properties; currentAttr; currentAttr = currentAttr->next )
				{
					if( xmlStrEqual( currentAttr->name , xmlCharStrdup( "name" ) ) )
					{
						ruleParameterName = currentAttr->children->content;

						continue;
					}
					
					if( xmlStrEqual( currentAttr->name , xmlCharStrdup( "value" ) ) )
					{
						ruleParameterValue = currentAttr->children->content;

						continue;
					}
				}

				if( xmlStrEqual( ruleParameterName , xmlCharStrdup( "full-gameboard" ) ) )
				{
					endWithFullGameboard = xmlStrEqual( ruleParameterValue , xmlCharStrdup( "yes" ) );

					continue;
				}
				
				if( xmlStrEqual( ruleParameterName , xmlCharStrdup( "continuous-pauses" ) ) )
				{
					continuousPauses = atoi( (const char *)ruleParameterValue );

					continue;
				}
			}

			((EndGameConditionsRule *)(rules->getRule( RULE_END_GAME_CODITIONS )))->setEndGameConditions( endWithFullGameboard , continuousPauses );
		}
		
	}
	
	return true;
}

/*
 * Counts number of node's children.
 *
 * Simple function with functionality, which I couldn't found
 * in libxml.
 */
int ConfigParser::countNodeChildren( xmlNode *node )
{
	int i = 0;

	for( xmlNode *currentNode = node->children; currentNode; currentNode = currentNode->next )
		i++;

	return i;
}


