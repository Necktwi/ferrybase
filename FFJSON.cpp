/* 
 * File:   FFJSON.cpp
 * Author: raheem
 * 
 * Created on November 29, 2013, 4:29 PM
 */

#include <string>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

#include "FFJSON.h"
#include "mystdlib.h"
#include "myconverters.h"
#include "logger.h"

FFJSON::FFJSON() {
}

FFJSON::FFJSON(std::string& ffjson, int* ci) {
    int i = (ci == NULL) ? 0 : *ci;
    int j = ffjson.length();
    this->type = UNRECOGNIZED;
    while (i < j) {
        if (ffjson[i] == '{') {
            this->type = OBJECT;
            this->val.pairs = new std::map<std::string, FFJSON*>();
            i++;
            int objIdNail = i;
            std::string objId;
            while (i < j) {
                if (ffjson[i] == ':') {
                    i++;
                    try {
                        objId = ffjson.substr(objIdNail, i - objIdNail - 1);
                        trimWhites(objId);
                        trimQuotes(objId);
                        FFJSON* obj = new FFJSON(ffjson, &i);
                        (*this->val.pairs)[objId] = obj;
                    } catch (Exception e) {

                    }
                } else if (ffjson[i] == ',') {
                    i++;
                    objIdNail = i;
                } else if (ffjson[i] == '}') {
                    i++;
                    break;
                } else {
                    i++;
                }
            }
            break;
        } else if (ffjson[i] == '[') {
            this->type = ARRAY;
            this->size = 0;
            this->val.array = new std::vector<FFJSON*>();
            i++;
            int objNail = i;
            while (i < j) {
                try {
                    FFJSON* obj = new FFJSON(ffjson, &i);
                    this->val.array->push_back(obj);
                    this->size++;
                } catch (Exception e) {

                }
                while (ffjson[i] != ',' && ffjson[i] != ']')i++;
                if (ffjson[i] == ',') {
                    i++;
                    objNail = i;
                } else if (ffjson[i] == ']') {
                    i++;
                    break;
                } else {
                    i++;
                }
            }
            break;
        } else if (ffjson[i] == '"') {
            i++;
            int strNail = i;
            this->type = STRING;
            while (i < j) {
                if (ffjson[i] == '"' && ffjson[i - 1] != '\\') {
                    this->size = i - strNail;
                    this->val.string = (char*) malloc(this->size + 1);
                    memcpy(this->val.string, ffjson.c_str() + strNail,
                            this->size);
                    this->val.string[this->size] = '\0';
                    i++;
                    break;
                } else {
                    i++;
                }
            }
            break;
        } else if (ffjson[i] == '<') {
            i++;
            int xmlNail = i;
            std::string xmlTag;
            int length = -1;
            bool tagset = false;
            while (ffjson[i] != '>' && i < j) {
                if (ffjson[i] == ' ') {
                    tagset = true;
                    if (ffjson[i + 1] == 'l' &&
                            ffjson[i + 2] == 'e' &&
                            ffjson[i + 3] == 'n' &&
                            ffjson[i + 4] == 'g' &&
                            ffjson[i + 5] == 't' &&
                            ffjson[i + 6] == 'h') {
                        i += 7;
                        while (ffjson[i] != '=' && i < j) {
                            i++;
                        }
                        i++;
                        while (ffjson[i] != '"' && i < j) {
                            i++;
                        }
                        i++;
                        std::string lengthstr;
                        while (ffjson[i] != '"' && i < j) {
                            lengthstr += ffjson[i];
                            i++;
                        }
                        length = std::stoi(lengthstr);
                    }
                } else if (!tagset) {
                    xmlTag += ffjson[i];
                }
                i++;
            }
            this->type = XML;
            i++;
            xmlNail = i;
            if (length>-1 && length < (j - i)) {
                i += length;
            }
            while (i < j) {
                if (ffjson[i] == '<' &&
                        ffjson[i + 1] == '/') {
                    if (xmlTag.compare(ffjson.substr(i + 2, xmlTag.length()))
                            == 0 && ffjson[i + 2 + xmlTag.length()] == '>') {
                        this->size = i - xmlNail;
                        this->val.string = (char*) malloc(this->size);
                        memcpy(this->val.string, ffjson.c_str() + xmlNail,
                                this->size);
                        i += 3 + xmlTag.length();
                        break;
                    }
                }
                i++;
            }
            break;
        } else if (ffjson[i] == 't' &&
                ffjson[i + 1] == 'r' &&
                ffjson[i + 2] == 'u' &&
                ffjson[i + 3] == 'e') {
            this->type = BOOL;
            this->val.boolean = true;
            i += 4;
            break;
        } else if (ffjson[i] == 'f' &&
                ffjson[i + 1] == 'a' &&
                ffjson[i + 2] == 'l' &&
                ffjson[i + 3] == 's' &&
                ffjson[i + 4] == 'e') {
            this->type = BOOL;
            this->val.boolean = true;
            i += 5;
            break;
        } else if ((ffjson[i] >= '0' && ffjson[i] <= '9') || ffjson[i] == '-' || ffjson[i] == '+') {
            int numNail = i;
            i++;
            while ((ffjson[i] >= '0' && ffjson[i] <= '9') || ffjson[i] == '.')
                i++;
            this->size = i - numNail;
            std::string num = ffjson.substr(numNail, i - numNail);
            this->val.number = std::stod(num);
            this->type = FFJSON_OBJ_TYPE::NUMBER;
            break;
        } else if (ffjson[i] == ',' || ffjson[i] == '}' || ffjson[i] == ']') {
            break;
        }
        i++;
    }
    if (ci != NULL)*ci = i;
}

FFJSON::~FFJSON() {
    if (this->type == FFJSON_OBJ_TYPE::OBJECT) {
        std::map<std::string, FFJSON*>::iterator i;
        i = val.pairs->begin();
        while (i != val.pairs->end()) {
            delete i->second;
            i++;
        }
        val.pairs->clear();
        delete this->val.pairs;
    } else if (this->type == FFJSON_OBJ_TYPE::ARRAY) {
        int i = this->val.array->size() - 1;
        while (i >= 0) {
            delete (*this->val.array)[i];
            i--;
        }
        delete this->val.array;
    } else if (this->type == FFJSON_OBJ_TYPE::STRING ||
            this->type == FFJSON_OBJ_TYPE::UNRECOGNIZED
            || this->type == FFJSON_OBJ_TYPE::XML) {
        free(this->val.string);
    }
}

void FFJSON::trimWhites(std::string & s) {
    int i = 0;
    int j = s.length() - 1;
    while (s[i] == ' ' || s[i] == '\n' || s[i] == '\t') {
        i++;
    }
    while (s[j] == ' ' || s[j] == '\n' || s[j] == '\t') {
        j--;
    }
    j++;
    s = s.substr(i, j - i);
}

void FFJSON::trimQuotes(std::string & s) {
    int i = 0;
    int j = s.length() - 1;
    if (s[0] == '"') {
        i++;
    }
    if (s[j] == '"') {
        j--;
    }
    j++;
    s = s.substr(i, j - i);
}

FFJSON::FFJSON_OBJ_TYPE FFJSON::objectType(std::string ffjson) {
    if (ffjson[0] == '{' && ffjson[ffjson.length() - 1] == '}') {
        return FFJSON_OBJ_TYPE::OBJECT;
    } else if (ffjson[0] == '"' && ffjson[ffjson.length() - 1] == '"') {
        return FFJSON_OBJ_TYPE::STRING;
    } else if (ffjson[0] == '[' && ffjson[ffjson.length() - 1] == ']') {
        return FFJSON_OBJ_TYPE::ARRAY;
    } else if (ffjson.compare("true") == 0 || ffjson.compare("false") == 0) {
        return FFJSON_OBJ_TYPE::BOOL;
    } else {
        return FFJSON_OBJ_TYPE::UNRECOGNIZED;
    }
}

FFJSON & FFJSON::operator[](const char* prop) {
    return (*this)[std::string(prop)];
}

FFJSON & FFJSON::operator[](std::string prop) {
    if (this->type == FFJSON_OBJ_TYPE::OBJECT) {
        if ((*this->val.pairs).find(prop) != (*this->val.pairs).end()) {
            if ((*this->val.pairs)[prop] != NULL) {
                return *((*this->val.pairs)[prop]);
            } else {
                throw Exception("NULL");
            }
        } else {
            throw Exception("Key \"" + prop + "\" not defined");
        }
    } else {
        throw Exception("NON OBJECT TYPE");
    }
}

FFJSON & FFJSON::operator[](int index) {
    if (this->type == FFJSON_OBJ_TYPE::ARRAY) {
        if ((*this->val.array)[index] != NULL) {
            return *((*this->val.array)[index]);
        } else {
            throw Exception("NULL");
        }
    } else {
        throw Exception("NON ARRAY TYPE");
    }
};

/**
 * converts FFJSON object to json string
 * @param encode_to_base64 if true then the binary data is base64 encoded
 * @return json string of this FFJSON object
 */
std::string FFJSON::stringify() {
    if (this->type == FFJSON_OBJ_TYPE::STRING) {
        return ("\"" + std::string(this->val.string, this->size) + "\"");
    } else if (this->type == FFJSON_OBJ_TYPE::NUMBER) {
        return (std::string(std::to_string(this->val.number)));
    } else if (this->type == FFJSON_OBJ_TYPE::XML) {
        if (this->base64encode) {
            int output_length = 0;
            char * b64_char = base64_encode((const unsigned char*) this->val.string, this->size, (size_t*) & output_length);
            std::string b64_str(b64_char, output_length);
            return ("\"" + b64_str + "\"");
        } else {
            return ("<xml length=\"" + std::to_string(this->size) + "\">" + std::string(this->val.string, this->size) + "</xml>");
        }
    } else if (this->type == FFJSON_OBJ_TYPE::BOOL) {
        if (this->val.boolean) {
            return ("true");
        } else {
            return ("false");
        }
    } else if (this->type == FFJSON_OBJ_TYPE::OBJECT) {
        std::string ffs;
        std::map<std::string, FFJSON*>& objmap = *(this->val.pairs);
        ffs = "{";
        std::map<std::string, FFJSON*>::iterator i;
        i = objmap.begin();
        while (i != objmap.end()) {
            if (this->base64encode)i->second->base64encode = true;
            if (this->base64encodeChildren&&!this->base64encodeStopChain)i->second->base64encodeChildren = true;
            ffs.append("\"" + i->first + "\":");
            ffs.append(i->second->stringify());
            ffs.append(",");
            i++;
        }
        if (objmap.size() == 0) {
            ffs += ',';
        }
        ffs[ffs.length() - 1] = '}';
        return ffs;
    } else if (this->type == FFJSON_OBJ_TYPE::ARRAY) {
        std::string ffs;
        std::vector<FFJSON*>& objarr = *(this->val.array);
        ffs = "[";
        int i = 0;
        while (i < objarr.size()) {
            if (this->base64encode)objarr[i]->base64encode = true;
            if (this->base64encodeChildren&&!this->base64encodeStopChain)objarr[i]->base64encodeChildren = true;
            ffs.append(objarr[i]->stringify());
            ffs.append(",");
            i++;
        }
        if (objarr.size() == 0) {
            ffs += ',';
        }
        ffs[ffs.length() - 1] = ']';
        return ffs;
    }
}

std::string FFJSON::prettyString(int indent) {
    if (this->type == FFJSON_OBJ_TYPE::STRING) {
        std::string ps = "\"";
        int stringNail = 0;
        int i = 0;
        for (i = 0; i<this->size; i++) {
            if (this->val.string[i] == '\n') {
                ps.append(this->val.string, stringNail, i + 1 - stringNail);
                ps.append('\t', indent);
                stringNail = i + 1;
            }
        }
        ps.append(this->val.string, stringNail, i - stringNail);
        ps += "\"";
        return ps;
    } else if (this->type == FFJSON_OBJ_TYPE::NUMBER) {
        return (std::string(std::to_string(this->val.number)));
    } else if (this->type == FFJSON_OBJ_TYPE::XML) {
        if (this->base64encode) {
            int output_length = 0;
            char * b64_char = base64_encode(
                    (const unsigned char*) this->val.string,
                    this->size, (size_t*) & output_length);
            std::string b64_str(b64_char, output_length);
            return ("\"" + b64_str + "\"");
        } else {
            return ("<xml length = \"" + std::to_string(this->size) + "\" >" + std::string(this->val.string, this->size) + "</xml>");
        }
    } else if (this->type == FFJSON_OBJ_TYPE::BOOL) {
        if (this->val.boolean) {
            return ("true");
        } else {
            return ("false");
        }
    } else if (this->type == FFJSON_OBJ_TYPE::OBJECT) {
        std::map<std::string, FFJSON*>& objmap = *(this->val.pairs);
        std::string ps = "{\n";
        std::map<std::string, FFJSON*>::iterator i;
        i = objmap.begin();
        while (i != objmap.end()) {
            if (this->base64encode)i->second->base64encode = true;
            if (this->base64encodeChildren&&!this->base64encodeStopChain)
                i->second->base64encodeChildren = true;
            ps.append(indent + 1, '\t');
            ps.append("\"" + i->first + "\"\t: ");
            ps.append(i->second->prettyString(indent + 1));
            if (++i != objmap.end())ps.append(",\n");
        }
        ps.append("\n");
        ps.append(indent, '\t');
        ps.append("}");
        return ps;
    } else if (this->type == FFJSON_OBJ_TYPE::ARRAY) {
        std::vector<FFJSON*>& objarr = *(this->val.array);
        std::string ps = "[";
        int i = 0;
        while (i < objarr.size()) {
            if (this->base64encode)objarr[i]->base64encode = true;
            if (this->base64encodeChildren&&!this->base64encodeStopChain)
                objarr[i]->base64encodeChildren = true;
            ps.append(objarr[i]->prettyString(indent + 1));
            if (++i != objarr.size())ps.append(", ");
        }
        ps.append("]");
        return ps;
    }
}

/**
 */
FFJSON::operator const char*() {
    return this->val.string;
}

FFJSON::operator double() {
    return this->val.number;
}

FFJSON::operator bool() {
    return this->val.boolean;
}

FFJSON::operator int() {
    return (int) this->val.number;
}

FFJSON::operator unsigned int() {
    return (unsigned int) this->val.number;
}
