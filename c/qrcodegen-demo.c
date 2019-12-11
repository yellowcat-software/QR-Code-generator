/* 
 * QR Code generator demo (C)
 * 
 * Run this command-line program with no arguments. The program
 * computes a demonstration QR Codes and print it to the console.
 * 
 * Copyright (c) Project Nayuki. (MIT License)
 * https://www.nayuki.io/page/qr-code-generator-library
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qrcodegen.h"


// Function prototypes
static void doBasicDemo(void);
static void doVarietyDemo(void);
static void doSegmentDemo(void);
static void doMaskDemo(void);
static void doOptimizedDemo(void);
static void printQr(const uint8_t qrcode[]);
static void saveQr(const uint8_t qrcode[], const char* filename);


// The main application program.
int main(void) {
	doBasicDemo();
	doVarietyDemo();
	doSegmentDemo();
	doMaskDemo();
	doOptimizedDemo();
	return EXIT_SUCCESS;
}



/*---- Demo suite ----*/

// Creates a single QR Code, then prints it to the console.
static void doBasicDemo(void) {
	const char *text = "Hello, world!";                // User-supplied text
	enum qrcodegen_Ecc errCorLvl = qrcodegen_Ecc_LOW;  // Error correction level
	
	// Make and print the QR Code symbol
	uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
	uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
	bool ok = qrcodegen_encodeText(text, tempBuffer, qrcode, errCorLvl,
		qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
	if (ok)
		printQr(qrcode);
}

// Creates an optimized QR Code, then prints it to the console.
static void doOptimizedDemo(void) {
	const char *text = 	"000201"
						"010212"
						"02164116340081341102"
						"52045411"
						"5303981"
						"54131234567890.50"
						"5502"
						"56123456789012.50"
						"5802GE"
						"5925Gigo's Groceryabcdefghijk"
						"6015Tbilisiabcdefgh"
						"623005091234567890713QR-WJ24I08B9K"
						"630420D7";                // User-supplied text
	enum qrcodegen_Ecc errCorLvl = qrcodegen_Ecc_LOW;  // Error correction level
	
	// Make and print the QR Code symbol
	uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
	uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
	uint8_t optimizationBufffer[qrcodegen_OPTIMIZATION_BUFFER_LEN_MAX];
	uint8_t segmentBuffer[qrcodegen_OPTIMIZED_SEGMENT_BUFFER_LEN_MAX];

	uint8_t* charModes = malloc(strlen(text) + sizeof(char));

	bool ok = qrcodegen_encodeTextOptimized(text, optimizationBufffer, segmentBuffer, tempBuffer, charModes, qrcode, errCorLvl,
		qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
	if (ok)
		saveQr(qrcode, "QR.BMP");
		//printQr(qrcode);

	free(charModes);
}


// Creates a variety of QR Codes that exercise different features of the library, and prints each one to the console.
static void doVarietyDemo(void) {
	{  // Numeric mode encoding (3.33 bits per digit)
		uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
		uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
		bool ok = qrcodegen_encodeText("314159265358979323846264338327950288419716939937510", tempBuffer, qrcode,
			qrcodegen_Ecc_MEDIUM, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
		if (ok)
			printQr(qrcode);
	}
	
	{  // Alphanumeric mode encoding (5.5 bits per character)
		uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
		uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
		bool ok = qrcodegen_encodeText("DOLLAR-AMOUNT:$39.87 PERCENTAGE:100.00% OPERATIONS:+-*/", tempBuffer, qrcode,
			qrcodegen_Ecc_HIGH, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
		if (ok)
			printQr(qrcode);
	}
	
	{  // Unicode text as UTF-8
		const char *text = "\xE3\x81\x93\xE3\x82\x93\xE3\x81\xAB\xE3\x81\xA1wa\xE3\x80\x81"
			"\xE4\xB8\x96\xE7\x95\x8C\xEF\xBC\x81\x20\xCE\xB1\xCE\xB2\xCE\xB3\xCE\xB4";
		uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
		uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
		bool ok = qrcodegen_encodeText(text, tempBuffer, qrcode,
			qrcodegen_Ecc_QUARTILE, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
		if (ok)
			printQr(qrcode);
	}
	
	{  // Moderately large QR Code using longer text (from Lewis Carroll's Alice in Wonderland)
		const char *text =
			"Alice was beginning to get very tired of sitting by her sister on the bank, "
			"and of having nothing to do: once or twice she had peeped into the book her sister was reading, "
			"but it had no pictures or conversations in it, 'and what is the use of a book,' thought Alice "
			"'without pictures or conversations?' So she was considering in her own mind (as well as she could, "
			"for the hot day made her feel very sleepy and stupid), whether the pleasure of making a "
			"daisy-chain would be worth the trouble of getting up and picking the daisies, when suddenly "
			"a White Rabbit with pink eyes ran close by her.";
		uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
		uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
		bool ok = qrcodegen_encodeText(text, tempBuffer, qrcode,
			qrcodegen_Ecc_HIGH, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
		if (ok)
			printQr(qrcode);
	}
}


// Creates QR Codes with manually specified segments for better compactness.
static void doSegmentDemo(void) {
	{  // Illustration "silver"
		const char *silver0 = "THE SQUARE ROOT OF 2 IS 1.";
		const char *silver1 = "41421356237309504880168872420969807856967187537694807317667973799";
		uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
		uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
		bool ok;
		{
			char *concat = calloc(strlen(silver0) + strlen(silver1) + 1, sizeof(char));
			strcat(concat, silver0);
			strcat(concat, silver1);
			ok = qrcodegen_encodeText(concat, tempBuffer, qrcode, qrcodegen_Ecc_LOW,
				qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
			if (ok)
				printQr(qrcode);
			free(concat);
		}
		{
			uint8_t *segBuf0 = malloc(qrcodegen_calcSegmentBufferSize(qrcodegen_Mode_ALPHANUMERIC, strlen(silver0)) * sizeof(uint8_t));
			uint8_t *segBuf1 = malloc(qrcodegen_calcSegmentBufferSize(qrcodegen_Mode_NUMERIC, strlen(silver1)) * sizeof(uint8_t));
			struct qrcodegen_Segment segs[] = {
				qrcodegen_makeAlphanumeric(silver0, segBuf0),
				qrcodegen_makeNumeric(silver1, segBuf1),
			};
			ok = qrcodegen_encodeSegments(segs, sizeof(segs) / sizeof(segs[0]), qrcodegen_Ecc_LOW, tempBuffer, qrcode);
			free(segBuf0);
			free(segBuf1);
			if (ok)
				printQr(qrcode);
		}
	}
	
	{  // Illustration "golden"
		const char *golden0 = "Golden ratio \xCF\x86 = 1.";
		const char *golden1 = "6180339887498948482045868343656381177203091798057628621354486227052604628189024497072072041893911374";
		const char *golden2 = "......";
		uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
		uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
		bool ok;
		{
			char *concat = calloc(strlen(golden0) + strlen(golden1) + strlen(golden2) + 1, sizeof(char));
			strcat(concat, golden0);
			strcat(concat, golden1);
			strcat(concat, golden2);
			ok = qrcodegen_encodeText(concat, tempBuffer, qrcode, qrcodegen_Ecc_LOW,
				qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
			if (ok)
				printQr(qrcode);
			free(concat);
		}
		{
			uint8_t *bytes = malloc(strlen(golden0) * sizeof(uint8_t));
			for (size_t i = 0, len = strlen(golden0); i < len; i++)
				bytes[i] = (uint8_t)golden0[i];
			uint8_t *segBuf0 = malloc(qrcodegen_calcSegmentBufferSize(qrcodegen_Mode_BYTE, strlen(golden0)) * sizeof(uint8_t));
			uint8_t *segBuf1 = malloc(qrcodegen_calcSegmentBufferSize(qrcodegen_Mode_NUMERIC, strlen(golden1)) * sizeof(uint8_t));
			uint8_t *segBuf2 = malloc(qrcodegen_calcSegmentBufferSize(qrcodegen_Mode_ALPHANUMERIC, strlen(golden2)) * sizeof(uint8_t));
			struct qrcodegen_Segment segs[] = {
				qrcodegen_makeBytes(bytes, strlen(golden0), segBuf0),
				qrcodegen_makeNumeric(golden1, segBuf1),
				qrcodegen_makeAlphanumeric(golden2, segBuf2),
			};
			free(bytes);
			ok = qrcodegen_encodeSegments(segs, sizeof(segs) / sizeof(segs[0]), qrcodegen_Ecc_LOW, tempBuffer, qrcode);
			free(segBuf0);
			free(segBuf1);
			free(segBuf2);
			if (ok)
				printQr(qrcode);
		}
	}
	
	{  // Illustration "Madoka": kanji, kana, Cyrillic, full-width Latin, Greek characters
		uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
		uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
		bool ok;
		{
			const char *madoka =  // Encoded in UTF-8
				"\xE3\x80\x8C\xE9\xAD\x94\xE6\xB3\x95\xE5"
				"\xB0\x91\xE5\xA5\xB3\xE3\x81\xBE\xE3\x81"
				"\xA9\xE3\x81\x8B\xE2\x98\x86\xE3\x83\x9E"
				"\xE3\x82\xAE\xE3\x82\xAB\xE3\x80\x8D\xE3"
				"\x81\xA3\xE3\x81\xA6\xE3\x80\x81\xE3\x80"
				"\x80\xD0\x98\xD0\x90\xD0\x98\xE3\x80\x80"
				"\xEF\xBD\x84\xEF\xBD\x85\xEF\xBD\x93\xEF"
				"\xBD\x95\xE3\x80\x80\xCE\xBA\xCE\xB1\xEF"
				"\xBC\x9F";
			ok = qrcodegen_encodeText(madoka, tempBuffer, qrcode, qrcodegen_Ecc_LOW,
				qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
			if (ok)
				printQr(qrcode);
		}
		{
			const int kanjiChars[] = {  // Kanji mode encoding (13 bits per character)
				0x0035, 0x1002, 0x0FC0, 0x0AED, 0x0AD7,
				0x015C, 0x0147, 0x0129, 0x0059, 0x01BD,
				0x018D, 0x018A, 0x0036, 0x0141, 0x0144,
				0x0001, 0x0000, 0x0249, 0x0240, 0x0249,
				0x0000, 0x0104, 0x0105, 0x0113, 0x0115,
				0x0000, 0x0208, 0x01FF, 0x0008,
			};
			size_t len = sizeof(kanjiChars) / sizeof(kanjiChars[0]);
			uint8_t *segBuf = calloc(qrcodegen_calcSegmentBufferSize(qrcodegen_Mode_KANJI, len), sizeof(uint8_t));
			struct qrcodegen_Segment seg;
			seg.mode = qrcodegen_Mode_KANJI;
			seg.numChars = (int)len;
			seg.bitLength = 0;
			for (size_t i = 0; i < len; i++) {
				for (int j = 12; j >= 0; j--, seg.bitLength++)
					segBuf[seg.bitLength >> 3] |= ((kanjiChars[i] >> j) & 1) << (7 - (seg.bitLength & 7));
			}
			seg.data = segBuf;
			ok = qrcodegen_encodeSegments(&seg, 1, qrcodegen_Ecc_LOW, tempBuffer, qrcode);
			free(segBuf);
			if (ok)
				printQr(qrcode);
		}
	}
}


// Creates QR Codes with the same size and contents but different mask patterns.
static void doMaskDemo(void) {
	{  // Project Nayuki URL
		uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
		uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
		bool ok;
		
		ok = qrcodegen_encodeText("https://www.nayuki.io/", tempBuffer, qrcode,
			qrcodegen_Ecc_HIGH, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
		if (ok)
			printQr(qrcode);
		
		ok = qrcodegen_encodeText("https://www.nayuki.io/", tempBuffer, qrcode,
			qrcodegen_Ecc_HIGH, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_3, true);
		if (ok)
			printQr(qrcode);
	}
	
	{  // Chinese text as UTF-8
		const char *text =
			"\xE7\xB6\xAD\xE5\x9F\xBA\xE7\x99\xBE\xE7\xA7\x91\xEF\xBC\x88\x57\x69\x6B\x69\x70"
			"\x65\x64\x69\x61\xEF\xBC\x8C\xE8\x81\x86\xE8\x81\xBD\x69\x2F\xCB\x8C\x77\xC9\xAA"
			"\x6B\xE1\xB5\xBB\xCB\x88\x70\x69\xCB\x90\x64\x69\x2E\xC9\x99\x2F\xEF\xBC\x89\xE6"
			"\x98\xAF\xE4\xB8\x80\xE5\x80\x8B\xE8\x87\xAA\xE7\x94\xB1\xE5\x85\xA7\xE5\xAE\xB9"
			"\xE3\x80\x81\xE5\x85\xAC\xE9\x96\x8B\xE7\xB7\xA8\xE8\xBC\xAF\xE4\xB8\x94\xE5\xA4"
			"\x9A\xE8\xAA\x9E\xE8\xA8\x80\xE7\x9A\x84\xE7\xB6\xB2\xE8\xB7\xAF\xE7\x99\xBE\xE7"
			"\xA7\x91\xE5\x85\xA8\xE6\x9B\xB8\xE5\x8D\x94\xE4\xBD\x9C\xE8\xA8\x88\xE7\x95\xAB";
		uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
		uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
		bool ok;
		
		ok = qrcodegen_encodeText(text, tempBuffer, qrcode,
			qrcodegen_Ecc_MEDIUM, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_0, true);
		if (ok)
			printQr(qrcode);
		
		ok = qrcodegen_encodeText(text, tempBuffer, qrcode,
			qrcodegen_Ecc_MEDIUM, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_1, true);
		if (ok)
			printQr(qrcode);
		
		ok = qrcodegen_encodeText(text, tempBuffer, qrcode,
			qrcodegen_Ecc_MEDIUM, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_5, true);
		if (ok)
			printQr(qrcode);
		
		ok = qrcodegen_encodeText(text, tempBuffer, qrcode,
			qrcodegen_Ecc_MEDIUM, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_7, true);
		if (ok)
			printQr(qrcode);
	}
}



/*---- Utilities ----*/

static uint8_t* bmpACreate(uint8_t* data, uint32_t width, uint32_t height, uint32_t* dataSize)
{
	struct __attribute__((__packed__)) {
		// BMP header
		uint8_t 	id[2];
		uint32_t 	fileSize;
		uint8_t		reserved[4];
		uint32_t 	offset;
		// DIB header (Windows BITMAPINFOHEADER)
		uint32_t 	dibSize; // should be 40
		uint32_t 	width;
		uint32_t 	height;
		uint16_t 	planes;
		uint16_t 	bpp;
		uint32_t 	compression; // 0 - no compression
		uint32_t 	dataSize; // image data dataSize
		int32_t 	dpiX;
		int32_t 	dpiY;
		uint32_t 	paletteColors;
		uint32_t 	importantColors;
		// Palette
		uint32_t	colors[2];
	} bitmapHeader = {
			{0x42, 0x4D},			//id: BM
			0,						//fileSize
			{0, 0, 0, 0},			//reserved
			0,						//offset
			40,						//dibSize
			0,						//width
			0,						//height
			1,						//planes
			1,						//bpp
			0,						//compression
			0,						//dataSize
			0,						//dpiX
			0,						//dpiY
			0,						//paletteColors
			0,						//importantColors
			{0x00000000, 0x00FFFFFF} //colors: Black, white
	};

	if ((width % 8) != 0)
		return NULL;

	int32_t rowSize = ((width + 31) / 32) * 4;
	int32_t x, y;
	uint8_t* bitmap = NULL;

	bitmapHeader.offset = sizeof(bitmapHeader);
	bitmapHeader.width = width;
	bitmapHeader.height = height;
	bitmapHeader.dataSize = rowSize * height;
	bitmapHeader.fileSize = bitmapHeader.dataSize + sizeof(bitmapHeader);

	bitmap = calloc(bitmapHeader.fileSize, sizeof(uint8_t));

	if(!bitmap)
	{
		printf("No memory for bitmap!\n");
		*dataSize = 0;
		return NULL;
	}

	memcpy(bitmap, &bitmapHeader, sizeof(bitmapHeader));

	for (y = 0; y < height; y++)
		for (x = 0; x < width/8; x++)
			bitmap[sizeof(bitmapHeader) + ((height - 1 - y) * rowSize) + x] = ~(data[y * (width/8) + x]);

	*dataSize = bitmapHeader.fileSize;

	return bitmap;
}

static void saveQr(const uint8_t qrcode[], const char* filename)
{
	FILE* file;
	uint8_t* qrImage = NULL;
	uint8_t* qrBitmap = NULL;
	uint32_t qrBitmapSize;

	int size = qrcodegen_getSize(qrcode);
	int border = 4;
	int side = (size + border + border + 7) / 8 * 8;
	qrImage = calloc(side * side / 8, sizeof(uint8_t));

	for (int y = -border; y < size + border; y++) {
		for (int x = -border; x < size + border; x++) {
			if (qrcodegen_getModule(qrcode, x, y))
				qrImage[y * (side/8) + x/8] |= 1 << (7 - (x % 8));
		}
	}

	qrBitmap = bmpACreate(qrImage, side, side, &qrBitmapSize);

	file = fopen(filename, "w");

	fwrite(qrBitmap, 1, qrBitmapSize, file);

	fclose(file);

	free(qrImage);
	free(qrBitmap);
}

// Prints the given QR Code to the console.
static void printQr(const uint8_t qrcode[]) {
	int size = qrcodegen_getSize(qrcode);
	int border = 4;
	for (int y = -border; y < size + border; y++) {
		for (int x = -border; x < size + border; x++) {
			fputs((qrcodegen_getModule(qrcode, x, y) ? "##" : "  "), stdout);
		}
		fputs("\n", stdout);
	}
	fputs("\n", stdout);
	fflush(stdout);
}
