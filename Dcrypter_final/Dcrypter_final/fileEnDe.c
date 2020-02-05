#include "fileEnDe.h"

char readChar()     // Take in only one char and prevents bad inputs by returning 0 on one.
{
    char ans, badAns = 0;
    do
    {
        ans = getchar();
    } while (ans == '\n' || ans == '\t' || ans==' ');
    while (getchar() != '\n')
    {
        badAns = 1;
    }
    
    if (badAns)
        return 0;
    else
        return ans;
}

FILE* openFile(char* fileName, int method) // Open file and checks for non-existent file, will ask the user to reenter the name METHODS: 0 for Read only, 1 for writing.
{
    FILE *file = 0;
    
    if (method == 0)
    {
        file = fopen(fileName, "r");
        
        if (file == NULL)
        {
            char newFileName[maxFileNameLen];
            while (file == NULL)
            {
                printf("Error: File not found, please try again.\n>> ");
                fgets(newFileName, maxFileNameLen, stdin);
                validateFileName(newFileName);
                file = fopen(newFileName, "r");
            }
        }
    }
    else if (method == 1)
    {
        file = fopen(fileName, "w");
    }
    return file;
}

FILE* getInFile()           //Prompt user for the filename and call openFile() to open in read mode.
{
    printf("\n");
    char fileName[maxFileNameLen];
    printf("Enter the file name: ");
    fgets(fileName, maxFileNameLen, stdin);
    validateFileName(fileName);
    FILE* inFile = openFile(fileName, 0);
    
    return inFile;
}

int isNotFileName(char* str)  // This function checks whether the string argument is a filename or not by looking for .txt in the string. Also serves to prevent the user from entering an empty filename.
{
    char *txt = ".txt";
    
    while (strlen(str) <= 1 || (str[0] == '.'))
    {
        printf("Error: The filename cannot be empty or starts with a dot, please try again.\n>> ");
        fgets(str, maxFileNameLen, stdin);
    }

    if (strstr(str, txt))
    {
        return 0;
    }
    
    return 1;
}

char* validateFileName(char* str)      // Take in filename, make sure it's valid and remove the \n character added by fgets().
{
    if(isNotFileName(str))
    {
        str[strlen(str)-1] = '\0';
        strcat(str, ".txt");
    }
    else
        str[strlen(str)-1] = '\0';

    return str;
}


int howLongIsTheFile(FILE* file)  // Character count for a file basically. (with space and \n included.)
{
    rewind(file);
    int length = 0;
    char x;
    
    while ((x = fgetc(file)) != EOF)
    {
        length += 1;
    }
    rewind(file);
    
    return length;
    
}

int* getKeyFromFile(FILE* keyFile)  // Read a key from a file and then store them into an array and return it.
{
    rewind(keyFile);
    int* eKey;
    int keyNow = 0;
    char k;
    int fileCharCount = howLongIsTheFile(keyFile);
    
    eKey = (int*) malloc(fileCharCount * sizeof(int));
    
    while ((k = fgetc(keyFile)) != EOF)
    {
        eKey[keyNow] = k;
        keyNow += 1;
    }
    
    return eKey;
}


void decodeAscii(FILE *inFile, int* eKey)  // Assuming the inFile is opened for reading. eKey here is an array storing all the encryption keys. This function 
{
    rewind(inFile);
    int keyNow = 0;  // Counter for the encrpytion key.
    char c;
    FILE *outFile;
    char fileName[maxFileNameLen];
    printf("Please enter the output file name: ");
    fgets(fileName, maxFileNameLen, stdin);
    
    validateFileName(fileName);
    
    outFile = fopen(fileName, "w");
    
    while ((c = fgetc(inFile)) != EOF)
    {
        int actualKey = eKey[keyNow] - numberAsciiOffSet;
        fputc(c - actualKey, outFile);
        keyNow += 1;
    }
    fclose(outFile);
    fclose(inFile);
    system("open ."); // Probably OS-dependent
    printf("Decryption completed.\n");
    
}

void encodeAscii()  // Encryption by randomizing a number between 0 - 9 and add it to each character in a file, and the key for each char is just that number.
{
    FILE* inFile = getInFile();
    rewind(inFile);
    int currentKey = 0;
    srand(time(NULL));
    char line[100];
    int fileCharCount = howLongIsTheFile(inFile);
    char fileName[maxFileNameLen];
    char keyFileName[maxFileNameLen] = {};
    printf("Please enter the output file name: ");
    fgets(fileName, maxFileNameLen, stdin);
    
    validateFileName(fileName);
    
    strncpy(keyFileName, fileName, strlen(fileName) - 4);
    strcat(keyFileName, "_key.txt");
    
    FILE *outFile = fopen(fileName, "w");
    FILE *outKey = fopen(keyFileName, "w");
    
    int *keys;
    keys = (int*) malloc(fileCharCount * sizeof(int));
    
    while (fgets(line, 100, inFile)) {
        for (int i = 0; i < 100; i++)  // For each char in a line, add to each char with some random int from 1 to 9.
        {
            if (line[i] == '\0')
                break;
            else
            {
                int key = rand() % 10;
                fputc(key + numberAsciiOffSet, outKey);
                keys[currentKey] = key;
                currentKey += 1;
                //printf("%c", line[i] + asci);
                fputc(line[i] + key, outFile);
            }
            
        }
    }
    printf("Here's your key: ");
    for (int j = 0; j < fileCharCount; j++)
    {
        printf("%d",keys[j]);
    }
    printf("\n");
    fclose(inFile);
    fclose(outKey);
    fclose(outFile);
    free(keys);
    printf("Encryption completed.\n");
    system("open ."); // Probably OS-dependent
}

void encodeAsciiFromText()  // Same as the encodeAscii function above, except this one receive direct input from user rather than a file.
{
    char words[maxStringLen];
    printf("Enter the string to be decrypted: ");
    fgets(words, maxStringLen,stdin);
    
    int currentKey = 0;
    srand(time(NULL));
    int fileCharCount = strlen(words);
    char fileName[maxFileNameLen];
    char keyFileName[maxFileNameLen] = {};
    printf("Please enter the output file name: ");
    fgets(fileName, maxFileNameLen, stdin);
    
    validateFileName(fileName);
    
    strncpy(keyFileName, fileName, strlen(fileName) - 4);
    strcat(keyFileName, "_key.txt");
    
    FILE *outFile = fopen(fileName, "w");
    FILE *outKey = fopen(keyFileName, "w");
    
    int *keys;
    keys = (int*) malloc(fileCharCount * sizeof(int));
    
    for (int i = 0; i < fileCharCount; i++)  // For each char in a line, add to each char with some random int from 1 to 9.
    {
        if (words[i] == '\0')
            break;
        else
        {
            int key = rand() % 10;
            fputc(key + numberAsciiOffSet, outKey);
            keys[currentKey] = key;
            currentKey += 1;
            //printf("%c", words[i] + asci);
            fputc(words[i] + key, outFile);
        }
        
    }
    printf("Here's your key: ");
    for (int j = 0; j < fileCharCount; j++)
    {
        printf("%d",keys[j]);
    }
    printf("\n");
    fclose(outKey);
    fclose(outFile);
    free(keys);
    printf("Encryption completed.\n");
    system("open ."); // Probably OS-dependent
}

void xorCipherRandomKeys()  //Encrypt each character by using the XOR bitwise operator (^) with a random key (A-Z).
{
    FILE* inFile = getInFile();
    rewind(inFile);
    srand(time(NULL));
    char keyLenS[100];
    char* tempLenS;
    long keyLength;
    int fileLength = howLongIsTheFile(inFile);
    int currentKey = 0;
    int *keys;
    char line[100];
    FILE *outFile;
    FILE *outKey;
    
    char fileName[maxFileNameLen];
    char keyFileName[maxFileNameLen] = {};
    printf("Please enter the output file name: ");
    fgets(fileName, maxFileNameLen, stdin);
    
    validateFileName(fileName);
    
    strncpy(keyFileName, fileName, strlen(fileName) - 4);
    strcat(keyFileName, "_key.txt");
    
    outFile = fopen(fileName, "w");
    outKey = fopen(keyFileName, "w");
    
    printf("How long to you want the key to be? (Any extra keys longer than the file length will be ignored).\nEnter 0 if you want the key to be as long as the word count in the file.\n");
    printf(">> ");
    fgets(keyLenS, 100, stdin);
    keyLength = strtol(keyLenS, &tempLenS, 10);
    
    while ((strcmp(tempLenS, "\n") != 0) || keyLength < 0)
    {
        printf("The input should be a non-negative integer, try again.\n");
        printf(">> ");
        fgets(keyLenS, 100, stdin);
        keyLength = strtol(keyLenS, &tempLenS, 10);
    }

    
    if (keyLength == 0 || keyLength > fileLength)
    {
        keyLength = fileLength;
    }
    
    keys = (int*) malloc(keyLength * sizeof(int));
    printf("Your key is ");
    for (int i = 0; i < keyLength; i++)
    {
        keys[i] = (rand() % 26) + bigAlphabetOffset;
        if (keys[i] != 0)
        {
            fputc(keys[i], outKey);
            printf("%c", keys[i]);
        }
    }
    printf("\n");
    
    while (fgets(line, 100, inFile)) {
        for (int i = 0; i < 100; i++)  // For each char in a line, XOR them with the key.
        {
            if (currentKey > keyLength - 1) //Loop back to the first key if the key is shorter than the text.
                currentKey = 0;
            if (line[i] == '\0')
                break;
            else
            {
                //printf("%c^%c -> %c\n", line[i], keys[currentKey], line[i]^keys[currentKey]);
                fputc(line[i]^keys[currentKey] , outFile);
                currentKey += 1;
            }
            
        }
    }
    fclose(inFile);
    fclose(outFile);
    fclose(outKey);
    free(keys);
    printf("Encryption completed.\n");
    system("open .");
    
}

void xorCipherRandomKeysFromText()  //Encrypt each character by using the XOR bitwise operator (^) with a random key (A-Z).
{
    char words[maxStringLen];
    printf("Enter the string to be decrypted: ");
    fgets(words, maxStringLen,stdin);
    
    srand(time(NULL));
    long keyLength;
    char keyLenS[maxFileNameLen];
    char* tempLenS;
    int stringLength = strlen(words);
    int currentKey = 0;
    int *keys;
    FILE *outFile;
    FILE *outKey;
    
    char fileName[maxFileNameLen];
    char keyFileName[maxFileNameLen] = {};
    printf("Please enter the output file name: ");
    fgets(fileName, maxFileNameLen, stdin);
    
    validateFileName(fileName);
    
    strncpy(keyFileName, fileName, strlen(fileName) - 4);
    strcat(keyFileName, "_key.txt");
    
    outFile = fopen(fileName, "w");
    outKey = fopen(keyFileName, "w");
    
    printf("How long to you want the key to be? (Any extra keys longer than the file length will be ignored).\nEnter 0 if you want the key to be as long as the word count in the file.\n");
    printf(">> ");
    fgets(keyLenS, 100, stdin);
    keyLength = strtol(keyLenS, &tempLenS, 10);
    
    while ((strcmp(tempLenS, "\n") != 0) || keyLength < 0)
    {
        printf("The input should be a non-negative integer, try again.\n");
        printf(">> ");
        fgets(keyLenS, 100, stdin);
        keyLength = strtol(keyLenS, &tempLenS, 10);
    }

    
    if (keyLength == 0 || keyLength > stringLength)
    {
        keyLength = stringLength;
    }
    
    keys = (int*) malloc(keyLength * sizeof(int));
    printf("Your key is ");
    for (int i = 0; i < keyLength; i++)
    {
        keys[i] = (rand() % 26) + bigAlphabetOffset;
        if (keys[i] != 0)
        {
            fputc(keys[i], outKey);
            printf("%c", keys[i]);
        }
    }
    printf("\n");
    
    for (int i = 0; i < stringLength; i++)  // For each char in the string, XOR them with the key.
    {
        if (currentKey > keyLength - 1) //Loop back to the first key if the key is shorter than the text.
            currentKey = 0;
        if (words[i] == '\0')
            break;
        else
        {
            //printf("%c^%c -> %c\n", words[i], keys[currentKey], words[i]^keys[currentKey]);
            fputc(words[i]^keys[currentKey] , outFile);
            currentKey += 1;
        }
        
    }
    
    fclose(outFile);
    fclose(outKey);
    free(keys);
    printf("Encryption completed.\n");
    system("open .");
    
}

void decodeXOR(FILE* inFile, FILE* keyFile)  // Decode the XOR Cipher by accepting the text file and a key file.
{
    rewind(inFile);
    int keyNow = 0;  // Counter for the encrpytion key.
    int* key = getKeyFromFile(keyFile);
    int keyLength = howLongIsTheFile(keyFile);
    //printf("keylength = %d\n", keyLength);
    char c;
    FILE *outFile;
    char fileName[maxFileNameLen];
    printf("Please enter the output file name: ");
    fgets(fileName, maxFileNameLen, stdin);
    
    validateFileName(fileName);
    
    outFile = fopen(fileName, "w");
    
    while ((c = fgetc(inFile)) != EOF)
    {
        if (keyNow > keyLength - 1)
            keyNow = 0;
        //printf("%c^%c -> %c\n", c, key[keyNow], c^key[keyNow]);
        fputc(c^key[keyNow], outFile);
        keyNow += 1;
    }
    
    fclose(inFile);
    free(key);
    fclose(outFile);
    printf("Decryption completed\n");
    system("open .");
}

void decodeXORuserKey(FILE* inFile, char* key)  // Decode the XOR Cipher by accepting the text file and a key input by the user.
{
    rewind(inFile);
    int keyNow = 0;  // Counter for the encrpytion key.
    int keyLength = strlen(key);
    char c;
    FILE *outFile;
    char fileName[maxFileNameLen];
    printf("Please enter the output file name: ");
    fgets(fileName, maxFileNameLen, stdin);
    
    validateFileName(fileName);
    
    outFile = fopen(fileName, "w");
    
    while ((c = fgetc(inFile)) != EOF)
    {
        if (keyNow > keyLength - 1)
            keyNow = 0;
        //printf("%c^%c -> %c\n", c, key[keyNow], c^key[keyNow]);
        fputc(c^key[keyNow], outFile);
        keyNow += 1;
    }
    fclose(inFile);
    fclose(outFile);
    printf("Decryption completed\n");
    system("open ."); // Probably OS-dependent
}

void leftRotate(char arr[], int rounds, int arrLen)     // Circular Shift an array to the left.
{
    for (int i = 0; i < rounds; i++)
    {
        char temp = arr[0], j;
        
        for (j = 0; j < arrLen-1; j++)
        {
            arr[j] = arr[j+1];
        }
        
        arr[j] = temp;
    }
}

void rightRotate(char arr[], int rounds, int arLen)     // Circular Shift an array to the right.
{
    for (int i = 0; i < rounds; i++)
    {
        char temp = arr[arLen - 1], j;
        
        for (j = arLen-1; j > 0; j--)
        {
            arr[j] = arr[j-1];
        }
        
        arr[0] = temp;
    }
}


void blockArxRepeat(FILE *inFile, char outFileName[maxFileNameLen], int validKey, int repeatCount)   // When the initial blockArx catches EOF while writing the file, it will call this function to do the whole process over again with a new set of keys.
{
    if (!validKey)
    {
        struct timespec t1, t2;
        t1.tv_nsec = 500000000;
        t1.tv_sec = 0;
        
        rewind(inFile);
        srand(time(NULL));
        char ch;
        int currentChar = 0;
        int currentKey = 0;
        int fileLength = howLongIsTheFile(inFile);
        int keyLength = (fileLength/blockSize + 1);
        int *keys;
        FILE *outFile;
        FILE *outKey;
        int bigOrSmall = 0;
        char *blocks[keyLength];
        
        char keyFileName[maxFileNameLen] = {};
        
        strncpy(keyFileName, outFileName, strlen(outFileName) - 4);
        strcat(keyFileName, "_key.txt");
        
        outFile = fopen(outFileName, "w");
        outKey = fopen(keyFileName, "w");
        
        keys = (int*) malloc((keyLength) * sizeof(int));
        
        for (int b = 0; b < keyLength; b++)
            blocks[b] = ((char *) malloc(blockSize));
        
        //printf("Your key is ");
        for (int i = 0; i < keyLength; i++)    // Generating keys from either (a-z) or (A-Z)
        {
            bigOrSmall = rand() % 10;
            keys[i] = (bigOrSmall >= 5)? ((rand() % 26) + bigAlphabetOffset):((rand() % 26) + 97);
            if (keys[i] != 0)
            {
                fputc(keys[i], outKey);
                //printf("%c", keys[i]);
            }
        }
        printf(".");
        
        while ((ch = fgetc(inFile)) != EOF)
        {
            if (currentChar >= blockSize)
            {
                //printf("before rotation and XOR: %s\n", blocks[currentKey]);
                leftRotate(blocks[currentKey], keys[currentKey] % 16, currentChar);     // Rotating the array of char by x rounds to the left.
                for (int i = 0; i < blockSize; i++)
                {
                    //printf("BLOCK#%d Xor'ing %c with %c = %c\n", currentKey, blocks[currentKey][i], keys[currentKey], (blocks[currentKey][i]) ^ (keys[currentKey]));
                    blocks[currentKey][i] = (blocks[currentKey][i]) ^ (keys[currentKey]);
                    if (blocks[currentKey][i] == EOF && currentKey != fileLength)
                    {
                        for (int b = 0; b < keyLength; b++)
                            free(blocks[b]);
                        
                        fclose(outKey);
                        fclose(outFile);
                        free(keys);
                        repeatCount += 1;
                        //printf("EOF char detected, recalling function for the %d(th) time.....\n", repeatCount);
                        nanosleep(&t1, &t2);    // Sleeping for about 0.5 seconds to allow srand() to set new seed.
                        blockArxRepeat(inFile, outFileName, 0, repeatCount);
                        return;
                        
                    }
                    else
                        fputc(blocks[currentKey][i], outFile);
                }
                //printf("After rotation and XOR: %s\n", blocks[currentKey]);
                //printf("\n");
                currentKey += 1;
                currentChar = 0;
            }
            
            //printf("BLOCK#%d BIT#%d %c + %d = %c\n", currentKey, currentChar, ch, keys[currentKey], ch + keys[currentKey]);
            blocks[currentKey][currentChar] = ch + keys[currentKey] % 26;   // MIGHT HAVE TO % 26 BEFORE ADDING KEYS
            //printf("%d ", keys[currentKey]);
            //printf("Block#%d %c\n", currentKey, blocks[currentKey][currentChar]);
            currentChar += 1;
        }
        
        leftRotate(blocks[currentKey], keys[currentKey] % 16, currentChar);     // Rotating the array of char by x rounds to the left.
        for (int i = 0; i < currentChar; i++)
        {
            //printf("BLOCK#%d Xor'ing %c with %c = %c\n", currentKey, blocks[currentKey][i], keys[currentKey], (blocks[currentKey][i]) ^ (keys[currentKey]));
            blocks[currentKey][i] = (blocks[currentKey][i]) ^ (keys[currentKey]);
            fputc(blocks[currentKey][i], outFile);
        }
        
        printf("\nEncryption completed with %d tries. Blocks: %d\n", repeatCount, keyLength);
        
        for (int b = 0; b < keyLength; b++)
            free(blocks[b]);
        
        fclose(inFile);
        fclose(outKey);
        fclose(outFile);
        free(keys);
        system("open .");
    }
    else
    {
        fclose(inFile);
    }
}

void blockArx()  // Encrypt the file in blocks of 16 char each, by adding numbers, then left rotating the block of characters, and then XOR with the key.
{
    FILE *inFile = getInFile();
    rewind(inFile);
    srand(time(NULL));
    char ch;
    int currentChar = 0;
    int currentKey = 0;
    int fileLength = howLongIsTheFile(inFile);
    int keyLength = (fileLength/blockSize + 1);
    int *keys;
    FILE *outFile;
    FILE *outKey;
    int bigOrSmall = 0;
    char *blocks[keyLength];
    
    char fileName[maxFileNameLen];
    char keyFileName[maxFileNameLen] = {};
    printf("Please enter the output file name: ");
    fgets(fileName, maxFileNameLen, stdin);
    
    validateFileName(fileName);
    
    strncpy(keyFileName, fileName, strlen(fileName) - 4);
    strcat(keyFileName, "_key.txt");
    
    outFile = fopen(fileName, "w");
    outKey = fopen(keyFileName, "w");
    
    keys = (int*) malloc((keyLength) * sizeof(int));
    
    for (int b = 0; b < keyLength; b++)
        blocks[b] = ((char *) malloc(blockSize));
    
    //printf("Your key is ");
    for (int i = 0; i < keyLength; i++)    // Generating keys from either (a-z) or (A-Z)
    {
        bigOrSmall = rand() % 10;
        keys[i] = (bigOrSmall >= 5)? ((rand() % 26) + bigAlphabetOffset):((rand() % 26) + 97);
        if (keys[i] != 0)
        {
            fputc(keys[i], outKey);
            //printf("%c", keys[i]);
        }
    }
    printf("\n");
    
    while ((ch = fgetc(inFile)) != EOF)
    {
        if (currentChar >= blockSize)
        {
            //printf("before rotation and XOR: %s\n", blocks[currentKey]);
            leftRotate(blocks[currentKey], keys[currentKey] % 16, currentChar);     // Rotating the array of char by x rounds to the left.
            for (int i = 0; i < blockSize; i++)
            {
                //printf("BLOCK#%d Xor'ing %c with %c = %c\n", currentKey, blocks[currentKey][i], keys[currentKey], (blocks[currentKey][i]) ^ (keys[currentKey]));
                blocks[currentKey][i] = (blocks[currentKey][i]) ^ (keys[currentKey]);
                if (blocks[currentKey][i] == EOF)
                {
                    for (int b = 0; b < keyLength; b++)
                        free(blocks[b]);
                    
                    fclose(outKey);
                    fclose(outFile);
                    free(keys);
                    //printf("EOF char detected, calling blockArxRepeat....\n");
                    blockArxRepeat(inFile, fileName, 0, 1);
                    return;
                }
                else
                {
                    fputc(blocks[currentKey][i], outFile);
                }
            }
            //printf("After rotation and XOR: %s\n", blocks[currentKey]);
            //printf("\n");
            currentKey += 1;
            currentChar = 0;
        }
        
        //printf("BLOCK#%d BIT#%d %c + %d = %c\n", currentKey, currentChar, ch, keys[currentKey], ch + keys[currentKey]);
        blocks[currentKey][currentChar] = ch + keys[currentKey] % 26;   // MIGHT HAVE TO % 26 BEFORE ADDING KEYS
        //printf("%d ", keys[currentKey]);
        //printf("Block#%d %c\n", currentKey, blocks[currentKey][currentChar]);
        currentChar += 1;
    }
    
    leftRotate(blocks[currentKey], keys[currentKey] % 16, currentChar);     // Rotating the array of char by x rounds to the left.
    for (int i = 0; i < currentChar; i++)
    {
        //printf("BLOCK#%d Xor'ing %c with %c = %c\n", currentKey, blocks[currentKey][i], keys[currentKey], (blocks[currentKey][i]) ^ (keys[currentKey]));
        blocks[currentKey][i] = (blocks[currentKey][i]) ^ (keys[currentKey]);
        fputc(blocks[currentKey][i], outFile);
    }
    
    printf("Encryption completed.\n");
    printf("Blocks: %d\n", keyLength);
    
    for (int b = 0; b < keyLength; b++)
        free(blocks[b]);
    
    fclose(inFile);
    fclose(outKey);
    fclose(outFile);
    free(keys);
    system("open .");
    
}

void blockArxFromTextRepeat(char str[maxStringLen], char outFileName[maxFileNameLen], int validKey, int repeatCount)   // When the initial blockArx catches EOF while writing the file, it will call this function to do the whole process over again with a new set of keys.
{
    if (!validKey)
    {
        struct timespec t1, t2;
        t1.tv_nsec = 500000000;
        t1.tv_sec = 0;
        
        srand(time(NULL));
        char ch;
        int currentChar = 0;
        int currentKey = 0;
        int stringLength = strlen(str);
        int keyLength = (stringLength/blockSize + 1);
        int *keys;
        FILE *outFile;
        FILE *outKey;
        int bigOrSmall = 0;
        char *blocks[keyLength];
        char keyFileName[maxFileNameLen] = {};
        
        strncpy(keyFileName, outFileName, strlen(outFileName) - 4);
        strcat(keyFileName, "_key.txt");
        
        outFile = fopen(outFileName, "w");
        outKey = fopen(keyFileName, "w");
        
        keys = (int*) malloc((keyLength) * sizeof(int));
        
        for (int b = 0; b < keyLength; b++)
            blocks[b] = ((char *) malloc(blockSize));
        
        //printf("Your key is ");
        for (int i = 0; i < keyLength; i++)    // Generating keys from either (a-z) or (A-Z)
        {
            bigOrSmall = rand() % 10;
            keys[i] = (bigOrSmall >= 5)? ((rand() % 26) + bigAlphabetOffset):((rand() % 26) + 97);
            if (keys[i] != 0)
            {
                fputc(keys[i], outKey);
                //printf("%c", keys[i]);
            }
        }
        printf(".");
        
        for(int c = 0; c < stringLength; c++)
        {
            if (currentChar >= blockSize)
            {
                //printf("before rotation and XOR: %s\n", blocks[currentKey]);
                leftRotate(blocks[currentKey], keys[currentKey] % 16, currentChar);     // Rotating the array of char by x rounds to the left.
                for (int i = 0; i < blockSize; i++)
                {
                    //printf("BLOCK#%d Xor'ing %c with %c = %c\n", currentKey, blocks[currentKey][i], keys[currentKey], (blocks[currentKey][i]) ^ (keys[currentKey]));
                    blocks[currentKey][i] = (blocks[currentKey][i]) ^ (keys[currentKey]);
                    if (blocks[currentKey][i] == EOF && currentKey != stringLength)
                    {
                        for (int b = 0; b < keyLength; b++)
                            free(blocks[b]);
                        
                        fclose(outKey);
                        fclose(outFile);
                        free(keys);
                        repeatCount += 1;
                        //printf("EOF char detected, recalling function for the %d(th) time.....\n", repeatCount);
                        nanosleep(&t1, &t2);    // Sleeping for about 0.5 seconds to allow srand() to set new seed.
                        blockArxFromTextRepeat(str, outFileName, 0, repeatCount);
                        return;
                        
                    }
                    else
                        fputc(blocks[currentKey][i], outFile);
                }
                //printf("After rotation and XOR: %s\n", blocks[currentKey]);
                //printf("\n");
                currentKey += 1;
                currentChar = 0;
            }
            
            //printf("BLOCK#%d BIT#%d %c + %d = %c\n", currentKey, currentChar, ch, keys[currentKey], ch + keys[currentKey]);
            blocks[currentKey][currentChar] = str[c] + keys[currentKey] % 26;   // MIGHT HAVE TO % 26 BEFORE ADDING KEYS
            //printf("%d ", keys[currentKey]);
            //printf("Block#%d %c\n", currentKey, blocks[currentKey][currentChar]);
            currentChar += 1;
        }
        
        leftRotate(blocks[currentKey], keys[currentKey] % 16, currentChar);     // Rotating the array of char by x rounds to the left.
        for (int i = 0; i < currentChar; i++)
        {
            //printf("BLOCK#%d Xor'ing %c with %c = %c\n", currentKey, blocks[currentKey][i], keys[currentKey], (blocks[currentKey][i]) ^ (keys[currentKey]));
            blocks[currentKey][i] = (blocks[currentKey][i]) ^ (keys[currentKey]);
            fputc(blocks[currentKey][i], outFile);
        }
        
        printf("\nEncryption completed with %d tries. Blocks: %d\n", repeatCount, keyLength);
        
        for (int b = 0; b < keyLength; b++)
            free(blocks[b]);
        
        fclose(outKey);
        fclose(outFile);
        free(keys);
        system("open .");
    }
}

void blockArxFromText()  // Encrypt the file in blocks of 16 char each, by adding numbers, then left rotating the block of characters, and then XOR with the key.
{
    char words[maxStringLen];
    printf("Enter the string to be encrypted: ");
    fgets(words, maxStringLen, stdin);
    
    srand(time(NULL));
    int currentChar = 0;
    int currentKey = 0;
    int stringLength = strlen(words);
    int keyLength = (stringLength/blockSize + 1);
    int *keys;
    FILE *outFile;
    FILE *outKey;
    int bigOrSmall = 0;
    char *blocks[keyLength];
    
    char fileName[maxFileNameLen];
    char keyFileName[maxFileNameLen] = {};
    printf("Please enter the output file name: ");
    fgets(fileName, maxFileNameLen, stdin);
    
    validateFileName(fileName);
    
    strncpy(keyFileName, fileName, strlen(fileName) - 4);
    strcat(keyFileName, "_key.txt");
    
    outFile = fopen(fileName, "w");
    outKey = fopen(keyFileName, "w");
    
    keys = (int*) malloc((keyLength) * sizeof(int));
    
    for (int b = 0; b < keyLength; b++)
        blocks[b] = ((char *) malloc(blockSize));
    
    //printf("Your key is ");
    for (int i = 0; i < keyLength; i++)    // Generating keys from either (a-z) or (A-Z)
    {
        bigOrSmall = rand() % 10;
        keys[i] = (bigOrSmall >= 5)? ((rand() % 26) + bigAlphabetOffset):((rand() % 26) + 97);
        if (keys[i] != 0)
        {
            fputc(keys[i], outKey);
            //printf("%c", keys[i]);
        }
    }
    printf("\n");
    
    for (int c = 0; c < stringLength; c++)
    {
        if (currentChar >= blockSize)
        {
            //printf("before rotation and XOR: %s\n", blocks[currentKey]);
            leftRotate(blocks[currentKey], keys[currentKey] % 16, currentChar);     // Rotating the array of char by x rounds to the left.
            for (int i = 0; i < blockSize; i++)
            {
                //printf("BLOCK#%d Xor'ing %c with %c = %c\n", currentKey, blocks[currentKey][i], keys[currentKey], (blocks[currentKey][i]) ^ (keys[currentKey]));
                blocks[currentKey][i] = (blocks[currentKey][i]) ^ (keys[currentKey]);
                if (blocks[currentKey][i] == EOF)
                {
                    for (int b = 0; b < keyLength; b++)
                        free(blocks[b]);
                    
                    fclose(outKey);
                    fclose(outFile);
                    free(keys);
                    //printf("EOF char detected, calling blockArxRepeat....\n");
                    blockArxFromTextRepeat(words, fileName, 0, 1);
                    return;
                }
                else
                {
                    fputc(blocks[currentKey][i], outFile);
                }
            }
            //printf("After rotation and XOR: %s\n", blocks[currentKey]);
            //printf("\n");
            currentKey += 1;
            currentChar = 0;
        }
        
        //printf("BLOCK#%d BIT#%d %c + %d = %c\n", currentKey, currentChar, ch, keys[currentKey], ch + keys[currentKey]);
        blocks[currentKey][currentChar] = words[c] + keys[currentKey] % 26;   // MIGHT HAVE TO % 26 BEFORE ADDING KEYS
        //printf("%d ", keys[currentKey]);
        //printf("Block#%d %c\n", currentKey, blocks[currentKey][currentChar]);
        currentChar += 1;
    }
    
    leftRotate(blocks[currentKey], keys[currentKey] % 16, currentChar);     // Rotating the array of char by x rounds to the left.
    for (int i = 0; i < currentChar; i++)
    {
        //printf("BLOCK#%d Xor'ing %c with %c = %c\n", currentKey, blocks[currentKey][i], keys[currentKey], (blocks[currentKey][i]) ^ (keys[currentKey]));
        blocks[currentKey][i] = (blocks[currentKey][i]) ^ (keys[currentKey]);
        fputc(blocks[currentKey][i], outFile);
    }
    
    printf("Encryption completed.\n");
    printf("Blocks: %d\n", keyLength);
    
    for (int b = 0; b < keyLength; b++)
        free(blocks[b]);
    
    fclose(outKey);
    fclose(outFile);
    free(keys);
    system("open .");
    
}


void decodeArx(FILE *inFile, FILE *keyFile)
{
    rewind(inFile);
    int *keys = getKeyFromFile(keyFile);
    char ch;
    int currentChar = 0;
    rewind(keyFile);
    int currentKey = 0;
    int keyLength = howLongIsTheFile(keyFile);
    char *blocks[keyLength];
    FILE *outFile;
    char fileName[maxFileNameLen];
    printf("Please enter the output file name: ");
    fgets(fileName, maxFileNameLen, stdin);
    
    validateFileName(fileName);
    
    outFile = fopen(fileName, "w");
    
    for (int b = 0; b < keyLength; b++)
        blocks[b] = ((char *) malloc(blockSize));
    
    while ((ch = fgetc(inFile)) != EOF)
    {
        if (currentChar >= blockSize)
        {
            //printf("before rotation and XOR: %s\n", blocks[currentKey]);
            rightRotate(blocks[currentKey], keys[currentKey] % 16, currentChar);     // Rotating the array of char by x rounds to the right.
            for (int i = 0; i < blockSize; i++)
            {
                blocks[currentKey][i] = (blocks[currentKey][i]) ^ (keys[currentKey]);
                fputc(blocks[currentKey][i] - keys[currentKey] % 26, outFile);
            }
            //printf("After rotation and XOR: %s\n", blocks[currentKey]);
            //printf("\n");
            currentKey += 1;
            currentChar = 0;
        }
        
        //printf("BLOCKS#%d #%d (%c^%c)=%c - %d = %c\n", currentKey, currentChar, ch, keys[currentKey], (ch ^ keys[currentKey]),  keys[currentKey], (ch ^ keys[currentKey]) - keys[currentKey] );
        
        blocks[currentKey][currentChar] = ch;
        //printf("Block#%d %c\n", currentKey, blocks[currentKey][currentChar]);
        currentChar += 1;
    }
    
    rightRotate(blocks[currentKey], keys[currentKey] % 16, currentChar);     // Rotating the array of char by x rounds to the right.
    
    for (int i = 0; i < currentChar; i++)
    {
        blocks[currentKey][i] = (blocks[currentKey][i]) ^ (keys[currentKey]);
        fputc(blocks[currentKey][i] - keys[currentKey] % 26, outFile);
    }
    
    
    for (int b = 0; b < keyLength; b++)
        free(blocks[b]);
    
    printf("Decryption completed.\n");
    free(keys);
    fclose(inFile);
    fclose(keyFile);
    fclose(outFile);
    system("open .");
}
