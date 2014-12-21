/* 
 * File:   FFJSON.h
 * Author: Satya Gowtham Kudupudi
 *
 * Created on November 29, 2013, 4:29 PM
 */

#ifndef FFJSON_H
#define	FFJSON_H

#include "myconverters.h"

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <exception>
#include <stdint.h>

class FFJSON {
public:

	class Exception : std::exception {
	public:

		Exception(std::string e) : identifier(e) {

		}

		const char* what() const throw () {
			return this->identifier.c_str();
		}

		~Exception() throw () {
		}
	private:
		std::string identifier;
	};

	enum OBJ_TYPE {
		UNDEFINED,
		STRING,
		XML,
		NUMBER,
		BOOL,
		OBJECT,
		ARRAY,
		LINK,
		NUL
	};

	enum QUERY_TYPE {
		/**
		 * To clear query type
		 */
		NONE,
		QUERY,
		SET,
		DELETE,
	};

	enum E_FLAGS {
		ENONE = 0,
		B64ENCODE = 1 << 0,
		B64ENCODE_CHILDREN = 1 << 1,
		B64ENCODE_STOP = 1 << 2,
		IS_COMMENT = 1 << 3,
		HAS_COMMENT = 1 << 4,
		IS_EXTENDED = 1 << 5
	};

	enum COPY_FLAGS {
		COPY_NONE = 0,
		COPY_QUERIES = 1 << 0,
		COPY_EFLAGS = 1 << 1
	};

	static const char OBJ_STR[8][15];

	static std::map<std::string, uint8_t> STR_OBJ;

	class Iterator {
	public:
		Iterator();
		Iterator(const Iterator& orig);
		Iterator(const FFJSON& orig, bool end = false);
		virtual ~Iterator();
		void init(const FFJSON& orig, bool end = false);
		Iterator& operator++();
		Iterator operator++(int);
		Iterator& operator--();
		Iterator operator--(int);
		Iterator& operator=(const Iterator& i);
		bool operator==(const Iterator& i);
		bool operator!=(const Iterator& i);
		FFJSON* operator->();
		FFJSON& operator*();
		operator const char*();

	private:
		uint8_t type;
		void copy(const Iterator& i);

		union {
			std::map<std::string, FFJSON*>::iterator* pi;
			std::vector<FFJSON*>::iterator* ai;
		} ui;
	};
	
	struct FFJSONExt{
		FFJSON* base = NULL;
	};

	struct FFJSONPObj {
		std::string* name;
		FFJSON* value = NULL;
		FFJSONPObj* pObj = NULL;
	};

	/**
	 * creates an UNRECOGNIZED FFJSON object. Any FFJSON object can be
	 * assigned any other type of FFJSON object.
	 */
	FFJSON();

	//FFJSON(std::ifstream file);
	/**
	 * Copy constructor. Creates a copy of FFJSON object
	 * @param orig is the object one wants to create a copy
	 */
	FFJSON(const FFJSON& orig, COPY_FLAGS cf = COPY_NONE);

	/**
	 * Creates a FFJSON object from a FFJSON string.
	 * @param ffjson is the FFJSON string to be parsed.
	 * @param ci is the offset in FFJSON string to be considered. Its 0 by 
	 * default.
	 */
	FFJSON(const std::string& ffjson, int* ci = NULL, int indent = 0,
			FFJSONPObj* pObj = NULL);
	void init(const std::string& ffjson, int* ci = NULL, int indent = 0,
			FFJSONPObj* pObj = NULL);

	/**
	 * Creates an empty FFJSON object of type @param t. It throws an Exception
	 * if @param t is UNRECOGNIZED or anything else other FFJSON_OBJ_TYPE
	 * @param t
	 */
	FFJSON(OBJ_TYPE t);

	~FFJSON();

	/**
	 * Emptys the FFJSON object. For example If you want delete objects in an 
	 * array or an object, invoke it.
	 */
	void freeObj();

	/**
	 * It holds the size of the FFJSON object. array size, object properties, string
	 * length. Do not change it!! Its made public only for reading convenience.
	 */
	unsigned int size = 0;

	/**
	 * If the object is of type @param t, it returns true else false.
	 * @param t : type to check
	 * @return true if type matched
	 */
	bool isType(uint8_t t) const;

	/**
	 * Sets type of the object to t
	 * @param t
	 */
	void setType(uint8_t t);

	uint8_t getType() const;

	bool isQType(uint8_t t) const;

	void setQType(uint8_t t);

	uint8_t getQType() const;

	bool isEFlagSet(uint8_t t) const;

	void setEFlag(uint8_t t);

	uint8_t getEFlags() const;

	void clearEFlag(uint8_t t);

	/**
	 * Removes leading and trailing white spaces; sapces and tabs from a string.
	 * @param s
	 */
	static void trimWhites(std::string& s);

	/**
	 * Removes leading and trailing quotes in a string.
	 * @param s
	 */
	static void trimQuotes(std::string& s);

	/**
	 * Trying to read an object property that doesn't exist creates the property
	 * with unrecognized object.
	 * @param f
	 */
	void trim();

	/**
	 * Gives FFJSON object type of FFJSON string.
	 * @param ffjson is the FFJSON string.
	 * @return FFJSON object type.
	 */
	OBJ_TYPE objectType(std::string ffjson);

	/**
	 * Converts FFJSON object into FFJSON string.
	 * @return FFJSON string.
	 */
	std::string stringify(bool json = false);

	/**
	 * Converts FFJSON object into FFJSON pretty string that has indents where
	 * they needed
	 * @param indent : Dont bother about it! Its 0 by default which you need. If
	 * you insist, it prepends its value number of indents to the output. To get
	 * an idea on what I'm saying, jst try it with non zero positive value.
	 * @return A pretty string :)
	 */
	std::string prettyString(bool json = false, bool printComments = false, unsigned int indent = 0);

	/**
	 * Generates a query string which can be used to query a FFJSON tree. Query 
	 * string is constructed based on SET, QUERY and DELETE marks on the FFJSON
	 * objects.
	 * E.g.
	 * {animals:{horses:{count:?,colors:?}}} can be used to query horses count 
	 * and colors available!
	 * @return Query string.
	 */
	std::string queryString();

	/**
	 * Generates an answer string for a query object from the FFJSON tree. Query
	 * object is the FFJSON object returned by FFJSON(queryString).
	 * E.g.
	 * {animals:{horses:{count:35,colors:["white","black","brown"]}}}
	 * can be the answer string for 
	 * {animals:{horses:{count:?,colors:?}}}
	 * query string.
	 * @param queryStirng
	 * @return Answer string
	 */
	FFJSON* answerString(FFJSON& queryObject);

	FFJSON* answerObject(FFJSON* queryObject);

	void erase(std::string name);

	void erase(int index);

	void erase(FFJSON* value);

	Iterator begin();

	Iterator end();

	union FFValue {
		char * string;
		std::vector<FFJSON*>* array;
		std::map<std::string, FFJSON*>* pairs;
		double number;
		bool boolean;
		FFJSON* fptr;
	} val;

	FFJSON& operator[](const char* prop);
	FFJSON& operator[](std::string prop);
	FFJSON& operator[](int index);
	FFJSON& operator=(const std::string& s);
	FFJSON& operator=(const int& i);
	FFJSON& operator=(const unsigned int& i);
	FFJSON& operator=(const double& d);
	FFJSON& operator=(const float& f);
	FFJSON& operator=(const short& s);
	FFJSON& operator=(const long& l);
	FFJSON& operator=(const FFJSON& f);
	FFJSON& operator=(const FFJSON* f);
	operator const char*();
	operator double();
	operator float();
	operator bool();
	operator int();
	operator unsigned int();

private:

	/**
	 * It holds the type of the FFJSON object.
	 */
	uint8_t type = UNDEFINED;
	uint8_t qtype;
	uint8_t etype;
	void* extPtr;
	void copy(const FFJSON& orig, COPY_FLAGS cf = COPY_NONE);
	static int getIndent(const char* ffjson, int* ci, int indent);
	static void strObjMapInit();
	static bool inline isWhiteSpace(char c);
	static bool inline isTerminatingChar(char c);
	static FFJSON* returnNameIfDeclared(splitstring name, FFJSONPObj* fpo);
};

#endif	/* FFJSON_H */
