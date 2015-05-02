/* 
 * File:   FFJSON.cpp
 * Author: Satya Gowtham Kudupudi
 * 
 * Created on November 29, 2013, 4:29 PM
 */

/*
 * To do:
 * 1. remove escape characters from string
 */
#include <string>
#ifndef __APPLE__
#ifndef __MACH__
#include <malloc.h>
#endif
#endif
#include <math.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <map>

#include "FFJSON.h"
#include "mystdlib.h"
#include "myconverters.h"
#include "logger.h"

using namespace std;

const char FFJSON::OBJ_STR[8][15] = {
	"UNDEFINED", "STRING", "XML", "NUMBER", "BOOL", "OBJECT", "ARRAY", "NUL"
};

std::map<std::string, uint8_t> FFJSON::STR_OBJ;

FFJSON::FFJSON() {
//	type = UNDEFINED;
//	qtype = NONE;
//	etype = ENONE;
        flags=0;
        size = 0;
	val.pairs = NULL;
	val.boolean = false;
}

//FFJSON::FFJSON(std::ifstream file) {
//	std::string ffjson((std::istreambuf_iterator<char>(file)),
//			std::istreambuf_iterator<char>());
//	init(ffjson);
//}

FFJSON::FFJSON(OBJ_TYPE t) {
    //s//
//	type = UNDEFINED;
//	qtype = NONE;
//	etype = ENONE;
        flags = 0;
	if (t == OBJECT) {
		setType(OBJECT);
		val.pairs = new map<string, FFJSON*>;
	} else if (t == ARRAY) {
		setType(ARRAY);
		val.array = new vector<FFJSON*>();
	} else if (t == STRING) {
		setType(STRING);
	} else if (t == XML) {
		setType(XML);
	} else if (t == NUMBER) {
		setType(NUMBER);
	} else if (t == BOOL) {
		setType(BOOL);
	} else if (t == NUL) {
		setType(NUL);
	} else {
		throw Exception("UNDEFINED");
	}
}

FFJSON::FFJSON(const FFJSON& orig, COPY_FLAGS cf) {
	copy(orig, cf);
}

FFJSON::FFJSON(const string& ffjson, int* ci, int indent,
        FFJSON::FFJSONPObj* pObj) {
	init(ffjson, ci, indent, pObj);
}

void FFJSON::copy(const FFJSON& orig, COPY_FLAGS cf) {
	setType(orig.getType());
	size = orig.size;
	//type = orig.type;
        //s//
        setType(orig.getType());
        if (isType(NUMBER)) {
		val.number = orig.val.number;
	} else if (isType(STRING)) {
		val.string = (char*) malloc(orig.size + 1);
		memcpy(val.string, orig.val.string, orig.size);
		val.string[orig.size] = '\0';
		size = orig.size;
		if (cf == COPY_EFLAGS) {
			//etype = orig.getEFlags();
                    setEFlag(orig.getEFlags());
		}
	} else if (isType(XML)) {
		val.string = (char*) malloc(orig.size + 1);
		memcpy(val.string, orig.val.string, orig.size);
		val.string[orig.size] = '\0';
		size = orig.size;
		if (cf == COPY_EFLAGS) {
			//s//etype = orig.getEFlags();
                    setEFlag(orig.getEFlags());
		}
	} else if (isType(BOOL)) {
		val.boolean = orig.val.boolean;
	} else if (isType(OBJECT)) {
		map<string, FFJSON*>::iterator i;
		i = orig.val.pairs->begin();
		val.pairs = new map<string, FFJSON*>();
		while (i != orig.val.pairs->end()) {
			FFJSON* fo;
			if (i->second != NULL)
				fo = new FFJSON(*i->second, cf);
			if (fo && ((cf == COPY_QUERIES && !fo->isQType(QUERY_TYPE::NONE))
					|| !fo->isType(UNDEFINED))) {
				(*val.pairs)[i->first] = fo;
			} else {
				delete fo;
			}
			i++;
		}
		if (val.pairs->size() == 0) {
			delete val.pairs;
			val.pairs = NULL;
			///s//type = UNDEFINED;
		        setType(UNDEFINED);
                };
	} else if (isType(ARRAY)) {
		int i = 0;
		val.array = new vector<FFJSON*>();
		bool matter = false;
		while (i < orig.val.array->size()) {
			FFJSON * fo = NULL;
			if ((*orig.val.array)[i] != NULL)
				fo = new FFJSON(*(*orig.val.array)[i]);
			if (fo && ((cf == COPY_QUERIES && !fo->isQType(QUERY_TYPE::NONE))
					|| !fo->isType(UNDEFINED))) {
				(*val.array).push_back(fo);
				matter = true;
			} else {
				(*val.array).push_back(NULL);
				delete fo;
			}
			i++;
		}
		if (!matter) {
			delete val.array;
			val.array = NULL;
			//s//type = UNDEFINED;
                        setType(UNDEFINED);
		}
	} else if (isType(NUL)) {
		setType(NUL);
		size = 0;
		val.boolean = false;
	} else if ((cf == COPY_QUERIES&&!isQType(QUERY_TYPE::NONE)) &&
			isType(UNDEFINED)) {
		//s//qtype = orig.getQType();
                setQType(orig.getQType());
	} else {
		setType(UNDEFINED);
		val.boolean = false;
	}
}

inline bool FFJSON::isWhiteSpace(char c) {
	switch (c) {
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			return true;
		default:
			return false;
	}
}

inline bool FFJSON::isTerminatingChar(char c) {
	switch (c) {
		case ',':
		case '}':
		case ']':
			return true;
		default:
			return false;
	}
}

void FFJSON::init(const std::string& ffjson, int* ci, int indent, FFJSONPObj* pObj) {
	int i = (ci == NULL) ? 0 : *ci;
	int j = ffjson.length();
//	type = UNDEFINED;
//	qtype = NONE;
//	etype = ENONE;
        flags=0;
	size = 0;
	while (i < j) {
		if (ffjson[i] == '{') {
			setType(OBJECT);
			val.pairs = new map<string, FFJSON*>();
			i++;
			int objIdNail = i;
			string objId;
			int nind = getIndent(ffjson.c_str(), &i, indent);
			bool comment = false;
			FFJSONPObj ffpo;
			ffpo.value = this;
			ffpo.pObj = pObj;
			ffpo.value = this;
			while (i < j) {
				if (ffjson[i] == ':' || ffjson[i] == '|') {
					objId = ffjson.substr(objIdNail, i - objIdNail);
					trimWhites(objId);
					trimQuotes(objId);
					ffpo.name = &objId;
					i++;
					FFJSON* obj = new FFJSON(ffjson, &i, nind, &ffpo);
					(*val.pairs)[objId] = obj;
					if (comment) {
						string comment("#");
						comment += objId;
						if (val.pairs->find(comment) != val.pairs->end()) {
                                                        obj->setEFlag(HAS_COMMENT);
						}
					}
					if (objId[0] == '#') {
						comment = true;
						//s//obj->etype |= IS_COMMENT;
                                                obj->setEFlag(HAS_COMMENT);
					} else {
						comment = false;
					}
					if (!comment)size++;
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
			setType(ARRAY);
			size = 0;
			val.array = new vector<FFJSON*>();
			i++;
			int objNail = i;
			int nind = getIndent(ffjson.c_str(), &i, indent);
			while (i < j) {
				FFJSON* obj = new FFJSON(ffjson, &i, nind);
				if (obj->isType(NUL) && ffjson[i] == ']' && size == 0) {
					delete obj;
					obj = NULL;
				} else {
					if ((obj->isType(NUL) || obj->isType(UNDEFINED)) &&
							obj->isQType(NONE)) {
						delete obj;
						val.array->push_back(NULL);
					} else {
						val.array->push_back(obj);
					}
					size++;
				}
				while (ffjson[i] != ',' && ffjson[i] != ']' && i < j)i++;
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
			setType(STRING);
			val.string = NULL;
			int nind = getIndent(ffjson.c_str(), &i, indent);
			vector<char*> bufvec;
			int k = 0;
			int pi = 0;
			char* buf = NULL;
			while (i < j) {
				if ((k % 100) == 0 && (pi == 0 || i >= 100 + pi)) {
					pi = i;
					buf = (char*) malloc(100);
					bufvec.push_back(buf);
					size += 100;
					k = 0;
				}
				if (ffjson[i] == '\\' && nind == indent) {
					i++;
					switch (ffjson[i]) {
						case 'n':
							buf[k] = '\n';
							break;
						case 'r':
							buf[k] = '\r';
							break;
						case 't':
							buf[k] = '\t';
							break;
						case '\\':
							buf[k] = '\\';
							break;
						default:
							buf[k] = ffjson[i];
							break;
					}
					i++;
					k++;
				} else if (ffjson[i] == '\n') {
					int ind = ++i;
					while (ffjson[ind] == '\t' && (ind - i) < nind) {
						ind++;
					}
					if ((ind - i) == nind) {
						i += nind;
					} else if (ffjson[ind] == '"' && ((ind - i) == (nind - 1))) {
						i = ind + 1;
						break;
					}
					if (k != 0) {
						buf[k] = '\n';
						k++;
					}
				} else if (ffjson[i] == '\r' && ffjson[i + 1] == '\n') {
					i++;
					int ind = ++i;
					while (ffjson[ind] == '\t' && (ind - i) < nind) {
						ind++;
					}
					if ((ind - i) == nind) {
						i += nind;
					} else if (ffjson[ind] == '"' && ((ind - i) == (nind - 1))) {
						i = ind + 1;
						break;
					}
					if (k != 0) {
						buf[k] = '\n';
						k++;
					}
				} else if (ffjson[i] == '"' && nind == indent) {
					i++;
					break;
				} else {
					buf[k] = ffjson[i];
					k++;
					i++;
				}
			}
			buf[k] = '\0';
			int ii = 0;
			size += k - 100;
			val.string = (char*) malloc(size + 1);
			int iis = bufvec.size() - 1;
			while (ii < iis) {
				memcpy(val.string + (100 * ii), bufvec[ii], 100);
				delete bufvec[ii];
				ii++;
			}
			memcpy(val.string + (100 * ii), bufvec[ii], k + 1);
			delete bufvec[ii];
			break;
		} else if (ffjson[i] == '<') {
			i++;
			int xmlNail = i;
			string xmlTag;
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
						string lengthstr;
						while (ffjson[i] != '"' && i < j) {
							lengthstr += ffjson[i];
							i++;
						}
						length = stoi(lengthstr);
					}
				} else if (!tagset) {
					xmlTag += ffjson[i];
				}
				i++;
			}
			val.string = NULL;
			setType(XML);
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
						size = i - xmlNail;
						val.string = (char*) malloc(size);
						memcpy(val.string, ffjson.c_str() + xmlNail,
								size);
						i += 3 + xmlTag.length();
						break;
					}
				}
				i++;
			}
			if (val.string == NULL)setType(NUL);
			break;
		} else if (ffjson[i] == 't' &&
				ffjson[i + 1] == 'r' &&
				ffjson[i + 2] == 'u' &&
				ffjson[i + 3] == 'e') {
			setType(BOOL);
			val.boolean = true;
			i += 4;
			break;
		} else if (ffjson[i] == 'f' &&
				ffjson[i + 1] == 'a' &&
				ffjson[i + 2] == 'l' &&
				ffjson[i + 3] == 's' &&
				ffjson[i + 4] == 'e') {
			setType(BOOL);
			val.boolean = false;
			i += 5;
			break;
		} else if ((ffjson[i] >= '0' && ffjson[i] <= '9') || ffjson[i] == '-' ||
				ffjson[i] == '+') {
			int numNail = i;
			i++;
			while ((ffjson[i] >= '0' && ffjson[i] <= '9') ||
					(ffjson[i] == '.' && (ffjson[i + 1] >= '0' && ffjson[i + 1] <= '9')))
				i++;
			size = i - numNail;
			string num = ffjson.substr(numNail, i - numNail);
			val.number = stod(num);
			setType(OBJ_TYPE::NUMBER);
			break;
		} else if (ffjson[i] == ':' && ffjson[i + 1] == '/' && ffjson[i + 2] == '/') {
			if (ffjson[i - 1] == 'e' && ffjson[i - 2] == 'l' &&
					ffjson[i - 3] == 'i' && ffjson[i - 4] == 'f'
					&& (ffjson[i - 5] < 'a' || ffjson[i - 5] > 'z')) {
				i += 3;
				string path;
				string objCaster;
				bool objCastNail = false;
				int k = 0;
				while (i < j && !isTerminatingChar(ffjson[i])) {
					if (!isWhiteSpace(ffjson[i])) {
						if (ffjson[i] == '|') {
							objCastNail = true;
							k = 0;
						} else if (objCastNail) {
							objCaster += ffjson[i];
							k++;
						} else {
							path += ffjson[i];
							k++;
						}
					}
					i++;
				}
				if (path.length() > 0) {
					ifstream ifs(path.c_str(), ios::in | ios::ate);
					if (ifs.is_open()) {
						string ffjsonStr;
						strObjMapInit();
						ifs.seekg(0, std::ios::end);
						uint8_t t = UNDEFINED;
						if (objCaster.length() > 0) {
							t = STR_OBJ[objCaster];
							if (t == STRING || t == OBJECT || t == ARRAY) {
								ffjsonStr.reserve(ifs.tellg() + 2);
								if (t == STRING) {
									ffjsonStr += "\"\n";
								} else if (t == OBJECT) {
									ffjsonStr += "{\n";
								} else if (t == ARRAY) {
									ffjsonStr += "[\n";
								} else {
									t = UNDEFINED;
								}
							};
						} else {
							ffjsonStr.reserve(ifs.tellg());
						}
						ifs.seekg(0, std::ios::beg);
						ffjsonStr.append((std::istreambuf_iterator<char>(ifs)),
								std::istreambuf_iterator<char>());
						if (t) {
							init(ffjsonStr, 0, -1);
						} else {
							init(ffjsonStr);
						}
					}
				}
			} else {
				while (i < j&&!isTerminatingChar(ffjson[i])) {
					i++;
				}
			}
			break;
		} else if (ffjson[i] == '?') {
			setQType(QUERY);
			i++;
			break;
		} else if (ffjson[i] == 'd' &&
				ffjson[i + 1] == 'e' &&
				ffjson[i + 2] == 'l' &&
				ffjson[i + 3] == 'e' &&
				ffjson[i + 4] == 't' &&
				ffjson[i + 5] == 'e') {
			setQType(DELETE);
			i += 6;
			break;
		} else if (ffjson[i] == 'n' &&
				ffjson[i + 1] == 'u' &&
				ffjson[i + 2] == 'l' &&
				ffjson[i + 3] == 'l') {
			setType(NUL);
			i += 4;
			break;
		} else if (isTerminatingChar(ffjson[i])) { // NULL Objects or links caught here eg. "[]", ",,", ",]", "name:,}"
			splitstring subffj(ffjson.c_str() + *ci, i - *ci);
			trimWhites(subffj);
			if (subffj.length() > 0) {
				vector<string>* prop = new vector<string>();
				explode(".", subffj, *prop);
				FFJSON* obj = returnNameIfDeclared(*prop, pObj);
				if (obj) {
					setType(LINK);
					val.fptr = obj;
					m_uFM.link = prop;
				} else {
					delete prop;
				}
			} else {
				setType(NUL);
				size = 0;
			}
			break;
		}
		//s//if (type != UNDEFINED) {
                if (!isType(UNDEFINED)) {
                    while (!isWhiteSpace(ffjson[i]) && i < j) {
				i++;
			}
			if (ffjson[i] == '|') {
				i++;
				FFJSON* obj = new FFJSON(ffjson, &i, indent, pObj);
				if (inherit(*obj)){
                                    this->m_uFM.link;/*G/*continue work here*/
                                }else{
                                    delete obj;
                                }
			}
		}
		i++;
	}
	if (ci != NULL)*ci = i;
}

void FFJSON::setFMCount(uint32_t iFMCount){
    iFMCount<<=28;
                    flags&=0x0FFFFFFF;
                    flags|=iFMCount;
                    
}

void FFJSON::insertFeaturedMember(FeaturedMember& fms,FeaturedMemType fMT){
            FeaturedMember* pFMS    = &m_uFM;
            uint32_t iFMCount       = flags>>28;
            uint32_t iFMTraversed   = 0;
            if(this->isEFlagSet(EXT_VIA_PARENT)){
                if(fMT==FM_TABHEAD){
                    if(pFMS->tabHead==NULL){
                        pFMS->tabHead=fms.tabHead;
                    }else {
                        FeaturedMemHook* pNewFMH = new FeaturedMemHook();
                        pNewFMH->m_pFMH.m_pFMH=pFMS->m_pFMH;
                        pNewFMH->m_uFM.tabHead=pFMS->tabHead;
                        pFMS->m_pFMH=pNewFMH;
                    }
                    iFMCount++;
                    setFMCount(iFMCount);
                    return;
                }else{
                    if(iFMCount-iFMTraversed==1){
                        //Should insert New FM hook before the right FMType match
                        FeaturedMemHook* pNewFMH = new FeaturedMemHook();
                        pNewFMH->m_uFM.tabHead=pFMS->tabHead;
                        pFMS->m_pFMH=pNewFMH;
                        pFMS=&pNewFMH->m_pFMH;
                    }else{
                        pFMS=&pFMS->m_pFMH;
                    }
                }
                iFMTraversed++;
            }
            if(this->isType(LINK)){
                if(fMT==FM_LINK){
                    if(pFMS->link==NULL){
                        pFMS->link=fms.link;
                    }else {
                        FeaturedMemHook* pNewFMH = new FeaturedMemHook();
                        pNewFMH->m_pFMH.m_pFMH=pFMS->m_pFMH;
                        pNewFMH->m_uFM.link=pFMS->link;
                        pFMS->m_pFMH=pNewFMH;
                    }
                    iFMCount++;
                    setFMCount(iFMCount);
                    return;
                }else{
                    if(iFMCount-iFMTraversed==1){
                        //Should insert New FM hook before the right FMType match
                        FeaturedMemHook* pNewFMH = new FeaturedMemHook();
                        pNewFMH->m_uFM.link=pFMS->link;
                        pFMS->m_pFMH=pNewFMH;
                        pFMS=&pNewFMH->m_pFMH;
                    }else{
                        pFMS=&pFMS->m_pFMH;
                    }
                }
                iFMTraversed++;
            }
            if(this->isEFlagSet(IS_EXTENDED)){
                if(fMT==FM_PARENT){
                    if(pFMS->m_pParent==NULL){
                        pFMS->m_pParent=fms.m_pParent;
                    }else {
                        FeaturedMemHook* pNewFMH = new FeaturedMemHook();
                        pNewFMH->m_pFMH.m_pFMH=pFMS->m_pFMH;
                        pNewFMH->m_uFM.m_pParent=pFMS->m_pParent;
                        pFMS->m_pFMH=pNewFMH;
                    }
                    iFMCount++;
                    setFMCount(iFMCount);
                    return;
                }else{
                    if(iFMCount-iFMTraversed==1){
                        //Should insert New FM hook before the right FMType match
                        FeaturedMemHook* pNewFMH = new FeaturedMemHook();
                        pNewFMH->m_uFM.m_pParent=pFMS->m_pParent;
                        pFMS->m_pFMH=pNewFMH;
                        pFMS=&pNewFMH->m_pFMH;
                    }else{
                        pFMS=&pFMS->m_pFMH;
                    }
                }
                iFMTraversed++;
            }
        }

FFJSON::FeaturedMember FFJSON::getFeaturedMember(FeaturedMemType fMT){
    FeaturedMember* pFMS    = &m_uFM;
            uint32_t iFMCount       = flags>>28;
            uint32_t iFMTraversed   = 0;
            if(this->isEFlagSet(EXT_VIA_PARENT)){
                if(fMT==FM_TABHEAD){
                    if(iFMCount-iFMTraversed==1){
                        return *pFMS;
                    }else{
                        return pFMS->m_pFMH->m_uFM;
                    }
                    return;
                }else{
                    if(iFMCount-iFMTraversed==1){
                        return FeaturedMember;
                    }else{
                        pFMS= &pFMS->m_pFMH->m_pFMH;
                    }
                }
                iFMTraversed++;
            }
            if(this->isType(LINK)){
                if(fMT==FM_LINK){
                    if(iFMCount-iFMTraversed==1){
                        return *pFMS;
                    }else{
                        return pFMS->m_pFMH->m_uFM;
                    }
                    return;
                }else{
                    if(iFMCount-iFMTraversed==1){
                        return FeaturedMember;
                    }else{
                        pFMS= &pFMS->m_pFMH->m_pFMH;
                    }
                }
                iFMTraversed++;
            }
            if(this->isEFlagSet(IS_EXTENDED)){
                if(fMT==FM_PARENT){
                    if(iFMCount-iFMTraversed==1){
                        return *pFMS;
                    }else{
                        return pFMS->m_pFMH->m_uFM;
                    }
                    return;
                }else{
                    if(iFMCount-iFMTraversed==1){
                        return FeaturedMember;
                    }else{
                        pFMS= &pFMS->m_pFMH->m_pFMH;
                    }
                }
                iFMTraversed++;
            }
            return FeaturedMember;
}

FFJSON* FFJSON::returnNameIfDeclared(vector<string>& prop, FFJSON::FFJSONPObj * fpo) {
	int j = 0;
	while (fpo != NULL) {
		FFJSON* fp = fpo->value;
		while (j < prop.size()) {
			if (fp->isType(OBJECT)) {
				if (fp->val.pairs->find(prop[j]) != fp->val.pairs->end()) {
					fp = (*fp->val.pairs)[prop[j]];
					j++;
					if (j == prop.size())return fp;
				} else {
					break;
				}
			} else {
				break;
			}
			j++;
		}
		fpo = fpo->pObj;
	}
	return NULL;
}

int FFJSON::getIndent(const char* ffjson, int* ci, int indent) {
	int i = *ci;
	if (ffjson[i] == '\n') {
		i++;
	} else if (ffjson[i] == '\r' && ffjson[i + 1] == '\n') {
		i += 2;
	} else {
		return indent;
	}
	int j = i + indent + 1;
	while (i < j) {
		if (ffjson[i] != '\t') {
			return indent;
		}
		i++;
	}
	return (indent + 1);
}

void FFJSON::strObjMapInit() {
	if (STR_OBJ.size() == 0) {
		STR_OBJ[string("")] = UNDEFINED;
		STR_OBJ[string("UNDEFINED")] = UNDEFINED;
		STR_OBJ[string("STRING")] = STRING;
		STR_OBJ[string("XML")] = XML;
		STR_OBJ[string("NUMBER")] = NUMBER;
		STR_OBJ[string("BOOL")] = BOOL;
		STR_OBJ[string("OBJECT")] = OBJECT;
		STR_OBJ[string("ARRAY")] = ARRAY;
		STR_OBJ[string("NUL")] = NUL;
	}
}

FFJSON::~FFJSON() {
	freeObj();
}

void FFJSON::freeObj() {
	if (isType(OBJ_TYPE::OBJECT)) {
		map<string, FFJSON*>::iterator i;
		i = val.pairs->begin();
		while (i != val.pairs->end()) {
			delete i->second;
			i++;
		}
		delete val.pairs;
	} else if (isType(OBJ_TYPE::ARRAY)) {
		int i = val.array->size() - 1;
		while (i >= 0) {
			delete (*val.array)[i];
			i--;
		}
		delete val.array;
	} else if (isType(OBJ_TYPE::STRING) || isType(OBJ_TYPE::XML)) {
		free(val.string);
	} else if (isType(LINK)) {
		delete m_uFM.link;
	}
}

void FFJSON::trimWhites(string & s) {
	int i = 0;
	int j = s.length() - 1;
	while (s[i] == ' ' || s[i] == '\n' || s[i] == '\t' || s[i] == '\r') {
		i++;
	}
	while (s[j] == ' ' || s[j] == '\n' || s[j] == '\t' || s[j] == '\r') {
		j--;
	}
	j++;
	s = i < j ? s.substr(i, j - i) : "";
}

void FFJSON::trimQuotes(string & s) {
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

FFJSON::OBJ_TYPE FFJSON::objectType(string ffjson) {
	if (ffjson[0] == '{' && ffjson[ffjson.length() - 1] == '}') {
		return OBJ_TYPE::OBJECT;
	} else if (ffjson[0] == '"' && ffjson[ffjson.length() - 1] == '"') {
		return OBJ_TYPE::STRING;
	} else if (ffjson[0] == '[' && ffjson[ffjson.length() - 1] == ']') {
		return OBJ_TYPE::ARRAY;
	} else if (ffjson.compare("true") == 0 || ffjson.compare("false") == 0) {
		return OBJ_TYPE::BOOL;
	} else {
		return OBJ_TYPE::UNDEFINED;
	}
}

FFJSON & FFJSON::operator[](const char* prop) {
	return (*this)[string(prop)];
}

FFJSON& FFJSON::operator[](string prop) {
	if (isType(UNDEFINED)) {
		//s//type = OBJECT;
                setType(OBJECT);
		val.pairs = new map<string, FFJSON*>();
		size = 0;
	}
	if (isType(OBJ_TYPE::OBJECT)) {
		if ((*val.pairs).find(prop) != (*val.pairs).end()) {
			if ((*val.pairs)[prop] != NULL) {
				if ((*val.pairs)[prop]->isType(LINK)) {
					return *((*val.fptr->val.pairs)[prop]);
				}
				return *((*val.pairs)[prop]);
			} else {
				return *((*val.pairs)[prop] = new FFJSON(NUL));
			}
		} else {
			size++;
			return *((*val.pairs)[prop] = new FFJSON());
		}
	} else if (isType(ARRAY)) {
		if (isEFlagSet(IS_EXTENDED)) {
			if (isEFlagSet(EXT_VIA_PARENT)) {

			}
		}
		return ((*this)[stoi(prop)]);
	} else {
		throw Exception("NON OBJECT TYPE");
	}
}

FFJSON & FFJSON::operator[](int index) {
	if (isType(OBJ_TYPE::ARRAY)) {
		if ((*val.array).size() > index) {
			if ((*val.array)[index] == NULL) {
				(*val.array)[index] = new FFJSON(UNDEFINED);
			}
			return *((*val.array)[index]);
		} else if (index == size) {
			if ((*this)[size - 1].isType(UNDEFINED)) {
				return (*this)[size - 1];
			} else {
				FFJSON* f = new FFJSON();
				val.array->push_back(f);
				size++;
				return *f;
			}
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
string FFJSON::stringify(bool json, FFJSONPObj* pObj) {
	if (isType(OBJ_TYPE::STRING)) {
		string s;
		s.reserve(2 * size + 2);
		s += '"';
		int i = 0;
		while (i < size) {
			switch (val.string[i]) {
				case '"':
					s += "\\\"";
					break;
				case '\n':
					s += "\\n";
					break;
				case '\r':
					s += "\\r";
					break;
				case '\t':
					s += "\\t";
					break;
				case '\\':
					s += "\\\\";
					break;
				default:
					s += val.string[i];
					break;
			}
			i++;
		}
		s += '"';
		return s;
	} else if (isType(OBJ_TYPE::NUMBER)) {
		return to_string(val.number);
	} else if (isType(OBJ_TYPE::XML)) {
		if (isEFlagSet(B64ENCODE)) {
			int output_length = 0;
			char * b64_char = base64_encode((const unsigned char*) val.string,
					size, (size_t*) & output_length);
			string b64_str(b64_char, output_length);
			free(b64_char);
			return ("\"" + b64_str + "\"");
		} else {
			return ("<xml length=\"" + to_string(size) + "\">" +
					string(val.string, size) + "</xml>");
		}
	} else if (isType(OBJ_TYPE::BOOL)) {
		if (val.boolean) {
			return ("true");
		} else {
			return ("false");
		}
	} else if (isType(OBJ_TYPE::OBJECT)) {
		string ffs;
		map<string, FFJSON*>& objmap = *(val.pairs);
		ffs = "{";
		map<string, FFJSON*>::iterator i;
		i = objmap.begin();
		while (i != objmap.end()) {
			uint32_t t = i->second ? i->second->getType() : NUL;
			if (t != UNDEFINED && !i->second->isEFlagSet(IS_COMMENT)) {
				if (t != NUL) {
					if (isEFlagSet(B64ENCODE))i->second->setEFlag(B64ENCODE);
					if ((isEFlagSet(B64ENCODE_CHILDREN))&&!isEFlagSet(B64ENCODE_STOP))
						i->second->setEFlag(B64ENCODE_CHILDREN);
				}
				ffs.append("\"" + i->first + "\":");
				if (t != NUL) {
					ffs.append(i->second->stringify(json));
				} else if (json) {
					ffs.append("null");
				}
			}
			if (++i != objmap.end())if (t != UNDEFINED)ffs.append(",");
		}
		ffs += '}';
		return ffs;
	} else if (isType(OBJ_TYPE::ARRAY)) {
		string ffs;
		vector<FFJSON*>& objarr = *(val.array);
		ffs = "[";
		int i = 0;
		while (i < objarr.size()) {
			uint32_t t = objarr[i] ? objarr[i]->getType() : NUL;
			if (t == NUL) {
				if (json) {
					ffs.append("null");
				}
			} else if (t != UNDEFINED) {
				if (isEFlagSet(B64ENCODE))objarr[i]->setEFlag(B64ENCODE);
				if ((isEFlagSet(B64ENCODE_CHILDREN))&&!isEFlagSet(B64ENCODE_STOP))
					objarr[i]->setEFlag(B64ENCODE_CHILDREN);
				ffs.append(objarr[i]->stringify(json));
			}
			if (++i != objarr.size()) {
		/*s*/		if (objarr[i] && objarr[i]->setType(UNDEFINED)) {
					ffs.append(",");
				} else {
					ffs.append(",");
				}
			}
		}
		ffs += ']';
		return ffs;
	} else if (!isQType(NONE)) {
		if (isQType(QUERY)) {
			return "?";
		} else if (isQType(DELETE)) {
			return "delete";
		} else {
			return "";
		}
	//s//} else if (type == LINK) {
        } else if (isType(link)) {
		if (returnNameIfDeclared(*m_uFM.link, pObj) != NULL) {
			return implode(".", *m_uFM.link);
		} else {
			return val.fptr->stringify(json, pObj);
		}
	}
}

string FFJSON::prettyString(bool json, bool printComments, unsigned int indent, FFJSONPrettyPrintPObj* pObj) {
	if (isType(OBJ_TYPE::STRING)) {
		string ps = "\"";
		bool hasNewLine = (strchr(val.string, '\n') != NULL);
		if (hasNewLine) {
			ps += '\n';
			ps.append(indent + 1, '\t');
		}
		int stringNail = 0;
		int i = 0;
		if (hasNewLine) {
			for (i = 0; i < size; i++) {
				if (val.string[i] == '\n') {
					ps.append(val.string, stringNail, i + 1 - stringNail);
					ps.append(indent + 1, '\t');
					stringNail = i + 1;
				}
			}
		} else {
			i = size;
		}
		ps.append(val.string, stringNail, i - stringNail);
		if (hasNewLine) {
			ps += '\n';
			ps.append(indent, '\t');
		}
		ps += "\"";
		return ps;
	} else if (isType(OBJ_TYPE::NUMBER)) {
		return to_string(val.number);
	} else if (isType(OBJ_TYPE::XML)) {
		if (isEFlagSet(B64ENCODE)) {
			int output_length = 0;
			char * b64_char = base64_encode(
					(const unsigned char*) val.string,
					size, (size_t*) & output_length);
			string b64_str(b64_char, output_length);
			free(b64_char);
			return ("\"" + b64_str + "\"");
		} else {
			return ("<xml length = \"" + to_string(size) + "\" >" +
					string(val.string, size) + "</xml>");
		}
	} else if (isType(OBJ_TYPE::BOOL)) {
		if (val.boolean) {
			return ("true");
		} else {
			return ("false");
		}
	} else if (isType(OBJ_TYPE::OBJECT)) {
		map<string, FFJSON*>& objmap = *(val.pairs);
		string ps = "{\n";
		map<string, FFJSON*>::iterator i;
		i = objmap.begin();
		bool notComment = false;
		bool hasComment = false;
		FFJSONPrettyPrintPObj lfpo;
		lfpo.pObj = pObj;
		lfpo.value = this;
		std::map<string*, string> memKeyFFPairMap;
                std::list<string> ffPairLst;
                std::map<string,string> deps;
                lfpo.m_lsFFPairLst=&ffPairLst;
                lfpo.m_mpMemKeyFFPairMap=&memKeyFFPairMap;
                lfpo.m_mpDeps=&deps;
		while (i != objmap.end()) {
                    ffPairLst.push_back(string());
                    //std::list<string>::iterator ffplIter = ffPairLst.end();
			string& ms = *ffPairLst.end();
			uint32_t t = i->second ? i->second->getType() : NUL;
			notComment = !i->second->isEFlagSet(IS_COMMENT);
			hasComment = i->second->isEFlagSet(HAS_COMMENT);
			if (t != UNDEFINED && t != NUL && notComment) {
				if (isEFlagSet(B64ENCODE))i->second->setEFlag(B64ENCODE);
				if ((isEFlagSet(B64ENCODE_CHILDREN))&&!isEFlagSet(B64ENCODE_STOP))
					i->second->setEFlag(B64ENCODE_CHILDREN);
				if (hasComment && !json && printComments) {
					string name("#");
					name += i->first;
					map<string, FFJSON*>::iterator ci = val.pairs->find(name);
					if (ci != val.pairs->end()) {
						ms += "\n";
						ms.append(indent + 1, '\t');
						//memKeyFFPairMap[name] = &ms;
						ms += name + ": ";
						lfpo.name = &name;
						ms += ci->second->prettyString(json, printComments, indent + 1, &lfpo);
						ms += ",\n";
					}
				}
				ms.append(indent + 1, '\t');
				if (json)ms += "\"";
				ms += i->first;
                                memKeyFFPairMap[&ms] = i->first;
				lfpo.name = &i->first;
				if (json)ms += "\"";
				ms += ": ";
				ms.append(i->second->prettyString(json, printComments, indent + 1, &lfpo));
			} else if (t == NUL) {
				ms.append(indent + 1, '\t');
				if (json)ms.append("\"");
				ms += i->first;
				memKeyFFPairMap[&ms] = i->first;
				if (json)ms += "\"";
				ms += ": ";
			}
			if (t != UNDEFINED && notComment) {		//comment already gets its ",\n" above.
				ms.append(",\n");
				if (hasComment && notComment && !json && printComments) {
					ms += '\n';
				}
			}
		}
                headTheHeader(std::map<string*,string>& memKeyFFPairMap, std::list<string>& ffPairLst,FFJSONPrettyPrintPObj& lfpo);
		ps.append(indent, '\t');
		ps.append("}");
		return ps;
	} else if (isType(OBJ_TYPE::ARRAY)) {
		vector<FFJSON*>& objarr = *(val.array);
		string ps = "[\n";
		int i = 0;
		FFJSONPObj lfpo;
		lfpo.pObj = pObj;
		lfpo.value = this;
		while (i < objarr.size()) {
			uint8_t t = objarr[i] ? objarr[i]->type : NUL;
			if (t != UNDEFINED && t != NUL) {
				if (isEFlagSet(B64ENCODE))objarr[i]->setEFlag(B64ENCODE);
				if ((isEFlagSet(B64ENCODE_CHILDREN))&&!isEFlagSet(B64ENCODE_STOP))
					objarr[i]->setEFlag(B64ENCODE_CHILDREN);
				ps.append(indent + 1, '\t');
				string name = to_string(i);
				lfpo.name = &name;
				ps.append(objarr[i]->prettyString(json, printComments, indent + 1, &lfpo));
			} else if (t == NUL) {
				ps.append(indent + 1, '\t');
			}
			if (++i != objarr.size()) {
				/*if (objarr[i] && objarr[i]->type != UNDEFINED) {
					ps.append(",\n");
				} else {
					ps.append(",\n");
				}*/
				ps.append(",\n");
			} else {
				ps.append("\n");
			}
		}
		ps.append(indent, '\t');
		ps.append("]");
		return ps;
	} else if (isType(LINK)) {
		vector<string>* vtProp = getFeaturedMember(FM_LINK).link;
		if (returnNameIfDeclared(*vtProp, pObj) != NULL) {
			return implode(".", *vtProp);
		} else {
			return val.fptr->stringify(json, pObj);
		}
	} else if (!isQType(NONE)) {
		if (isQType(QUERY)) {
			return "?";
		} else if (isQType(DELETE)) {
			return "delete";
		} else {
			return "";
		}
	} else {

		return "";
	}
}

/**
 */
FFJSON::operator const char*() {

	return isType(LINK) ? val.fptr->val.string : val.string;
}

FFJSON::operator double() {
	return isType(LINK) ? val.fptr->val.number : val.number;
}

FFJSON::operator float() {

	return (float) isType(LINK) ? val.fptr->val.number : val.number;
}

FFJSON::operator bool() {
	FFJSON* fp = this;
	if (isType(LINK)) {
		fp = this->val.fptr;
	};
	//s//if (type != BOOL && type != UNDEFINED && type != NUL) {
             if (isType(!BOOL) && isType(!UNDEFINED) && isType(!NUL)) {   
		return true;
	//s//} else if (type == BOOL) {
             } else if (isType(BOOL)) {   
		return val.boolean;
	} else {

		return false;
	}
}

FFJSON::operator int() {
	if (isType(LINK)) {
		return (int) (val.fptr->val.number);
	}
	return (int) val.number;
}

FFJSON::operator unsigned int() {
	//s//if (type == LINK) {
            if (isType(LINK)) {
		return (unsigned int) (val.fptr->val.number);
	}
	return (unsigned int) val.number;
}

FFJSON & FFJSON::operator=(const string& s) {
	freeObj();
	int i = 0;
	int j = s.length();
	if (s[0] == '<') {
		i++;
		int xmlNail = i;
		string xmlTag;
		int length = -1;
		bool tagset = false;
		while (s[i] != '>' && i < j) {
			if (s[i] == ' ') {
				tagset = true;
				if (s[i + 1] == 'l' &&
						s[i + 2] == 'e' &&
						s[i + 3] == 'n' &&
						s[i + 4] == 'g' &&
						s[i + 5] == 't' &&
						s[i + 6] == 'h') {
					i += 7;
					while (s[i] != '=' && i < j) {
						i++;
					}
					i++;
					while (s[i] != '"' && i < j) {
						i++;
					}
					i++;
					string lengthstr;
					while (s[i] != '"' && i < j) {
						lengthstr += s[i];
						i++;
					}
					length = stoi(lengthstr);
				}
			} else if (!tagset) {
				xmlTag += s[i];
			}
			i++;
		}
		setType(XML);
		i++;
		xmlNail = i;
		if (length>-1 && length < (j - i)) {
			i += length;
		}
		while (i < j) {
			if (s[i] == '<' &&
					s[i + 1] == '/') {
				if (xmlTag.compare(s.substr(i + 2, xmlTag.length()))
						== 0 && s[i + 2 + xmlTag.length()] == '>') {
					size = i - xmlNail;
					val.string = (char*) malloc(size + 1);
					memcpy(val.string, s.c_str() + xmlNail,
							size);
					val.string[size] = '\0';
					i += 3 + xmlTag.length();
					break;
				}
			}
			i++;
		}
	} else {

		setType(STRING);
		size = s.length();
		val.string = (char*) malloc(size + 1);
		memcpy(val.string, s.c_str(),
				size);
		val.string[size] = '\0';
	}
}

FFJSON& FFJSON::operator=(const int& i) {

	freeObj();
	setType(NUMBER);
	val.number = i;
}

FFJSON& FFJSON::operator=(const FFJSON& f) {

	freeObj();
	copy(f);
}

FFJSON& FFJSON::operator=(FFJSON * f) {

	freeObj();
	setType(LINK);
	val.fptr = f;
}

FFJSON& FFJSON::operator=(const double& d) {

	freeObj();
	setType(NUMBER);
	val.number = d;
}

FFJSON& FFJSON::operator=(const float& f) {

	freeObj();
	setType(NUMBER);
	val.number = f;
}

FFJSON& FFJSON::operator=(const long& l) {

	freeObj();
	setType(NUMBER);
	val.number = l;
}

FFJSON& FFJSON::operator=(const short& s) {

	freeObj();
	setType(NUMBER);
	val.number = s;
}

FFJSON & FFJSON::operator=(const unsigned int& i) {

	freeObj();
	setType(NUMBER);
	val.number = i;
}

void FFJSON::trim() {
	if (isType(OBJECT)) {
		map<string, FFJSON*>::iterator i;
		i = val.pairs->begin();
		while (i != val.pairs->end()) {
			if (((*i->second).isType(UNDEFINED)&&!(*i->second).isQType(NONE)) || (*i->second).isType(NUL)) {
				delete i->second;
				map<string, FFJSON*>::iterator j = i;
				i++;
				val.pairs->erase(j);
				size--;
			} else {
				i++;
			}
		}
	} else if (isType(ARRAY)) {
		if ((*this)[size - 1].isType(UNDEFINED)) {
			delete (*val.array)[size - 1];
			val.array->pop_back();
			size--;
		}
		int i = 0;
		while (i < val.array->size()) {
			if ((*val.array)[i] != NULL) {
				if (((*val.array)[i]->isType(UNDEFINED)&&
						!(*val.array)[i]->isQType(NONE)) ||
						(*val.array)[i]->isType(NUL)) {

					delete (*val.array)[i];
					(*val.array)[i] = NULL;
				}
			}
			i++;
		}
	}
}

string FFJSON::queryString() {
	if (isType(OBJ_TYPE::STRING)) {
		if (isQType(SET)) {
			return ("\"" + string(val.string, size) + "\"");
		} else if (isQType(QUERY)) {
			return "?";
		} else if (isQType(DELETE)) {
			return "delete";
		} else {
			return "";
		}
	} else if (isType(OBJ_TYPE::NUMBER)) {
		if (isQType(SET)) {
			return to_string(val.number);
		} else if (isQType(QUERY)) {
			return "?";
		} else if (isQType(DELETE)) {
			return "delete";
		} else {
			return "";
		}
	} else if (isType(OBJ_TYPE::XML)) {
		if (isQType(SET)) {
			if (isEFlagSet(B64ENCODE)) {
				int output_length = 0;
				char * b64_char = base64_encode((const unsigned char*) val.string,
						size, (size_t*) & output_length);
				string b64_str(b64_char, output_length);
				free(b64_char);
				return ("\"" + b64_str + "\"");
			} else {
				return ("<xml length=\"" + to_string(size) + "\">" +
						string(val.string, size) + "</xml>");
			}
		} else if (isQType(QUERY)) {
			return "?";
		} else if (isQType(DELETE)) {
			return "delete";
		} else {
			return "";
		}
	} else if (isType(OBJ_TYPE::BOOL)) {
		if (isQType(SET)) {
			if (val.boolean) {
				return ("true");
			} else {
				return ("false");
			}
		} else if (isQType(QUERY)) {
			return "?";
		} else if (isQType(DELETE)) {
			return "delete";
		} else {
			return "";
		}
	} else if (isType(OBJ_TYPE::OBJECT)) {
		if (isQType(QUERY)) {
			return "?";
		} else if (isQType(DELETE)) {
			return "delete";
		} else {
			string ffs;
			map<string, FFJSON*>& objmap = *(val.pairs);
			ffs = "{";
			map<string, FFJSON*>::iterator i;
			i = objmap.begin();
			bool matter = false;
			while (i != objmap.end()) {
				string ffjsonStr;
				uint32_t t = (i->second != NULL) ? i->second->getType() : NUL;
				if (t != UNDEFINED || (t != NUL&&!i->second->isQType(NONE))) {
					if (t != NUL) {
						if (isEFlagSet(B64ENCODE))i->second->setEFlag(B64ENCODE);
						if ((isEFlagSet(B64ENCODE_CHILDREN))&&!isEFlagSet(B64ENCODE_STOP))
							i->second->setEFlag(B64ENCODE_CHILDREN);
						ffjsonStr = i->second->queryString();
					}
					if (ffjsonStr.length() > 0) {
						if (matter)ffs.append(",");
						ffs.append("\"" + i->first + "\":");
						ffs.append(ffjsonStr);
						matter = true;
					}
				}
				i++;
			}
			if (ffs.length() == 1) {
				ffs = "";
			} else {
				ffs += '}';
			}
			return ffs;
		}
	} else if (isType(OBJ_TYPE::ARRAY)) {
		if (isQType(QUERY)) {
			return "?";
		} else if (isQType(DELETE)) {
			return "delete";
		} else {
			string ffs;
			vector<FFJSON*>& objarr = *(val.array);
			ffs = "[";
			bool matter = false;
			int i = 0;
			string ffjsonstr;
			bool firstTime = false;
			while (i < objarr.size()) {
				unsigned int t = objarr[i] != NULL ? objarr[i]->type : NUL;
				if (t != UNDEFINED || (t != NUL&&!objarr[i]->isQType(NONE))) {
					if (t != NUL) {
						if (isEFlagSet(B64ENCODE))objarr[i]->setEFlag(B64ENCODE);
						if ((isEFlagSet(B64ENCODE_CHILDREN))&&!isEFlagSet(B64ENCODE_STOP))
							objarr[i]->setEFlag(B64ENCODE_CHILDREN);
						ffjsonstr = objarr[i]->queryString();
					} else {
						ffjsonstr = "";
					}
					if (firstTime)ffs += ',';
					firstTime = true;
					if (ffjsonstr.length() > 0) {
						ffs.append(ffjsonstr);
						matter = true;
					}
				}
				i++;
			}
			if (matter) {
				ffs += ']';
			} else {
				ffs = "";
			};
			return ffs;
		}
	} else if (!isQType(NONE)) {
		if (isQType(QUERY)) {
			return "?";
		} else if (isQType(DELETE)) {
			return "delete";
		} else {
			return "";
		}
	} else {

		return "";
	}
}

FFJSON * FFJSON::answerObject(FFJSON * queryObject) {
	FFJSON * ao = NULL;
	if (queryObject->isType(getType()) || queryObject->isQType(QUERY) ||
			queryObject->isQType(DELETE)) {
		if (queryObject->isQType(DELETE)) {
			freeObj();
			setType(NUL);
		} else if (queryObject->isQType(QUERY)) {
			ao = new FFJSON(*this);
		} else if (queryObject->isType(OBJECT)) {
			map<string, FFJSON*>::iterator i = queryObject->val.pairs->begin();
			while (i != queryObject->val.pairs->end()) {
				map<string, FFJSON*>::iterator j;
				if ((j = (*this).val.pairs->find(i->first)) != val.pairs->end()) {
					FFJSON* lao = j->second->answerObject(i->second);
					if (lao != NULL) {
						if (ao == NULL)ao = new FFJSON(OBJECT);
						(*(*ao).val.pairs)[i->first] = lao;
					}
				} else {
					/*FFJSON* nao = new FFJSON(*i->second);
					if (!nao->isType(UNDEFINED)) {
						(*this).val.pairs[i->first] = nao;
					}*/
				}
				i++;
			}
		} else if (queryObject->isType(ARRAY)) {
			if (queryObject->size == size) {
				int i = 0;
				bool matter = false;
				ao = new FFJSON(ARRAY);
				while (i < queryObject->size) {
					if ((*queryObject->val.array)[i] != NULL) {
						FFJSON* ffo = NULL;
						ffo = (*val.array)[i]->answerObject
								((*queryObject->val.array)[i]);
						if (ffo) {
							ao->val.array->push_back(ffo);
							matter = true;
						} else {
							ao->val.array->push_back(NULL);
						}
					} else {
						ao->val.array->push_back(NULL);
					}
					i++;
				}
				if (!matter) {
					delete ao;
					ao = NULL;
				}
			}
		} else if (queryObject->isType(STRING) ||
				queryObject->isType(XML) ||
				queryObject->isType(BOOL) ||
				queryObject->isType(NUMBER)) {
			freeObj();
			copy(*queryObject);
		} else {

			ao = NULL;
		}
	}
	return ao;
}

//bool FFJSON::isType(uint8_t t) const {
//
//	return (t == type);
//}

bool FFJSON::isType(OBJ_TYPE t) const {
    return ((t&flags)==t);
}

//void FFJSON::setType(uint8_t t) {
//
//	type = t;
//}

void FFJSON::setType(OBJ_TYPE t) {
    flags&=0xffffff00;
    flags|=t;
}

//uint8_t FFJSON::getType() const {
//
//	return type;
//}

FFJSON::OBJ_TYPE FFJSON::getType() const {
    int type = 0xff;
    type&=flags;
	return type;
}

//bool FFJSON::isQType(uint8_t t) const {
//
//	return (t == qtype);
//}

bool FFJSON::isQType(QUERY_TYPE t) const {
    uint32_t qtype = flags;
    qtype&=0xff00;
	return (t == qtype);
}

//void FFJSON::setQType(uint8_t t) {
//        
//	qtype = t;
//}

void FFJSON::setQType(QUERY_TYPE t) {
    flags&=(~0xff00);
        //flags&=11111111111111110000000011111111b;
        flags|=t;
}

//uint8_t FFJSON::getQType() const {
//
//	return qtype;
//}

FFJSON::QUERY_TYPE FFJSON::getQType() const {
    uint32_t qtype=flags;
    qtype&=0xff00;
    return qtype;
}

//bool FFJSON::isEFlagSet(int t) const {
//
//	return (t & etype == t);
//}

bool FFJSON::isEFlagSet(E_FLAGS t) const {
    return (t & flags == t);
}

//uint8_t FFJSON::getEFlags() const {
//
//	return this->etype;
//}

FFJSON::E_FLAGS FFJSON::getEFlags() const {
    return flags;
}

//void FFJSON::setEFlag(int t) {
//    
//	etype |= t;
//}

void FFJSON::setEFlag(E_FLAGS t) {
    flags|=t;
}

//void FFJSON::clearEFlag(int t) {
//
//	etype &= ~t;
//}

void FFJSON::clearEFlag(E_FLAGS t) {
	flags &= ~(t);
}

void FFJSON::erase(string name) {
	FFJSON* fp = this;
	if (isType(LINK))fp = val.fptr;
	if (isType(OBJECT)) {

		delete (*val.pairs)[name];
		val.pairs->erase(name);
	}
}

void FFJSON::erase(int index) {
	if (isType(ARRAY)) {
		if (index < size) {

			delete (*val.array)[index];
			(*val.array)[index] = NULL;
		}
	}
}

void FFJSON::erase(FFJSON * value) {
	if (isType(OBJECT)) {
		map<string, FFJSON*>::iterator i = val.pairs->begin();
		while (i != val.pairs->end()) {
			if (i->second == value) {
				delete i->second;
				val.pairs->erase(i);
				break;
			}
			i++;
		}
	} else if (isType(ARRAY)) {
		int i = 0;
		while (i < size) {
			if ((*val.array)[i] == value) {
				delete (*val.array)[i];
				(*val.array)[i] = NULL;

				break;
			}
			i++;
		}
	}
}

bool FFJSON::inherit(FFJSON& obj) {
	if (obj.isType(ARRAY)) {
		if (isType(ARRAY)) {
			if (obj.size == 1) {
				FFJSON& arr = obj[0];
				map<string, int>* m = new map<string, int>();
				int i = arr.size;
				while (i > 0) {
					i--;
					(*m)[string((const char*) arr[i])] = i;
				}
				i = obj.size;
				while (i > 0) {
					i--;
					obj[i].setEFlag(FFJSON::E_FLAGS::EXT_VIA_PARENT);
					obj[i].m_uFM.tabHead = m;
				}
			}
		}
	} else if (obj.isType(OBJECT)) {

	} else if (obj.isType(STRING)) {

	} else if (obj.isType(LINK)) {

	}
	return false;
}

FFJSON::Iterator FFJSON::begin() {
	FFJSON* fp = this;
	if (isType(LINK))fp = val.fptr;
	Iterator i(*fp);

	return i;
}

FFJSON::Iterator FFJSON::end() {
	FFJSON* fp = this;
	if (isType(LINK))fp = val.fptr;
	Iterator i(*fp, true);

	return i;
}

FFJSON::Iterator::Iterator() {
	type = NUL;
}

FFJSON::Iterator::Iterator(const Iterator & orig) {

	copy(orig);
}

FFJSON::Iterator::Iterator(const FFJSON& orig, bool end) {

	init(orig, end);
}

FFJSON::Iterator::~Iterator() {
	if (type == OBJECT) {
         	delete ui.pi;
	} else if (type == ARRAY) {

		delete ui.ai;
	}
}

void FFJSON::Iterator::copy(const Iterator & i) {
	type = i.type;
        if (type == OBJECT) {
		ui.pi = new map<string, FFJSON*>::iterator();
		(*ui.pi) = *i.ui.pi;
	} else if (isType(ARRAY)) {//s//

		ui.ai = new vector<FFJSON*>::iterator();
		(*ui.ai) = *i.ui.ai;
	}
}

void FFJSON::Iterator::init(const FFJSON& orig, bool end) {
	if (orig.isType(ARRAY)) {
		type = ARRAY;
                ui.ai = new vector<FFJSON*>::iterator();
		(*ui.ai) = end ? orig.val.array->end() : orig.val.array->begin();
	} else if (orig.isType(OBJECT)) {
		type = OBJECT;
                ui.pi = new map<string, FFJSON*>::iterator();
		(*ui.pi) = end ? orig.val.pairs->end() : orig.val.pairs->begin();
	} else {

		type = NUL;
                ui.ai = NULL;
	}
}

FFJSON::Iterator & FFJSON::Iterator::operator=(const Iterator & i) {

	copy(i);
}

FFJSON & FFJSON::Iterator::operator*() {
	//s//if (type == OBJECT) {
         if (isType(OBJECT)) {   
		return *((*ui.pi)->second);
	//s//} else if (type == ARRAY) {
            
                } else if (isType(ARRAY)) {
		return **(*ui.ai);
	}
}

FFJSON * FFJSON::Iterator::operator->() {
	if (type == OBJECT) {
        	return ((*ui.pi)->second);
	} else if (type == ARRAY) {
        	return &(**(*ui.ai));
	}
}

FFJSON::Iterator & FFJSON::Iterator::operator++() {
	if (type == OBJECT) {
        	while ((*ui.pi)->second->isEFlagSet(IS_COMMENT)) {
			(*ui.pi)++;
		}
		(*ui.pi)++;
	} else if (type == ARRAY) {
        	(*ui.ai)++;
	}
	return *this;
}

FFJSON::Iterator FFJSON::Iterator::operator++(int) {
	FFJSON::Iterator tmp(*this);
	operator++();

	return tmp;
}

FFJSON::Iterator & FFJSON::Iterator::operator--() {
	if (type == OBJECT) {
        	(*ui.pi)--;
	} else if (type == ARRAY) {
        	(*ui.ai)--;
	}
	return *this;
}

FFJSON::Iterator FFJSON::Iterator::operator--(int) {
	FFJSON::Iterator tmp(*this);
	operator++();

	return tmp;
}

bool FFJSON::Iterator::operator==(const Iterator & i) {
	if (type == i.type) {
        	if (type == OBJECT) {
                	return ((*ui.pi) == (*i.ui.pi));
	} else if (type == ARRAY) {
			return (*ui.ai == *i.ui.ai);
	} else if (type == NUL) {

			return true;
		}
	}
	return false;
}

bool FFJSON::Iterator::operator!=(const Iterator & i) {

	return !operator==(i);
}

FFJSON::Iterator::operator const char*() {
	if (type == OBJECT) {
        	return (*ui.pi)->first.c_str();
	}
	return NULL;
}

FFJSON::FFJSONPrettyPrintPObj::FFJSONPrettyPrintPObj(std::list<StringPair>& spl) : spl(spl) {
};

void FFJSON::headTheHeader(FFJSONPrettyPrintPObj& lfpo){
    std::list<string>::iterator ffPL = lfpo.ffPairLst->begin();
    while(ffPL!=ffPairLst.end()){
        FFJSON* pF = (*val.pairs)[(*lfpo.memKeyFFPairMap)[&(*ffPL)]];
        if(pF->isEFlagSet(IS_EXTENDED)){
            pF->m_uFM.link
        }else if(pF->isEFlagSet(EXT_VIA_PARENT)){
            
        };
    }
}