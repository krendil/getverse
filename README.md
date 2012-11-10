getverse
========

A unix-style command line front-end to the SWORD engine.

dependencies
------------

 - SWORD (preferably from svn, the latest release sometimes has issues)

usage
-----

Usage:	getverse -t <translation> [-f format] [<reference>]
		getverse [-h | -?]
Options:
	-t	A Bible translation you have installed in SWORD (e.g. KJV)
	-f	A format string that will determine the format of the output
		 Defaults to '%p'
	-h
	-?	Display this message and exit
Format variables:
	%p	The passage text
	%t	The translation name
	%r	The supplied reference
	%%	A literal % character

If no reference is supplied, getverse tries to read from stdin.

notes
-----

 -	SWORD is pretty lax about what constitutes a verse reference. For instance,
	Psalm 151:1 is interpreted as Proverbs 1:1.
   
