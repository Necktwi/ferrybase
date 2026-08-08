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
#include <cstdint>
#include "mystdlib.h"
//int atoi(const char* str);
//float atof(const char* str);

using namespace std;
typedef const char* ccp;
static ccp NullCcp= "";

std::string itoa(int i, int size = 0);
std::string implode(const std::string glue,
		const std::vector<std::string> &pieces);
std::vector<std::string> explode(
   const std::string &str, const std::string delimiter = " ");
void explode(const std::string delimiter, const std::string &str,
				 std::vector<std::string>& shrapnel);
std::vector<std::string> explodeByNum (const std::string& str);

float timeToSec(std::string timestring);
int tolower (ccp s);
int tolower (string& s);
void str_cstrlit(const char *str, char *buffer, size_t buflen);

class splitstring : public std::string {
	std::vector<std::string> flds;
public:

	// splitstring (const string& str) : fstr(str) {
	// };

	// splitstring (const string& str, size_t pos, size_t len = npos) :
   //    fstr(str, pos, len)
   // {};

	// splitstring(const char* s) : fstr(s) {
	// };

	// splitstring(const char* s, size_t n) : fstr(s, n) {
	// };

	// splitstring(size_t n, char c) : fstr(n, c) {
	// };

   // inherit all std::string constructors (C++11+)
   using std::string::string;

   // inherit assignment operators
   using std::string::operator=;

   splitstring() = default;
   splitstring(const splitstring&) = default;
   splitstring(splitstring&&) noexcept = default;
   ~splitstring() = default;

   /** split: receives a char delimiter; returns a vector of strings
	 * By default ignores repeated delimiters, unless argument rep == 1.
	 **/
	std::vector<std::string>& split(char delim, char dum, int rep = 1);
};

#ifdef __CYGWIN__
template < typename T > std::string to_string(const T& n);
int stoi(const string& s, size_t* t = NULL);
double stod(const string& s, size_t* t = NULL);
#endif
/**
 * returns a string of base64 encoded data
 * @param data:input string
 * @param input_length: the length of the input string
 * @param output_length: pointer to the variable that gets length of the output
 * string.
 * @return pointer to the string encoded in base65 and should be freed using
 * free() by the calling function.
 */
char* base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length);

/**
 * @param data : input base64 encoded string
 * @param input_length : length of the @param data
 * @param output_length : pointer to the variable that gets filled with length
 * returned binary value.
 * @return binary block; should be freed by calling function using free().
 */
unsigned char* base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length);
void base64_cleanup();
void build_decoding_table();

int decodeURIComponent (string& str);

int8_t countSetBits (unsigned int n);
void reduceImg (const char* imgPath);
#endif	/* MYCONVERTERS_H */

