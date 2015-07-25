/* 
 * File:   myconverters.h
 * Author: satya gowtham kudupudi
 *
 * Created on 22 March, 2013, 11:52 AM
 */

#ifndef MYCONVERTERS_H
#define	MYCONVERTERS_H
#include <string>
#include <vector>

//int atoi(const char* str);
//float atof(const char* str);

using namespace std;

std::string itoa(int i, int size = 0);
std::string implode(const std::string glue,
		const std::vector<std::string> &pieces);
std::vector<std::string> explode(const std::string delimiter,
		const std::string &str);
void explode(const std::string delimiter, const std::string &str,
		std::vector<std::string>& shrapnel);
float timeToSec(std::string timestring);
std::string tolower(std::string s);
void str_cstrlit(const char *str, char *buffer, size_t buflen);

class splitstring : public string {
	std::vector<splitstring> flds;
public:

	splitstring(const string& str) : string(str) {
	};

	splitstring(const string& str, size_t pos, size_t len = npos) : string(str, pos, len) {
	};

	splitstring(const char* s) : string(s) {
	};

	splitstring(const char* s, size_t n) : string(s, n) {
	};

	splitstring(size_t n, char c) : string(n, c) {
	};

	/** split: receives a char delimiter; returns a vector of strings
	 * By default ignores repeated delimiters, unless argument rep == 1.
	 **/
	std::vector<splitstring>& split(char delim, char dum, int rep = 1);
};

#ifdef __CYGWIN__
template < typename T > std::string to_string(const T& n);
int stoi(const string& s, size_t* t = NULL);
double stod(const string& s, size_t* t = NULL);
#endif

#endif	/* MYCONVERTERS_H */

