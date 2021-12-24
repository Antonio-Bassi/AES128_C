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

static u8 subBytes(u8 byte)
{
    /** @brief      performs sub-bytes transformation on given byte.
     *  @param byte byte to be mapped to the s-Box.
    */
    u8 sByte = 0;

    sByte = byte;
    sByte = GF256_inv(byte);
    sByte = ( sByte ^ circularShift(sByte, 1) ^ circularShift(sByte, 2) ^ circularShift(sByte, 3) ^ circularShift(sByte, 4) ) ^ FOWARD_MAP; 
    return sByte;
    
}

static u32 subWord(u32 doubleWord)
{
    /** @brief              Performs sub-Bytes transformation on 32-bit word.
     *  @param doubleWord   32-bit word to be mapped to the s-Box.
    */

   u8 byteNum = 0;
   u8 tmp = 0;
   u32 sDoubleWord = 0;

    while (byteNum < N_BLOCKS)
    {
        tmp = ( u8 )( ( doubleWord ) >> ( byteNum * 8 ) );
        tmp = subBytes(tmp);
        sDoubleWord |= ( ( u32 ) tmp) << ( byteNum * 8 );
        byteNum++;      
    }
    
    return sDoubleWord;
}

static void subState(u8 (*stateBuffer)[N_BLOCKS])
{
    /** @brief             Performs sub-bytes transformation on given state.
     *  @param stateBuffer pointer to first array of state entity to be mapped to the s-Box
     * 
    */
    for (u8 R = 0; R < N_BLOCKS; R++)
    {
        for (u8 C = 0; C < N_BLOCKS; C++)
        {
            stateBuffer[R][C] = subBytes(stateBuffer[R][C]);
        }
    }
    return;
}

static u8 invSubBytes(u8 sByte)
{
    /** @brief          performs inverse sub-bytes transformation on given byte.
     *  @param sByte    byte to be mapped back from the s-Box.
    */
    u8 byte = 0;

    byte = sByte;
    byte = ( circularShift(byte, 1) ^ circularShift(byte,3) ^ circularShift(byte,6) ) ^ INVERSE_MAP;
    byte = GF256_inv(byte);
    return byte;
}

static u32 invSubWord(u32 sDoubleWord)
{
    /** @brief              Performs inverse sub-Bytes transformation on 32-bit word.
     *  @param sDoubleWord  32-bit word to be mapped back from the s-box.
    */

   u8 byteNum = 0;
   u8 tmp = 0;
   u32 doubleWord = 0;

    while (byteNum < N_BLOCKS)
    {
        tmp = ( u8 )( ( doubleWord ) >> ( byteNum * 8 ) );
        tmp = invSubBytes(tmp);
        doubleWord |= ( ( u32 ) tmp) << ( byteNum * 8 );
        byteNum++;      
    }
    
    return doubleWord;
}

static void invSubState(u8 (*stateBuffer)[N_BLOCKS])
{
    /** @brief             Performs inverse sub-bytes transformation on given state.
     *  @param stateBuffer pointer to first array of state entity to be mapped back from the s-Box
     * 
    */
    for (u8 R = 0; R < N_BLOCKS; R++)
    {
        for (u8 C = 0; C < N_BLOCKS; C++)
        {
            stateBuffer[R][C] = invSubBytes(stateBuffer[R][C]);
        }
    }
    return;
}

static void shiftRows(u8 (*stateBuffer)[N_BLOCKS])
{
    /** @brief              shifts rows from state entity.
     *  @param stateBuffer  pointer to first array of state entity.
    */
    u32 tmp = 0;

    for (u8 R = 0; R < N_BLOCKS; R++)
    {
        tmp = wordRow(stateBuffer, R);
        tmp = cyclicalShift(tmp, R);
        
        for(u8 C = 0; C < N_BLOCKS; C++)
        {
            stateBuffer[R][C] = (u8)(tmp >> 8*C); 
        }
    }
    return;
}

static void invShiftRows(u8 (*stateBuffer)[N_BLOCKS])
{
    /** @brief              performs the inverse shift rows on state entity
     *  @param stateBuffer  pointer to first array of state entity.
    */

   u32 tmp = 0;

   for (u8 R = 0; R < N_BLOCKS; R++)
    {
       tmp = wordRow(stateBuffer, R);
       tmp = invCyclicalShift(tmp, R);

        for (u8 C = 0; C < N_BLOCKS; C++)
        {
            stateBuffer[R][C] = (u8)(tmp >> 8*C);
        }
    }
    return;
}

static void mixColumns(u8 (*stateBuffer)[N_BLOCKS])
{
    /** @brief              perform mix columns transformations on state.
     *  @param stateBuffer  pointer to first array of state entity.
    */
    u8 C = 0;
    u8 tmp[N_BLOCKS][N_BLOCKS] = {0};

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

static void invMixColumns(u8 (*stateBuffer)[N_BLOCKS])
{
    /** @brief              perform the inverse mix columns transformations on state.
     *  @param stateBuffer  pointer to first array of state entity.
    */
    u8 C = 0;
    u8 tmp[N_BLOCKS][N_BLOCKS] = {0};

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

static void keyExpansion(u8* userKey, u32* keySched)
{
    /** @brief          performs the key expansion algorithm from user key.
     *  @param userKey  pointer to user key array.
     *  @param keySched pointer to key schedule array.
    */
    u32 tmp = 0;
    u8 I = 0;

    while ( I < N_KEY )
    {
        keySched[I] = ( u32 )( userKey[4*I] | userKey[4*I + 1] << 8 | userKey[4*I + 2] << 16 | userKey[4*I + 3] << 24 );
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

static void addRoundKey(u8 roundNum, u8 (*stateBuffer)[N_BLOCKS], u32* keySched)
{
    /** @brief              adds round key from key schedule to given state
     *  @param roundNum     ongoing cryptography round.
     *  @param stateBuffer  pointer to first array of ongoing state entity.
     *  @param keySched     pointer to key schedule array.
     * 
    */
    u8 keyIndex = roundNum * N_BLOCKS;
    u32 tmp = 0;

    for (u8 C = 0; C < N_BLOCKS; C++)
    {
        tmp = wordColumn(stateBuffer, C);
      	tmp ^= keySched[keyIndex];

        for (u8 R = 0; R < N_BLOCKS; R++)
        {
            stateBuffer[R][C] = ( u8 )( tmp >> 8 * R );
        }

        keyIndex++;
    }
    return;
}
