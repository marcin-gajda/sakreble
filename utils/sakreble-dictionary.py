#!/usr/bin/python
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

# Simple script to work with Sakreble! word dictionaries.

import sys, getopt, locale, xml.dom.minidom

class SakrebleDictionary:
	"""Dictionary class"""

	def __init__( self ):
		"""Initializes dictionary with empty dictionary.
		
		_wordlist is dictionary of lists, where keys are
		word lengths."""

		self._wordlist = {}
		self._withBigLetters = False
		self._withProgressBar = True
		self._configFile = "./sakreble-config"
		self._whenLetterNotInAlphabet = "skip"
	
	def fromFile( self , fileName ):
		"""Adds words from file."""

		if self._withProgressBar:
			counter = 10000L
			word_count = 0L
		
		for word in file( fileName ):
			word = word.strip()

			if len( word ) > 0:
				if len( word ) not in self._wordlist.keys():
					self._wordlist[ len( word ) ] = []
		
				if self._withProgressBar:
					counter = counter - 1
					word_count = word_count + 1

					if counter == 0:
						print "%d..." %  word_count

						counter = 10000L

				self._wordlist[ len( word ) ].append( word )

		if self._withProgressBar:
			self._word_count = word_count



	def dump( self , fileName ):
		"""Dumps dictionary into file.
		
		All logic is here. Words are sorted and duplicates
		removed. Word's letters are replaced with order numbers
		from xml file (first letter in xml file gets 0, second 1,
		and so on)."""

		#
		# Parsing game config file
		#
		try:
			dom = xml.dom.minidom.parse( self._configFile )
		except:
			print "Error: bad xml file."

			sys.exit(2)

		#
		# Determining the alphabet and letter mapping. Remember
		# that zero is reserved for blank, one for empty field
		# and two for field below the game.
		#
		letters = [ node.getAttributeNode( "name" ).nodeValue for node in dom.getElementsByTagName( "Letter" ) ]
		letterMapping = {}
		for i in xrange( len(letters) ):
			letterMapping[ letters[i] ] = (i+3 < 10 and [i+3] or [i+4])[0]


		legths = self._wordlist.keys()
		legths.sort()

		f = open( fileName , "w" )
		prev = None

		if self._withProgressBar:
			counter = 10000L
			word_count = self._word_count
		
		for i in legths:
			self._wordlist[i].sort()

			for word in self._wordlist[i]:
				if self._withProgressBar:
					counter = counter - 1
					word_count = word_count - 1

					if counter == 0:
						print "%d..." %  word_count

						counter = 10000L

				if prev == word:
					continue

				if not self._withBigLetters and word[0].isupper():
					continue
			
				try:
					f.write( "".join( [ chr( letterMapping[c] ) for c in unicode( word ) ] ) + "\n" )
				except KeyError:
					print "Letter from word %s not present in config file." % word,

					if self._whenLetterNotInAlphabet == "skip":
						print " Skipping..."
					else:
						print

						sys.exit(2)

				prev = word


	def setWithBigLetters( self , value ):
		"""Sets _withBigLetters."""

		self._withBigLetters = value
	
	def setWithProgressBar( self , value ):
		"""Sets _withProgressBar."""

		self._withProgressBar = value
	
	def setConfigFile( self , file ):
		"""Sets _configFile."""

		self._configFile = file
	
	def setWhenLetterNotInAlphabet( self , value ):
		"""Sets _whenLetterNotInAlphabet."""

		self._whenLetterNotInAlphabet = value



def show_usage():
	print """Usage:

$ sakreble-dictionary [options] infile outfile

where options may be:

--with-big-letters            - don't remove capitalized words
--for-locale=locale           - use specified locales for capitalization
                                determining
--without-progress-bar        - don't show progress bar
--config-file                 - game config file (for alphabet,
                                ./sakreble-config defaults)
--when-letter-not-in-alphabet - action performed, when programm detects letter
				in dictionary, which is not present in alphabet
				parsed from config file,
				valid values are:
				* skip - skip this word and continue processing
				* exit - exit programm with error"""


def main():
	"""Main function."""
	
	# Parse options
	try:
		opts, args = getopt.getopt( sys.argv[1:] , "" , [ "with-big-letters" , "for-locale=" , "without-progress-bar" , "config-file=" , "when-letter-not-in-alphabet=" ] )
	except getopt.GetoptError:
		show_usage()

		sys.exit(2)

	# We need exactly two arguments
	if len( args ) != 2:
		show_usage()

		sys.exit(2)



	slownik = SakrebleDictionary()

	for option, argument in opts:
		if option == "--with-big-letters":
			slownik.setWithBigLetters( true )
			continue
		
		if option == "--for-locale":
			locale.setlocale( locale.LC_ALL , argument )
			continue

		if option == "--without-progress-bar":
			slownik.setWithProgressBar( False )
			continue

		if option == "--config-file":
			slownik.setConfigFile( argument )

		if option == "--when-letter-not-in-alphabet":
			if argument not in [ "skip" , "exit" ]:
				show_usage()

				sys.exit(2)
			else:
				slownik.setWhenLetterNotInAlphabet( argument )


	slownik.fromFile( args[0] )
	slownik.dump( args[1] )

	sys.exit(0)


if __name__ == "__main__":
	main()

