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

#endif	/* MYCONVERTERS_H */

