/* 
 * File:   JPEGImage.cpp
 * Author: gowtham
 * 
 * Created on 30 May, 2014, 3:08 PM
 */

#include "JPEGImage.h"
#include "mystdlib.h"
#include <memory>
#ifndef __APPLE__
#ifndef __MACH__
#include <malloc.h>
#endif
#endif
#include <vector>
#include <iostream>

JPEGImage::JPEGImage() {
}

JPEGImage::JPEGImage(const JPEGImage& orig) {
}

JPEGImage::JPEGImage(char* src_img, int length) {
    this->length = length;
    this->src_img = src_img;
}

JPEGImage::~JPEGImage() {

}

char JPEGImage::StdHuffmanTable[] = {
    (char) 0xC4, (char) 0x01, (char) 0xA2, (char) 0x00, (char) 0x00, (char) 0x01, (char) 0x05, (char) 0x01,
    (char) 0x01, (char) 0x01, (char) 0x01, (char) 0x01, (char) 0x01, (char) 0x00, (char) 0x00, (char) 0x00, (char) 0x00,
    (char) 0x00, (char) 0x00, (char) 0x00, (char) 0x00, (char) 0x01, (char) 0x02, (char) 0x03, (char) 0x04, (char) 0x05,
    (char) 0x06, (char) 0x07, (char) 0x08, (char) 0x09, (char) 0x0A, (char) 0x0B, (char) 0x01, (char) 0x00, (char) 0x03,
    (char) 0x01, (char) 0x01, (char) 0x01, (char) 0x01, (char) 0x01, (char) 0x01, (char) 0x01, (char) 0x01, (char) 0x01,
    (char) 0x00, (char) 0x00, (char) 0x00, (char) 0x00, (char) 0x00, (char) 0x00, (char) 0x01, (char) 0x02, (char) 0x03,
    (char) 0x04, (char) 0x05, (char) 0x06, (char) 0x07, (char) 0x08, (char) 0x09, (char) 0x0A, (char) 0x0B, (char) 0x10,
    (char) 0x00, (char) 0x02, (char) 0x01, (char) 0x03, (char) 0x03, (char) 0x02, (char) 0x04, (char) 0x03, (char) 0x05,
    (char) 0x05, (char) 0x04, (char) 0x04, (char) 0x00, (char) 0x00, (char) 0x01, (char) 0x7D, (char) 0x01, (char) 0x02,
    (char) 0x03, (char) 0x00, (char) 0x04, (char) 0x11, (char) 0x05, (char) 0x12, (char) 0x21, (char) 0x31, (char) 0x41,
    (char) 0x06, (char) 0x13, (char) 0x51, (char) 0x61, (char) 0x07, (char) 0x22, (char) 0x71, (char) 0x14, (char) 0x32,
    (char) 0x81, (char) 0x91, (char) 0xA1, (char) 0x08, (char) 0x23, (char) 0x42, (char) 0xB1, (char) 0xC1, (char) 0x15,
    (char) 0x52, (char) 0xD1, (char) 0xF0, (char) 0x24, (char) 0x33, (char) 0x62, (char) 0x72, (char) 0x82, (char) 0x09,
    (char) 0x0A, (char) 0x16, (char) 0x17, (char) 0x18, (char) 0x19, (char) 0x1A, (char) 0x25, (char) 0x26, (char) 0x27,
    (char) 0x28, (char) 0x29, (char) 0x2A, (char) 0x34, (char) 0x35, (char) 0x36, (char) 0x37, (char) 0x38, (char) 0x39,
    (char) 0x3A, (char) 0x43, (char) 0x44, (char) 0x45, (char) 0x46, (char) 0x47, (char) 0x48, (char) 0x49, (char) 0x4A,
    (char) 0x53, (char) 0x54, (char) 0x55, (char) 0x56, (char) 0x57, (char) 0x58, (char) 0x59, (char) 0x5A, (char) 0x63,
    (char) 0x64, (char) 0x65, (char) 0x66, (char) 0x67, (char) 0x68, (char) 0x69, (char) 0x6A, (char) 0x73, (char) 0x74,
    (char) 0x75, (char) 0x76, (char) 0x77, (char) 0x78, (char) 0x79, (char) 0x7A, (char) 0x83, (char) 0x84, (char) 0x85,
    (char) 0x86, (char) 0x87, (char) 0x88, (char) 0x89, (char) 0x8A, (char) 0x92, (char) 0x93, (char) 0x94, (char) 0x95,
    (char) 0x96, (char) 0x97, (char) 0x98, (char) 0x99, (char) 0x9A, (char) 0xA2, (char) 0xA3, (char) 0xA4, (char) 0xA5,
    (char) 0xA6, (char) 0xA7, (char) 0xA8, (char) 0xA9, (char) 0xAA, (char) 0xB2, (char) 0xB3, (char) 0xB4, (char) 0xB5,
    (char) 0xB6, (char) 0xB7, (char) 0xB8, (char) 0xB9, (char) 0xBA, (char) 0xC2, (char) 0xC3, (char) 0xC4, (char) 0xC5,
    (char) 0xC6, (char) 0xC7, (char) 0xC8, (char) 0xC9, (char) 0xCA, (char) 0xD2, (char) 0xD3, (char) 0xD4, (char) 0xD5,
    (char) 0xD6, (char) 0xD7, (char) 0xD8, (char) 0xD9, (char) 0xDA, (char) 0xE1, (char) 0xE2, (char) 0xE3, (char) 0xE4,
    (char) 0xE5, (char) 0xE6, (char) 0xE7, (char) 0xE8, (char) 0xE9, (char) 0xEA, (char) 0xF1, (char) 0xF2, (char) 0xF3,
    (char) 0xF4, (char) 0xF5, (char) 0xF6, (char) 0xF7, (char) 0xF8, (char) 0xF9, (char) 0xFA, (char) 0x11, (char) 0x00,
    (char) 0x02, (char) 0x01, (char) 0x02, (char) 0x04, (char) 0x04, (char) 0x03, (char) 0x04, (char) 0x07, (char) 0x05,
    (char) 0x04, (char) 0x04, (char) 0x00, (char) 0x01, (char) 0x02, (char) 0x77, (char) 0x00, (char) 0x01, (char) 0x02,
    (char) 0x03, (char) 0x11, (char) 0x04, (char) 0x05, (char) 0x21, (char) 0x31, (char) 0x06, (char) 0x12, (char) 0x41,
    (char) 0x51, (char) 0x07, (char) 0x61, (char) 0x71, (char) 0x13, (char) 0x22, (char) 0x32, (char) 0x81, (char) 0x08,
    (char) 0x14, (char) 0x42, (char) 0x91, (char) 0xA1, (char) 0xB1, (char) 0xC1, (char) 0x09, (char) 0x23, (char) 0x33,
    (char) 0x52, (char) 0xF0, (char) 0x15, (char) 0x62, (char) 0x72, (char) 0xD1, (char) 0x0A, (char) 0x16, (char) 0x24,
    (char) 0x34, (char) 0xE1, (char) 0x25, (char) 0xF1, (char) 0x17, (char) 0x18, (char) 0x19, (char) 0x1A, (char) 0x26,
    (char) 0x27, (char) 0x28, (char) 0x29, (char) 0x2A, (char) 0x35, (char) 0x36, (char) 0x37, (char) 0x38, (char) 0x39,
    (char) 0x3A, (char) 0x43, (char) 0x44, (char) 0x45, (char) 0x46, (char) 0x47, (char) 0x48, (char) 0x49, (char) 0x4A,
    (char) 0x53, (char) 0x54, (char) 0x55, (char) 0x56, (char) 0x57, (char) 0x58, (char) 0x59, (char) 0x5A, (char) 0x63,
    (char) 0x64, (char) 0x65, (char) 0x66, (char) 0x67, (char) 0x68, (char) 0x69, (char) 0x6A, (char) 0x73, (char) 0x74,
    (char) 0x75, (char) 0x76, (char) 0x77, (char) 0x78, (char) 0x79, (char) 0x7A, (char) 0x82, (char) 0x83, (char) 0x84,
    (char) 0x85, (char) 0x86, (char) 0x87, (char) 0x88, (char) 0x89, (char) 0x8A, (char) 0x92, (char) 0x93, (char) 0x94,
    (char) 0x95, (char) 0x96, (char) 0x97, (char) 0x98, (char) 0x99, (char) 0x9A, (char) 0xA2, (char) 0xA3, (char) 0xA4,
    (char) 0xA5, (char) 0xA6, (char) 0xA7, (char) 0xA8, (char) 0xA9, (char) 0xAA, (char) 0xB2, (char) 0xB3, (char) 0xB4,
    (char) 0xB5, (char) 0xB6, (char) 0xB7, (char) 0xB8, (char) 0xB9, (char) 0xBA, (char) 0xC2, (char) 0xC3, (char) 0xC4,
    (char) 0xC5, (char) 0xC6, (char) 0xC7, (char) 0xC8, (char) 0xC9, (char) 0xCA, (char) 0xD2, (char) 0xD3, (char) 0xD4,
    (char) 0xD5, (char) 0xD6, (char) 0xD7, (char) 0xD8, (char) 0xD9, (char) 0xDA, (char) 0xE2, (char) 0xE3, (char) 0xE4,
    (char) 0xE5, (char) 0xE6, (char) 0xE7, (char) 0xE8, (char) 0xE9, (char) 0xEA, (char) 0xF2, (char) 0xF3, (char) 0xF4,
    (char) 0xF5, (char) 0xF6, (char) 0xF7, (char) 0xF8, (char) 0xF9, (char) 0xFA, (char) 0xFF
};

char* JPEGImage::huffmanPatchChar() {
    if (!this->hasHuffmanTables) {
        int n = NELEMS(StdHuffmanTable) + this->length;
        char* patchedImage = (char*) malloc(n);
        int i = 0;
        int j = 0;
        int m = 0;
        int l = 0;
        bool huffmanTableAdded = false;
        while (j < (this->length)) {
            if ((this->src_img)[j] == (char) 0xFF) {
                if ((this->src_img)[j + 1] == (char) 0xC4) {
                    this->hasHuffmanTables = true;
                    std::cout << "\nHuffman table found!";
                    return NULL;
                } else if ((this->src_img)[j + 1] == (char) 0xE0/*(char) 0xDA*/ && !huffmanTableAdded) {
                    int m = i + NELEMS(StdHuffmanTable);
                    int k = 0;
                    std::cout << "\nAdding Huffman table before DA start of image marker";
                    while (i < m) {
                        patchedImage[i] = StdHuffmanTable[k];
                        i++;
                        k++;
                    }
                    huffmanTableAdded = true;
                } else if ((this->src_img)[j + 1] == (char) 0x00) {
                    std::cout << "\nMarker in data " << ++l;
                } else {
                    std::cout << "\nMarker hit " << ++m;
                }
            }
            patchedImage[i] = this->src_img[j];
            i++;
            j++;
        }
        this->patchedImageSize = i;
        return patchedImage;
    }
    return NULL;
}

std::shared_ptr<std::vector<char>> JPEGImage::huffmanPatch() {
    if (!this->hasHuffmanTables) {
        int n = NELEMS(StdHuffmanTable) + this->length;
        std::shared_ptr<std::vector<char>> patchedImage(new std::vector<char>(n));
        int i = 0;
        int j = 0;
        while (j<this->length) {
            if ((this->src_img)[j] == (char) 0xFF) {
                if ((this->src_img)[j + 1] == (char) 0xC4) {
                    this->hasHuffmanTables = true;
                    return std::shared_ptr<std::vector<char>>(new std::vector<char>(0));
                } else if ((this->src_img)[j + 1] == (char) 0xDA) {
                    int m = i + NELEMS(StdHuffmanTable);
                    int k = 0;
                    while (i < m) {
                        (*patchedImage)[i] = StdHuffmanTable[k];
                        i++;
                        k++;
                    }
                }
            }
            (*patchedImage)[i] = this->src_img[j];
            i++;
            j++;
        }
        return patchedImage;
    }
    return std::shared_ptr <std::vector<char>>(new std::vector<char>(0));
}
