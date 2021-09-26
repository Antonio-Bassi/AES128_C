/**************************************************************************************
 * @name    routines.c
 * @author  Antonio Vitor Grossi Bassi
 * @brief   functions executing the AES cryptography steps
 **************************************************************************************
 * @note contact info: antoniovitor.gb@gmail.com
 * @note contact info: antonio.bassi@sprace.org.br
 ************************************************************************************** 
*/

void requestInput(uint8_t* dataIN, uint8_t* userKey)
{
    /** @brief              requests user input and key
     *  @param dataIN       pointer to data input array.
     *  @param userKey      pointer to key array.
    */
    system("cls"); 
    printf("Enter data to be enciphered\nInput >> ");
    safeScanf(&dataIN[0]);

    system("cls"); 
    printf("Insert your key\nInput >> ");
    safeScanf(&userKey[0]);
    
    return;
}

void requestKey(uint8_t* userKey)
{
    /** @brief              requests user key for decryption.
     *  @param userKey      pointer to key array.
    */
    system("cls");
    printf("Enter key\nInput >> ");
    safeScanf(&userKey[0]);
    return;
}

void beginCipher(uint8_t* dataIN, uint8_t* dataOUT, uint8_t* userKey)
{
    /** @brief              starts AES cryptography process.
     *  @param dataIn       pointer to input array.
     *  @param dataOUT      pointer to output array.
     *  @param userKey      pointer to userKey array.
    */
    uint8_t memOffset = 0;    
    uint8_t stateBuffer[N_KEY][N_BLOCKS] = {0};
    uint32_t keySched[N_BLOCKS * (N_ROUNDS + 1)] = {0};
    uint8_t cipherIN[N_KEY * N_BLOCKS] = {0};

    keyExpansion(&userKey[0], &keySched[0]);
    while ( memOffset < N_BLOCKS )
    {
        load(&dataIN[0], &cipherIN[0], memOffset);
        state(&cipherIN[0], stateBuffer);

        /* the cryptography begins here */
        addRoundKey(0, stateBuffer, &keySched[0]);
        for(uint8_t round = 1; round < N_ROUNDS; round++)
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
    cleanBuffer(&dataIN[0]);
    return;
}

void beginInverseCipher(uint8_t* dataIN, uint8_t* dataOUT, uint8_t* userKey)
{
    /** @brief              starts AES decryption process.
     *  @param dataIn       pointer to input array.
     *  @param dataOUT      pointer to output array.
     *  @param stateBuffer  pointer to stateBuffer arrays.
    */

    uint8_t memOffset = 0;    
    uint8_t stateBuffer[N_KEY][N_BLOCKS] = {0};
    uint32_t keySched[N_BLOCKS * (N_ROUNDS + 1)] = {0};
    uint8_t cipherIN[N_KEY * N_BLOCKS] = {0};

    keyExpansion(&userKey[0], &keySched[0]);
    while (memOffset < N_BLOCKS)
    {
        load(&dataIN[0], &cipherIN[0], memOffset);
        state(&cipherIN[0], stateBuffer);

        addRoundKey(10, stateBuffer, &keySched[0]);
        for(uint8_t round = N_ROUNDS - 1; round > 0; round--)
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

uint8_t writeCipherSheet(uint8_t* dataOUT)
{
    int option = 0;

    system("cls");

    printf("Do you wish to store enciphered data in the cipher sheet?\n(1) Yes\n(2) No\nInput >> ");

    scanf("%d", &option);

    if(option == 1) return writeFile(&dataOUT[0]);
    else if(option == 2) return EXIT_USER; 
    else return EXIT_UNKNWN;
}

uint8_t readCipherSheet(uint8_t* dataIN)
{
    int option = 0;

    system("cls");

    printf("Retrieve cipher from sheet?\n(1) Yes\n(2) No\nInput >> ");

    scanf("%d", &option);

    if(option == 1) return readFile(&dataIN[0]);
    else if(option == 2) return EXIT_USER;
    else return EXIT_UNKNWN;
    return EXIT_SUCCESS;
}