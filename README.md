Sakreble! - an open word game


Contents: _____________________________________________________________________

I. Installation.
I.1. Requirements.
I.2. Compilation.
I.3. Installation.

II. Running.
II.1. Default running method.
II.2. Dictionary.
II.3. File with players data.
II.4. File with game config.
II.4.1. Alphabet syntax.
II.4.2. Gameboard syntax.
II.4.3. Rules syntax.

III. Credits


I. Installation. ______________________________________________________________

I.1. Requirements. ............................................................

* Qt library, version >= 3.0
* libxml library, version >= 2.6

Makefile searches for proper header files in /usr/include/qt/ and
/usr/include/libxml2/ directories, you may change it for your system however
(eg.  /usr/local/include/qt/).

* for sakreble-dictionary.py utility you need Python >= 2.3 installed.


I.2. Compilation. .............................................................

Just type make in src/ directory.


I.3. Installation. ............................................................

Soon... Now just type ./Sakreble in src/ directory or copy executable into your
bin path.


II. Running. __________________________________________________________________

II.1. Default running method. .................................................

Type...

$ Sakreble --config-file=path-to-game-config-file

...and enjoy!


II.2. Dictionary. .............................................................

Sakreble! requires a dictionary file. Default Sakreble! searches for dictionary
file in directory where it is running. It could be changed by inserting valid
path in command line option --directory-path (see usage).

You can create your own dictionary file with sakreble-dictionary.py utility.
It can be found in utils/ directory. Input for this utility is a file with
words, one per line. Output is a valid Sakreble! dictionary file (it is a
binary file with word letters replaced with small integers, sorted primarily by
word length and secondarily in alphabetical order).

sakreble-dictionary.py utility needs game config file provided by option
--config-file (see section II.4 for info about game config file). This file is
used for determining game's alphabet. The same alphabet should be used while
playing Sakreble! .


II.3. File with players data. .................................................

Sakreble! stores info about players and their games in a single file. Name for
this file can be set with command line option player-data-path (see usage).


II.4. File with game config. ..................................................

Sakreble! is (or will be ;) fully configurable. Since version 0.75 all info
about game rules is stored in a special XML file. At the moment I prepared only
two example files, you'll find them in examples directory.

Game config file has three general sections: Alphabet, Gameboard and Rules.
Alphabet section is used to define letter sets. Gameboard section helps by
defining gameboard (size, shape, fields, etc.). Rules section is intended to be
a powerful game rules configuring system, making most game rules configurable
by config file creator.

Game config file path is a Sakreble!'s command line option. You may set it by
typing proper path for --config-file option. Default is ./sakreble-config.

XML file needs DTD to be written, so if you'll prepare wrong file, Sakreble!
may crash.


II.4.1 Alphabet syntax. . . . . . . . . . . . . . . . . . . . . . . . . . . . . 

It is possible to define both game letters and blanks in the alphabet section. 

Number of blanks is defined as a parameter of Alphabet tag in game config file.
Parameter's name is number-of-blanks and it is required. Valid values are
natural numbers.

Each letter in config file is indicated by one XML tag Letter. Syntax for this
tag:

<Letter name="a" count="2" value="5" show="A" />

where 'name' is one-letter string with letter name. 'count' indicates number of
letters with this name in sack during game beginning. 'value' is point value
for this letter. 'show' is a string which will be shown on a gameboard during
game.  'show' is optional - if not present, 'name' will be used.

Notice that Alphabet part is required by sakreble-dictionary.py utility for
determining letters belonging to dictionary. So defining letters with count
zero could help with parsing the entire dictionary, even if such letters aren't
required during game. Second reason for letters with count zero is a game
situation where user changes his blank for a letter. He can choose every
letter, even that which is unavailable in starting sack.


II.4.2. Gameboard syntax. . . . . . . . . . . . . . . . . . . . . . . . . . . . 

Gameboard is defined by regions. Each region is defined by a rectangle with
dimensions from proper XML tag FieldRegion. In default, all gameboard fields
are marked as disabled - players can't place their letters on this fields.
Each region changes field type for fields that it defines. Syntax for
FieldRegion tag is as follows:

<FieldRegion left="0" top="0" width="15" height="15" type="empty-field" />

where 'left' and 'top' both indicate top-left corner of a region. 'width' and
'height' define region size (in fields). Currently 'left' and 'top' must be in
range [0,14]. 'width' and 'height' can't exceed gameboard dimensions (15x15).
'type' is a field type - region will be filled with this field type.

Valid field types are:
* 'empty-field' - normal field, no bonus,
* 'disabled' - disabled field,
* 'two-letter-bonus' - field with two letter bonus,
* 'three-letter-bonus' - field with three letter bonus,
* 'two-word-bonus' - field with two word bonus,
* 'three-word-bonus' - field with three word bonus.


II.4.3. Rules syntax. . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Each rule in game config file has its own name, defining rule behaviour. Syntax
for single game rule is as follows:

<Rules name="game-rule-name">
	<RuleParameter name="par0" value="val0">
	<RuleParameter name="par1" value="val1">
	...
	<RuleParameter name="parn" value="valn">
</Rule>

RuleParameter tags describe configuration options for rule indicated by name
given in Rules tag "name" parameter. These options are rule-dependent. All
currently available game rules are explained below and possible parameter
values are described.

Rule name: start-field-required
  Description: this rule is used to define game start field; game start field
               is a gameboard field, where a letter must be placed during first
               game move; if this rule isn't present in config file, there are
               no requirements for first move - player can start game anywhere
               on the gameboard

  Parameters: start-field-x-position (numeric), start-field-y-position (numeric),
               start-field-flexible (boolean)

  start-field-x-position: x coordinate (column) for first move field;
               column coordinates are counted from zero from left edge of the
               gameboard
	
  start-field-y-position: y coordinate (row) for first move field; row
               coordinates are counted from zero from top edge of the gameboard

  start-field-flexible: this parameter defines whenever start field is flexible;
               flexible start field means that first move should only
               include this field; setting this parameter value to "no" means
               that first move should start exactly in this field

Rule name: end-game-coditions
  Description: this rule is uses to define conditions under which game ends; if
               this isn't present in config file, games ends when sack is empyt
               and one of players has no letters more

  Parameters: full-gameboard(boolean), continuous-pauses(numeric)

  full-gameboard: boolean value indicating whenever game should be ended after
               filling entire gameboard with letters

  continuous-pauses: numeric value indicating number of continuous pauses during
               game needed to end game; if this parameter isn't present or value
               is set to 0 (zero), game will not end after any number of
			   continuous pauses


III. Credits

The algorithm for quickly finding all anagrams for a given shelf of letters has proudly been used by the following websites:
* https://szarada.net -- a portal for enthusiasts of solving crosswords
* https://wikicrosswords.org -- an online dictionary of clues and answers for crosswords in 30+ languages
