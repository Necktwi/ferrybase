base
====

C++ linux library that provides FFJSON parser and contains other common functions used in my other projects

FFJSON JSON parser
==================
I doubt a parser could be any faster!

Intention to develop this parser is to provide ability to access name value pairs in a JSON string using '[]' operator in a C++ program.

Its a valgrind clean library. One don't have to worry about freeing up parser.

E.g.:
-----
	#include <base/FFJSON.cpp>

	FFJSON obj("{\"name\":\"Gowtham\",\"id\":1729,\"isProgrammer\":true,\"favLanguages\":[\"C++\",\"javascript\"]}");
	cout << obj["name"] << endl;
	cout << (int)obj["id"] << endl;
	if(obj["isProgrammer"])
		cout << obj["favLanguages"].prettyString() << endl;

output:
-------
	Gowtham
	1729
	[
		"C++",
		"javascript",
	]

Installation
------------
	$ git clone https://github.com/necktwi/base.git
	$ cd base
	$ make
	$ sudo make install

Linker option
--------------
	-lbase
	
It got lot many cool features; look https://github.com/necktwi/base/blob/master/nbproject/tests/ffjsonTest.cpp.

FFJSON acronym is "FerryFairJSON"
