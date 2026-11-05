/*
 * File:	StrPool.h
 * Author:	Gowtham Kudupudi
 */

#ifndef STRPOOL_H
#define STRPOOL_H

#include <cstddef>
#include <unordered_set>
#include <vector>
#include <string>

typedef const char* ccp;

class StrPool_ {
	class Arena_ {
		char* _data;
		size_t _size;
		size_t _used;

	public:
		Arena_ (size_t size);
		~Arena_ ();

		bool hasSpace (size_t size) const;
		char* allocate (size_t size);
	};

	struct Hash_ {
		size_t operator () (ccp string) const;
	};

	struct Equal_ {
		bool operator () (ccp a, ccp b) const;
	};

	std::vector<Arena_*> _arenas;
	std::unordered_set<ccp, Hash_, Equal_> _strings;

	size_t _initialArenaSize;
	size_t _nextArenaSize;

	char* allocate (size_t size);

public:
	StrPool_ (size_t arenaSize = 4096);
	~StrPool_ ();

	ccp ccpFromPool (ccp string);
	ccp ccpIfInPool (ccp string);
	void clear ();
};

extern StrPool_ StrPool;

ccp ccpFromPool (ccp string);
ccp ccpFromPool (const std::string& string);
ccp ccpIfInPool (ccp string);
ccp ccpIfInPool (const std::string& string);
#endif
