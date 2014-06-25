/* 
 * File:   JPEGImage.h
 * Author: gowtham
 *
 * Created on 30 May, 2014, 3:08 PM
 */

#ifndef JPEGIMAGE_H
#define	JPEGIMAGE_H

#include <memory>
#include <vector>

class JPEGImage {
public:
    JPEGImage();
    JPEGImage(const JPEGImage& orig);
    JPEGImage(char * src_img, int length);
    virtual ~JPEGImage();

    /**
     * Inserts huffman table into this image and sets @var hasHuffmanTables if this image alread has Huffman tables.
     * @return shared pointer of this jpeg image with huffman table if it doesn't have huffman table else it returns shared pointer of NULL
     */
    std::shared_ptr<std::vector<char>> huffmanPatch();
    char* huffmanPatchChar();
    int patchedImageSize = 0;

    /**
     * true if image has Huffman table.
     */
    bool hasHuffmanTables = false;

    /**
     * Standard Huffman table defined by JPEG specification
     */
    static char StdHuffmanTable[];
private:

    /**
     *source data pointer is stored in this variable.
     */
    char * src_img;

    /**
     * length of src_img
     */
    int length;
};

#endif	/* JPEGIMAGE_H */

