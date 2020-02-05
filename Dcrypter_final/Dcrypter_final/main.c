#include "fileEnDe.h"

int main()
{
    int alive = 1;
    char response;
    
    Menu main_menu;
    Menu encrypt_menu;
    Menu decrypt_menu;
    Menu current_menu;
    
    main_menu.menuText = " ___________________________________________\n"
    "| _____                        _            |\n"
    "||  __ \\                      | |           |\n"
    "|| |  | | ___ _ __ _   _ _ __ | |_ ___ _ __ |\n"
    "|| |  | |/ __| '__| | | | '_ \\| __/ _ \\ '__||\n"
    "|| |__| | (__| |  | |_| | |_) | ||  __/ |   |\n"
    "||_____/ \\___|_|   \\__, | .__/ \\__\\___|_|   |\n"
    "|                   __/ | |                 |\n"
    "|                  |___/|_|                 |\n"
    " -------------------------------------------\n"
    "1. Encrypt\n"
    "2. Decrypt\n"
    "3. Quit\n"
    ">> ";
    
    main_menu.prior = NULL;
    main_menu.function = 0;
    main_menu.next[0] = &encrypt_menu;
    main_menu.next[1] = &decrypt_menu;
    
    encrypt_menu.menuText = "Choose encryption method (Anything else to go back to main menu)\n1. XOR cipher\n2. ascii9\n3. ARX block cipher\n>> ";
    encrypt_menu.prior = &main_menu;
    encrypt_menu.function = 1;
    
    decrypt_menu.menuText = "Choose decryption method (Anything else to go back to main menu)\n1. XOR decipher\n2. ascii9\n3. ARX block cipher\n>> ";
    decrypt_menu.prior = &main_menu;
    decrypt_menu.function = 2;
    
    current_menu = main_menu;
    
    
    while (alive) {
        printf("%s", current_menu.menuText);
        //   scanf(" %c", &response);
        response = readChar();
        printf("\n");
        if (current_menu.function == 0)
        {
            switch (response) {
                case '1':
                    current_menu = *current_menu.next[0];
                    break;
                case '2':
                    current_menu = *current_menu.next[1];
                    break;
                case '3':
                    if (current_menu.prior == NULL)
                        alive = 0;
                    else
                        current_menu = *current_menu.prior;
                    break;
                default:
                    printf("Invalid command.\n");
                    break;
            }
        }
        else
        {
            /*----------------ENCRYPTION STARTS-------------------------------------------*/
            switch (current_menu.function) {
                case 1:  // If encrypting
                {
                    
                    switch (response) {
                            
                        case '1': // If XOR encrypt
                        {
                            printf("Select encryption mode (Anything else to go back to main menu)\n1. from file\n2. from input\n>> ");
                            response = readChar();
                            printf("\n");
                            
                            switch (response) {
                                    
                                case '1': // From file
                                {
                                    xorCipherRandomKeys();
                                    current_menu = main_menu;
                                    response = 0;
                                    break;
                                }
                                    
                                case '2':  // From input
                                {
                                    xorCipherRandomKeysFromText();
                                    current_menu = main_menu;
                                    response = 0;
                                    break;
                                }
                                    
                                default:
                                {
                                    current_menu = main_menu;
                                    response = 0;
                                    break;
                                    
                                }
                            }
                            break;
                        }
                            
                        case '2': // If ascii encrypt
                        {
                            printf("Select encryption mode (Anything else to go back to main menu)\n1. from file\n2. from input\n>> ");
                            response = readChar();
                            printf("\n");
                            
                            switch (response) {
                                    
                                case '1':
                                {
                                    encodeAscii();
                                    current_menu = main_menu;
                                    response = 0;
                                    break;
                                }
                                    
                                case '2':
                                {
                                    encodeAsciiFromText();
                                    current_menu = main_menu;
                                    response = 0;
                                    break;
                                }
                                    
                                default:
                                {
                                    current_menu = main_menu;
                                    response = 0;
                                    break;
                                }
                            }
                            break;
                        }
                        case '3':  // IF ARX encrypt
                        {
                            printf("Select encryption mode (Anything else to go back to main menu)\n1. from file\n2. from input\n>> ");
                            response = readChar();
                            printf("\n");
                            
                            switch (response) {
                                    
                                case '1':
                                {
                                    blockArx();
                                    current_menu = main_menu;
                                    response = 0;
                                    break;
                                }
                                    
                                case '2':
                                {
                                    blockArxFromText();
                                    current_menu = main_menu;
                                    response = 0;
                                    break;
                                }
                                    
                                default:
                                {
                                    current_menu = main_menu;
                                    response = 0;
                                    break;
                                }
                            }
                            break;
                        }
                            
                        default:    // Neither
                        {
                            current_menu = main_menu;
                            response = 0;
                            break;
                        }
                    }
                    current_menu = main_menu;
                    response = 0;
                    break;
                }
                    
                    /*----------------------ENCRYPTION_END-------------------------*/
                    
                case 2: // If decrypting
                {
                    
                    switch (response)
                    {
                            
                        case '1':  // If decrypting XOR
                        {
                            printf("\n");
                            FILE* inFile = getInFile();
                            char keyName[maxFileNameLen];
                            printf("Enter the key (or the filename with file extension): ");
                            fgets(keyName, maxFileNameLen, stdin);
                            if (isNotFileName(keyName))
                            {
                                keyName[strlen(keyName) - 1] = '\0';
                                printf("Decrypting with user inputted key.\n");
                                decodeXORuserKey(inFile, keyName);
                            }
                            else
                            {
                                validateFileName(keyName);
                                FILE* keyFile = openFile(keyName, 0);
                                decodeXOR(inFile, keyFile);
                                fclose(keyFile);
                            }
                            current_menu = main_menu;
                            response = 0;
                            break;
                        }
                            
                        case '2': // If ascii Decrypting
                        {
                            FILE *inFile = getInFile();
                            char keyName[maxFileNameLen];
                            printf("Enter the key's filename: ");
                            fgets(keyName, maxFileNameLen, stdin);
                            validateFileName(keyName);
                            FILE *keyFile = openFile(keyName, 0);
                            int *key = getKeyFromFile(keyFile);
                            
                            decodeAscii(inFile, key);
                            current_menu = main_menu;
                            response = 0;
                            fclose(keyFile);
                            break;
                        }
                            
                        case '3': // If decrypting ARX
                        {
                            FILE *inFile = getInFile();
                            char keyName[maxFileNameLen];
                            printf("Enter the key's filename: ");
                            fgets(keyName, maxFileNameLen, stdin);
                            validateFileName(keyName);
                            FILE *keyFile = openFile(keyName, 0);
                            
                            decodeArx(inFile, keyFile);
                            current_menu = main_menu;
                            response = 0;
                            fclose(keyFile);
                            break;
                        }
                            
                        default:
                            current_menu = main_menu;
                            response = 0;
                            break;
                    }
                    break;
                }
                    
                default:
                    current_menu = main_menu;
                    response = 0;
                    break;
            }
            
        }
        
    }
    
    return 0;
}


