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
    int option = 0;

    u8 wFlag = 0;
    u8 rFlag = 0;

    u8 userKey[MAX_SIZE] = {0}; 
    u8 dataIN[MAX_SIZE] = {0};
    u8 dataOUT[MAX_SIZE] = {0};
    u8 output[MAX_SIZE] = {0};
    
    while ( 1 )
    {
        system(SYS_CLI);
        printf(WELCOME_PROMPT);
        printf(MODE_PROMPT);
        printf(MENU_PROMPT);
        scanf("%d", &option);

        if( option == 1)
        {
            /* Encryption routines go here */
            requestInput(&dataIN[0], &userKey[0]);
            beginCipher(&dataIN[0], &dataOUT[0], &userKey[0]);
            wFlag = writeCipherSheet(&dataOUT[0]);

            if(wFlag == EXIT_SUCCESS)
            {
                cleanBuffer(&dataOUT[0]);
                printf(W_SUCCESS_PROMPT);
                getchar();
            }
            else if(wFlag == EXIT_FAILURE)
            {
                cleanBuffer(&dataOUT[0]);
                printf(W_FAILURE_PROMPT);
                getchar();
            }
            else if(wFlag == EXIT_USER)
            {
                cleanBuffer(&dataOUT[0]);
                printf(W_DISCARD_PROMPT);
                getchar();
                getchar();

            }
            else if(wFlag == EXIT_UNKNWN)
            {
                cleanBuffer(&dataOUT[0]);
                printf(W_UNKNOWN_PROMPT);
                getchar();
            }
            else; /* Do Nothing */
            option = 0;
        }

        else if(option == 2)
        {
            /* Decryption routines go here */
            rFlag = readCipherSheet(&dataIN[0]);
            if(rFlag == EXIT_SUCCESS)
            {
                requestKey(&userKey[0]);
                beginInverseCipher(&dataIN[0], &output[0], &userKey[0]); 
                printf("plaintext >> ");
                for (u8 I = 0; I < MAX_SIZE; I++)
                {
                    printf(" %c ", output[I]);
                }
                printf(KEY_PROMPT);
                getchar();
            }
            else if(rFlag == EXIT_FAILURE)
            {
                printf(R_FAILURE_PROMPT);
                getchar();
            }
            else if(rFlag == EXIT_USER)
            {
                printf(R_USEREXIT_PROMPT);
                getchar();    
            }
            else if(rFlag == EXIT_UNKNWN)
            {
                printf(R_UNKNOWN_PROMPT);
                getchar();
            }
            option = 0;
        }

        else if(option == 3)
        {
            system(SYS_CLI);
            printf(GOODBYE_PROMPT);
            getchar();getchar();
            system(SYS_CLI);
            return EXIT_SUCCESS; 
        }

        /* Hidden Debug Function */ 
        else if(option == 4)
        {
            system(SYS_CLI);
            printf(" ---------------- Algorithm Demonstration ---------------- \n -------------------- Data Encryption -------------------- \n ");
            printf("Enter data to be enciphered\nInput >> ");
            safeScanf(&dataIN[0]);
            printf("\nInsert your key\nInput >> ");
            safeScanf(&userKey[0]);
            beginCipher( &dataIN[0], &dataOUT[0], &userKey[0] );
            printf("plaintext >> ");
            for (u8 I = 0; I < MAX_SIZE; I++)
            {
                printf(" %c ", dataOUT[I]);
            }
            printf("\nPress <Enter> key to continue.");
            getchar();
            system(SYS_CLI);

            printf(" ---------------- Algorithm Demonstration ---------------- \n -------------------- Data Decryption -------------------- \n ");
            printf("Enter key\nInput >> ");
            safeScanf(&userKey[0]);
            beginInverseCipher( &dataOUT[0], &output[0], &userKey[0] );
            printf("plaintext >> ");
            for (u8 I = 0; I < MAX_SIZE; I++)
            {
                printf(" %c ", output[I]);
            } 
            printf("\nPress <Enter> key to continue.");
            getchar();
            cleanBuffer( &output[0] );
            system(SYS_CLI);
        }

        else; /* restart the loop */
    }
}
