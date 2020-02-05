#ifndef fileEnDe_h
#define fileEnDe_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#define maxFileNameLen 256          // Max filename length
#define blockSize 16                // Size of the block used for the ARX encryption/decryption.
#define bigAlphabetOffset 65        // Ascii offset to make the randomized number a capitalized alphabet
#define maxStringLen 1001           // Max string length that the user will be able to enter for the input encryption mode.
#define numberAsciiOffSet 48        // add or subtract to an int will make it an ASCII character for the fputc function

struct Menu     // structure for the main menu - > (1. encrypt menu, 2. decrypt menu);
{
    char* menuText;
    int function;
    struct Menu *next[2];
    struct Menu *prior;
};

typedef struct Menu Menu;

char readChar(void);
FILE* openFile(char*, int);
FILE* getInFile(void);
int isNotFileName(char*);
char* validateFileName(char*);
int howLongIsTheFile(FILE*);
int* getKeyFromFile(FILE*);

void decodeAscii(FILE*, int*);
void encodeAscii(void);
void encodeAsciiFromText(void);

void xorCipherRandomKeys(void);
void xorCipherRandomKeysFromText(void);
void decodeXOR(FILE*, FILE*);
void decodeXORuserKey(FILE*, char*);

void leftRotate(char*, int, int);
void rightRotate(char*, int, int);
void blockArxRepeat(FILE*, char*, int, int);
void blockArx(void);
void blockArxFromTextRepeat(char*, char*, int, int);
void blockArxFromText(void);
void decodeArx(FILE*, FILE*);

#endif /* fileEnDe_h */



