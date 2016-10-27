#include "myconverters.h"
#include<string>
#include<sstream>
#include<vector>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<stdio.h>

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

#ifdef __CYGWIN__
template < typename T > std::string to_string( const T& n )
{
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}
int stoi(const string& s, size_t* t){
    char* pEndPtr = NULL;
    int iResult = (int)strtol(s.c_str(), &pEndPtr,10);
    (*t) = pEndPtr-s.c_str();
    return iResult;
}
double stod(const string& s, size_t* t){
    char* pEndPtr = NULL;
    double iResult = (double)strtol(s.c_str(), &pEndPtr,10);
    if(t)(*t) = pEndPtr-s.c_str();
    return iResult;
}
#endif

static char encoding_table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};
static char* decoding_table = NULL;
static int mod_table[] = {0, 2, 1};

char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length) {
    
    *output_length = 4 * ((input_length + 2) / 3);
    
    char *encoded_data = (char*) malloc(*output_length);
    if (encoded_data == NULL) return NULL;
    
    for (int i = 0, j = 0; i < input_length;) {
        
        uint32_t octet_a = i < input_length ? data[i++] : 0;
        uint32_t octet_b = i < input_length ? data[i++] : 0;
        uint32_t octet_c = i < input_length ? data[i++] : 0;
        
        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
        
        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }
    
    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';
    
    return encoded_data;
}

unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length) {
    
    if (decoding_table == NULL) build_decoding_table();
    
    if (input_length % 4 != 0) return NULL;
    
    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;
    
    unsigned char *decoded_data = (unsigned char *) malloc(*output_length);
    if (decoded_data == NULL) return NULL;
    
    for (int i = 0, j = 0; i < input_length;) {
        
        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        
        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);
        
        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }
    
    return decoded_data;
}

void build_decoding_table() {
    
    decoding_table = (char*) malloc(256);
    
    for (int i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}

void base64_cleanup() {
    free(decoding_table);
}
