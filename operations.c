/**************************************************************************************
 * @file    operations.c
 * @author  Antonio Vitor Grossi Bassi
 * @brief   AES transformations and mathematical operations.
 **************************************************************************************
 * @note contact info: antoniovitor.gb@gmail.com
 * @note contact info: antonio.bassi@sprace.org.br
 ************************************************************************************** 
*/

#include "operators.c"

static uint8_t subBytes(uint8_t byte)
{
    /** @brief      performs sub-bytes transformation on given byte.
     *  @param byte byte entity to be mapped to the s-Box.
    */
    uint8_t sByte = 0;

    sByte = byte;
    sByte = GF256_inv(byte);
    sByte = ( sByte ^ circularShift(sByte, 1) ^ circularShift(sByte, 2) ^ circularShift(sByte, 3) ^ circularShift(sByte, 4) ) ^ FOWARD_MAP; 
    return sByte;
    
}

static uint32_t subWord(uint32_t doubleWord)
{
    /** @brief              Performs sub-Bytes transformation on 32-bit word.
     *  @param doubleWord   32-bit entity to be mapped to the s-Box.
    */

   uint8_t byteNum = 0;
   uint8_t tmp = 0;
   uint32_t sDoubleWord = 0;

    while (byteNum < N_KEY)
    {
        tmp = ( uint8_t )( ( doubleWord ) >> ( byteNum * 8 ) );
        tmp = subBytes(tmp);
        sDoubleWord |= ( ( uint32_t ) tmp) << ( byteNum * 8 );
        byteNum++;      
    }
    
    return sDoubleWord;
}

static void subState(uint8_t (*stateBuffer)[N_KEY])
{
    /** @brief             performs sub-bytes transformation on given state.
     *  @param stateBuffer pointer to first array of state entity to be mapped to the s-Box
     * 
    */
    for (uint8_t R = 0; R < N_KEY; R++)
    {
        for (uint8_t C = 0; C < N_BLOCKS; C++)
        {
            stateBuffer[R][C] = subBytes(stateBuffer[R][C]);
        }
    }
    return;
}

static uint8_t invSubBytes(uint8_t sByte)
{
    /** @brief          performs sub-bytes transformation on given byte.
     *  @param sByte    byte entity to be mapped back from the s-Box.
    */
    uint8_t byte = 0;

    byte = sByte;
    byte = ( circularShift(byte, 1) ^ circularShift(byte,3) ^ circularShift(byte,6) ) ^ INVERSE_MAP;
    byte = GF256_inv(byte);
    return byte;
}

static uint32_t invSubWord(uint32_t sDoubleWord)
{
    /** @brief              Performs sub-Bytes transformation on 32-bit word.
     *  @param sDoubleWord  32-bit entity to be mapped back from the s-box.
    */

   uint8_t byteNum = 0;
   uint8_t tmp = 0;
   uint32_t doubleWord = 0;

    while (byteNum < N_KEY)
    {
        tmp = ( uint8_t )( ( doubleWord ) >> ( byteNum * 8 ) );
        tmp = invSubBytes(tmp);
        doubleWord |= ( ( uint32_t ) tmp) << ( byteNum * 8 );
        byteNum++;      
    }
    
    return doubleWord;
}

static void invSubState(uint8_t (*stateBuffer)[N_KEY])
{
    /** @brief             performs sub-bytes transformation on given state.
     *  @param stateBuffer pointer to first array of state entity to be mapped back from the s-Box
     * 
    */
    for (uint8_t R = 0; R < N_KEY; R++)
    {
        for (uint8_t C = 0; C < N_BLOCKS; C++)
        {
            stateBuffer[R][C] = invSubBytes(stateBuffer[R][C]);
        }
    }
    return;
}

static void shiftRows(uint8_t (*stateBuffer)[N_KEY])
{
    /** @brief              shifts rows from state entity.
     *  @param stateBuffer  pointer to first array of state entity.
    */
    uint32_t tmp = 0;

    for (uint8_t R = 0; R < N_KEY; R++)
    {
        tmp = wordRow(stateBuffer, R);
        tmp = cyclicalShift(tmp, R);
        
        for(uint8_t C = 0; C < N_BLOCKS; C++)
        {
            stateBuffer[R][C] = (uint8_t)(tmp >> 8*C); 
        }
    }
    return;
}

static void invShiftRows(uint8_t (*stateBuffer)[N_KEY])
{
    /** @brief              performs the inverse shift rows on state entity
     *  @param stateBuffer  pointer to first array of state entity.
    */

   uint32_t tmp = 0;

   for (uint8_t R = 0; R < N_KEY; R++)
    {
       tmp = wordRow(stateBuffer, R);
       tmp = invCyclicalShift(tmp, R);

        for (uint8_t C = 0; C < N_BLOCKS; C++)
        {
            stateBuffer[R][C] = (uint8_t)(tmp >> 8*C);
        }
    }
    return;
}

static void mixColumns(uint8_t (*stateBuffer)[N_KEY])
{
    /** @brief              perform mix columns transformations on state.
     *  @param stateBuffer  pointer to first array of state entity.
    */
    uint8_t C = 0;
    uint8_t tmp[N_BLOCKS][N_KEY] = {0};

    matcpy(stateBuffer, tmp);
    
    while(C < N_BLOCKS)
    {
        stateBuffer[0][C] = GF256_multi(0x02, tmp[0][C]) ^ GF256_multi(0x03, tmp[1][C]) ^ tmp[2][C] ^ tmp[3][C];
        stateBuffer[1][C] = tmp[0][C] ^ GF256_multi(0x02, tmp[1][C]) ^ GF256_multi(0x03, tmp[2][C]) ^ tmp[3][C];
        stateBuffer[2][C] = tmp[0][C] ^ tmp[1][C] ^ GF256_multi(0x02, tmp[2][C]) ^ GF256_multi(0x03, tmp[3][C]);
        stateBuffer[3][C] = GF256_multi(0x03, tmp[0][C]) ^ tmp[1][C] ^ tmp[2][C] ^ GF256_multi(0x02, tmp[3][C]);
        C++;
    }
    return;
}

static void invMixColumns(uint8_t (*stateBuffer)[N_KEY])
{
    /** @brief              perform the inverse mix columns transformations on state.
     *  @param stateBuffer  pointer to first array of state entity.
    */
    uint8_t C = 0;
    uint8_t tmp[N_BLOCKS][N_KEY] = {0};

    matcpy(stateBuffer, tmp);

    while ( C < N_BLOCKS )
    {
        stateBuffer[0][C] = GF256_multi(0x0e, tmp[0][C]) ^ GF256_multi(0x0b, tmp[1][C]) ^ GF256_multi(0x0d, tmp[2][C]) ^ GF256_multi(0x09, tmp[3][C]);
        stateBuffer[1][C] = GF256_multi(0x09, tmp[0][C]) ^ GF256_multi(0x0e, tmp[1][C]) ^ GF256_multi(0x0b, tmp[2][C]) ^ GF256_multi(0x0d, tmp[3][C]);
        stateBuffer[2][C] = GF256_multi(0x0d, tmp[0][C]) ^ GF256_multi(0x09, tmp[1][C]) ^ GF256_multi(0x0e, tmp[2][C]) ^ GF256_multi(0x0b, tmp[3][C]);
        stateBuffer[3][C] = GF256_multi(0x0b, tmp[0][C]) ^ GF256_multi(0x0d, tmp[1][C]) ^ GF256_multi(0x09, tmp[2][C]) ^ GF256_multi(0x0e, tmp[3][C]);
        C++;
    }
    return;
}

static void keyExpansion(uint8_t* userKey, uint32_t* keySched)
{
    /** @brief          performs the key expansion algorithm from user key.
     *  @param userKey  pointer to user key array.
     *  @param keySched pointer to key schedule array.
    */
    uint32_t tmp = 0;
    uint8_t I = 0;

    while ( I < N_KEY )
    {
        keySched[I] = ( uint32_t )( userKey[4*I] | userKey[4*I + 1] << 8 | userKey[4*I + 2] << 16 | userKey[4*I + 3] << 24 );
        I++;
    }

    I = N_KEY;

    while( I < N_BLOCKS * (N_ROUNDS + 1) )
    {
        tmp = keySched[I - 1];
        if((I % N_KEY) == 0) tmp = subWord( rotWord(tmp) ) ^ rCon[I/N_KEY];
        else if( (N_KEY > 6) & (I % N_KEY == 4) ) tmp = subWord( tmp );
        else; /* do nothing */
        keySched[I] = keySched[I - N_KEY] ^ tmp;
        I++;
    }
}

static void addRoundKey(uint8_t roundNum, uint8_t (*stateBuffer)[N_KEY], uint32_t* keySched)
{
    /** @brief              adds round key from key schedule to given state
     *  @param roundNum     ongoing cryptography round.
     *  @param stateBuffer  pointer to first array of ongoing state entity.
     *  @param keySched     pointer to key schedule array.
     * 
    */
    uint8_t keyIndex = roundNum * N_BLOCKS;
    uint32_t tmp = 0;

    for (uint8_t C = 0; C < N_KEY; C++)
    {
        tmp = wordColumn(stateBuffer, C);
    	tmp ^= keySched[keyIndex];

        for (uint8_t R = 0; R < N_BLOCKS; R++)
        {
            stateBuffer[R][C] = ( uint8_t )( tmp >> 8 * R );
        }

        keyIndex++;
    }
    return;
}
