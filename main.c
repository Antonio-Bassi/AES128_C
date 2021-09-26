/**************************************************************************************
 * @file    main.c
 * @author  Antonio Vitor Grossi Bassi
 * @brief   main programn body
 **************************************************************************************
 * @note AES cryptography implementation following the Federal Information Processing
 * Standards 197 (FIPS197).
 * 
 * @note contact info: antoniovitor.gb@gmail.com
 * @note contact info: antonio.bassi@sprace.org.br
 * ************************************************************************************
*/

#include "params.h"

int main(void)
{
    int userCursor = 0;

    uint8_t wFlag = 0;
    uint8_t rFlag = 0;

    uint8_t userKey[4 * N_BLOCKS] = {0}; 
    uint8_t dataIN[4 * N_KEY * N_BLOCKS] = {0};
    uint8_t dataOUT[4 * N_KEY * N_BLOCKS] = {0};
    uint8_t output[4 * N_KEY * N_BLOCKS] = {0};
    
    while ( 1 )
    {
        system("cls");
        printf("Personal cryptographer v0.0\n");
        printf("Welcome, what do you wish to do?\n(1) Encrypt data\n(2) Decrypt data\n(3) Quit\nInput >> ");
        scanf("%d", &userCursor);

        if( userCursor == 1)
        {
            /* Encryption routines go here */
            requestInput(&dataIN[0], &userKey[0]);
            beginCipher(&dataIN[0], &dataOUT[0], &userKey[0]);
            wFlag = writeCipherSheet(&dataOUT[0]);

            if(wFlag == EXIT_SUCCESS)
            {
                cleanBuffer(&dataOUT[0]);
                printf("Ciphertext successfully written.\nPress any key to continue.");
                getch();
            }
            else if(wFlag == EXIT_FAILURE)
            {
                cleanBuffer(&dataOUT[0]);
                printf("Ciphertext was not written due to NULL file pointer.\nPress any key to continue.");
                getch();
            }
            else if(wFlag == EXIT_USER)
            {
                cleanBuffer(&dataOUT[0]);
                printf("Ciphertext successfully discarded.\nPress any key to continue.");
                getch();
            }
            else if(wFlag == EXIT_UNKNWN)
            {
                cleanBuffer(&dataOUT[0]);
                printf("Ciphertext was not written due to unknown error.\nPress any key to continue.");
                getch();
            }
            else; /* Do Nothing */
            userCursor = 0;
        }

        else if(userCursor == 2)
        {
            /* Decryption routines go here */
            rFlag = readCipherSheet(&dataIN[0]);
            if(rFlag == EXIT_SUCCESS)
            {
                requestKey(&userKey[0]);
                beginInverseCipher(&dataOUT[0], &output[0], &userKey[0]); 
                printf("plaintext >> ");
                for (uint8_t I = 0; I < 4 * N_KEY * N_BLOCKS; I++)
                {
                    printf(" %c ", output[I]);
                }
                printf("\nPress any key to continue.");
                getch();
            }
            else if(rFlag == EXIT_FAILURE)
            {
                printf("No ciphersheet found or file Pointer is NULL.\nTo create a ciphersheet use option (1) Encrypt data.\nPress any key to continue.");
                getch();
            }
            else if(rFlag == EXIT_USER)
            {
                printf("Ciphersheet was not read.\nPress any key to continue.");
                getch();    
            }
            else if(rFlag == EXIT_UNKNWN)
            {
                printf("Ciphersheet could not be read due to unknown error.\nPress any key to continue.");
                getch();
            }
            userCursor = 0;
        }

        else if(userCursor == 3)
        {
            system("cls");
            printf("Program terminated, goodbye. (/'A')/\nPress any key to leave.\n");
            getch();
            system("cls");
            return EXIT_SUCCESS; 
        }

        else; /* restart the loop */
    } 
}