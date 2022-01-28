/**************************************************************************************
 * @name    routines.c
 * @author  Antonio Vitor Grossi Bassi
 * @brief   functions executing the AES cryptography steps
 **************************************************************************************
 * @note contact info: antoniovitor.gb@gmail.com
 * @note contact info: antonio.bassi@sprace.org.br
 ************************************************************************************** 
*/

#include "aesmanip.c"
#include "operators.c"

void requestInput(u8* dataIN, u8* userKey)
{
    /** @brief              requests user input and key
     *  @param dataIN       pointer to data input array.
     *  @param userKey      pointer to key array.
    */
    system(SYS_CLI); 
    printf("Enter data to be enciphered\nInput >> ");
    safeScanf(&dataIN[0]);

    system(SYS_CLI); 
    printf("Insert your key\nInput >> ");
    safeScanf(&userKey[0]);
    
    return;
}

void requestKey(u8* userKey)
{
    /** @brief              requests user key for decryption.
     *  @param userKey      pointer to key array.
    */
    system(SYS_CLI);
    printf("Enter key\nInput >> ");
    safeScanf(&userKey[0]);
    return;
}

void beginCipher(u8* dataIN, u8* dataOUT, u8* userKey)
{
    /** @brief              starts AES cryptography process.
     *  @param dataIn       pointer to input array.
     *  @param dataOUT      pointer to output array.
     *  @param userKey      pointer to userKey array.
    */
    u8 memOffset = 0;    
    u8 stateBuffer[N_BLOCKS][N_BLOCKS] = {0};
    u32 keySched[N_BLOCKS * (N_ROUNDS + 1)] = {0};

    keyExpansion(&userKey[0], &keySched[0]);
    while ( memOffset < N_BLOCKS )
    {
        state(&dataIN[0], stateBuffer, memOffset);
        /* the cryptography begins here */
        addRoundKey(0, stateBuffer, &keySched[0]);
        for(u8 round = 1; round < N_ROUNDS; round++)
        {
            subState(stateBuffer);
            shiftRows(stateBuffer);
            mixColumns(stateBuffer);
            addRoundKey(round, stateBuffer, &keySched[0]);
        }
        subState(stateBuffer);
        shiftRows(stateBuffer);
        addRoundKey(10, stateBuffer, &keySched[0]);

        write(&dataOUT[0], stateBuffer, memOffset);
        memOffset++;    /* repeats for each state block present */
    }    
    cleanKey( &userKey[0] );
    cleanBuffer( &dataIN[0] );
    return;
}

void beginInverseCipher(u8* dataIN, u8* dataOUT, u8* userKey)
{
    /** @brief              starts AES decryption process.
     *  @param dataIn       pointer to input array.
     *  @param dataOUT      pointer to output array.
     *  @param stateBuffer  pointer to stateBuffer arrays.
    */

    u8 memOffset = 0;    
    u8 stateBuffer[N_BLOCKS][N_BLOCKS] = {0};
    u32 keySched[N_BLOCKS * (N_ROUNDS + 1)] = {0};

    keyExpansion(&userKey[0], &keySched[0]);
    while (memOffset < N_BLOCKS)
    {
        state(&dataIN[0], stateBuffer, memOffset);
        addRoundKey(10, stateBuffer, &keySched[0]);
        for(u8 round = N_ROUNDS - 1; round > 0; round--)
        {
            invShiftRows(stateBuffer);
            invSubState(stateBuffer);
            addRoundKey(round, stateBuffer, &keySched[0]);
            invMixColumns(stateBuffer);
        }
        invShiftRows(stateBuffer);
        invSubState(stateBuffer);
        addRoundKey(0, stateBuffer, &keySched[0]);

        write(&dataOUT[0], stateBuffer, memOffset);
        memOffset++;
    }
    cleanKey( &userKey[0] );
    return;
}

u8 writeCipherSheet(u8* dataOUT)
{
    int option = 0;

    system(SYS_CLI);

    printf("Do you wish to store enciphered data in the cipher sheet?\n(1) Yes\n(2) No\nInput >> ");

    scanf("%d", &option);

    if(option == 1) return writeFile(&dataOUT[0]);
    else if(option == 2) return EXIT_USER; 
    else return EXIT_UNKNWN;
}

u8 readCipherSheet(u8* dataIN)
{
    int option = 0;

    system(SYS_CLI);

    printf("Retrieve cipher from sheet?\n(1) Yes\n(2) No\nInput >> ");

    scanf("%d", &option);

    if(option == 1) return readFile(&dataIN[0]);
    else if(option == 2) return EXIT_USER;
    else return EXIT_UNKNWN;
    return EXIT_SUCCESS;
}
