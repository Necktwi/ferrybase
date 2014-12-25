#include "myconverters.h"
#include<string>
#include<sstream>
#include<vector>
#include<stdlib.h>
#include<string.h>
#include<iostream>

using namespace std;

/*int atoi(const char* str) {
	int num = 0;
	char digit;
	while ((digit = *str++) != '\0') {
		if (digit < '0' || digit > '9') {
			return num; // No valid conversion possible 
		}
		num *= 10;
		num += digit - '0';
	}
	return num;
}*/

/*float atof(const char* str) {
	float num = 0;
	char digit;
	bool f = false;
	int dc = 0;
	while ((digit = *str++) != '\0') {
		if (digit < '0' || digit > '9') {
			if (digit == '.' && !f) {
				f = true;
			} else {
				return num; // No valid conversion possible 
			}
		} else if (digit != '0') {

		}
		if (!f) {
			num *= 10;
			num += digit - '0';
		} else {
			dc++;
			num += (digit - '0') / (10^dc);
		}
	}
	return num;
}*/

std::string itoa(int i, int size) {
	std::stringstream ss;
	std::string out;
	ss << i;
	ss >> out;
	if (size > 0) {
		int sl = out.length();
		int additionalZeroes = size - sl;
		while (additionalZeroes > 0) {
			out = "0" + out;
			additionalZeroes--;
		}
	}
	return out;
}

std::string implode(const std::string glue, const std::vector<std::string> &pieces) {
	std::string a;
	int leng = pieces.size();
	for (int i = 0; i < leng; i++) {
		a += pieces[i];
		if (i < (leng - 1))
			a += glue;
	}
	return a;
}

vector<splitstring>& splitstring::split(char delim, char dum, int rep) {
	if (!flds.empty()) flds.clear(); // empty vector if necessary
	string& work = *this;
	string buf = "";
	int i = 0;
	while (i < work.length()) {
		if (work[i] != delim)
			buf += work[i];
		else if (rep == 1) {
			flds.push_back(buf);
			buf = "";
		} else if (buf.length() > 0) {
			flds.push_back(buf);
			buf = "";
		}
		i++;
	}
	if (!buf.empty())
		flds.push_back(buf);
	return flds;
}

std::vector<std::string> explode(const std::string delimiter, const std::string &str) {
	std::vector<std::string> arr;
	explode(delimiter, str, arr);
	return arr;
}

void explode(const std::string delimiter, const std::string &str, std::vector<std::string>& shrapnel) {
	int strleng = str.length();
	int delleng = delimiter.length();
	if (delleng == 0)
		return; //no change

	int i = 0;
	int k = 0;
	while (i < strleng) {
		int j = 0;
		while (i + j < strleng && j < delleng && str[i + j] == delimiter[j])
			j++;
		if (j == delleng)//found delimiter
		{
			shrapnel.push_back(str.substr(k, i - k));
			i += delleng;
			k = i;
		} else {
			i++;
		}
	}
	shrapnel.push_back(str.substr(k, i - k));
	return;
}

float timeToSec(std::string timestring) {
	float secs = 0;
	std::vector<std::string> t = explode(":", timestring);
	secs = atoi(t[0].c_str())*60 * 60 + atoi(t[1].c_str())*60 + atof(t[2].c_str());
	return secs;
}

std::string tolower(std::string s) {
	char* buf;
	buf = (char*) s.c_str();
	int i;
	for (int i = 0; i < s.length(); i++) {
		buf[i] = tolower(buf[i]);
	}
	return std::string(buf);
}

void chr_cstrlit(unsigned char u, char *buffer, size_t buflen) {
	if (buflen < 2)
		*buffer = '\0';
	else if (isprint(u) && u != '\"' && u != '\\')
		sprintf(buffer, "%c", u);
	else if (buflen < 3)
		*buffer = '\0';
	else {
		switch (u) {
			case '\n': strcpy(buffer, "\\n");
				break;
			case '\r': strcpy(buffer, "\\r");
				break;
			case '\t': strcpy(buffer, "\\t");
				break;
			case '\\': strcpy(buffer, "\\\\");
				break;
			case '\"': strcpy(buffer, "\\\"");
				break;
			default:
				if (buflen < 5)
					*buffer = '\0';
				else
					sprintf(buffer, "\\%03o", u);
				break;
		}
	}
}

void str_cstrlit(const char *str, char *buffer, size_t buflen) {
	unsigned char u;
	size_t len;

	while ((u = (unsigned char) *str++) != '\0') {
		chr_cstrlit(u, buffer, buflen);
		if ((len = strlen(buffer)) == 0)
			return;
		buffer += len;
		buflen -= len;
	}
}