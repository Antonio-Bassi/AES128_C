/**************************************************************************************
 * @name    aesmanip.c
 * @author  Antonio Vitor Grossi Bassi
 * @brief   data structure manipulations for AES algorithm.
 **************************************************************************************
 * @note contact info -> antoniovitor.gb@gmail.com
 *                    -> antonio.bassi@sprace.org.br
 ************************************************************************************** 
*/

static void safeScanf(u8* dataIN)
{
    /** @brief          scans user input, stores in array
     *  @param dataIN   pointer to byte array  
    */
    u8 ch = (u8)( getchar() );
    u8 I = 0;
    while( I < MAX_SIZE )
    {
      ch = getchar();
      if( ch == 0x0A )
      {
        ch == '\0';
        dataIN[ I ] = ch;
        return;
      }
      else
      {
        dataIN[ I ] = ch;
        ++I;
      }
    }
    return;
}

static void write(u8* dataOUT, u8 (*stateBuffer)[N_BLOCKS], u8 memOffset)
{
    /** @brief              writes cipher output to buffer.
     *  @param dataOUT      pointer to output array.
     *  @param stateBuffer  pointer to an array of N_BLOCKS Bytes.
     *  @param memOffset    memory offset, integer number varying from 0 to 3.
    */
    for (u8 R = 0; R < N_BLOCKS; R++)
    {
        for (u8 C = 0; C < N_BLOCKS; C++)
        {
            dataOUT[( R + 4*C ) + ( 16 * memOffset )] = stateBuffer[R][C];   
        }
    }
    return;
}

static void cleanKey(u8* userKey)
{
    /** @brief          cleans user Key buffer.
     *  @param userKey  pointer to user key array.
    */
   for (u8 I = 0; I < MAX_SIZE; I++)
   {
       userKey[I] = 0;
   }
   return;
}

static void cleanBuffer(u8* dataBuffer)
{
    /** @brief              Cleans data input buffers.
     *  @param dataBuffer   Pointer to dataIN or dataOUT buffers.
     * 
    */
   for(u8 I = 0; I < 4 * N_BLOCKS * N_KEY; I++)
   {
       dataBuffer[I] = 0;
   }
}

static void state(u8* dataIN, u8 (*stateBuffer)[N_BLOCKS], u8 memOffset )
{
    /** @brief              re-organize input array into a state matrix.
     *  @param dataIN       pointer to byte array.
     *  @param stateBuffer  pointer to an array of N_BLOCKS Bytes.
    */
    for (u8 R = 0; R < N_BLOCKS; R++)
    {
        for (u8 C = 0; C < N_BLOCKS; C++)
        {
            stateBuffer[R][C] = dataIN[R + 4*C + ( 16 * memOffset )];   
        }
    }

    return;
}

static u32 wordRow(u8 (*stateBuffer)[N_BLOCKS], u8 row)
{
    /** @brief              extracts bytes from stateBuffer rows and creates 32-bit "words" with them.
     *  @param stateBuffer  pointer to an array of N_BLOCKS Bytes.
     *  @param row          desired row from state to be converted into a single doubleword entity.
    */

    u32 word = 0;
    u8 C = 0;

    while( C < N_BLOCKS )
    {
        word |= ( (u32) stateBuffer[row][C] ) << ( 8 * C );
        C++;
    }

    return word;
}

static u32 wordColumn(u8 (*stateBuffer)[N_BLOCKS], u8 column)
{
    /** @brief              extracts bytes from stateBuffer columns and creates 32-bit words with them.
     *  @param stateBuffer  pointer to an array of N_BLOCKS bytes.
     *  @param column       desired column from state to be converted into a single doubleword entity. 
    */

   u32 word = 0;
   u8 R = 0;

   while ( R < N_BLOCKS )
   {
       word |= ( (u32) stateBuffer[R][column] ) << ( 8 * R );
       R++; 
   }

   return word;
   
}

static void matcpy(u8 (*stateBuffer)[N_BLOCKS], u8 (*tmpBuffer)[N_BLOCKS])
{
    /** @brief              copies elements from stateBuffer to tmpBuffer
     *  @param stateBuffer  state containing elements to be copied.
     *  @param tmpBuffer    buffer to store copied content. 
    */
    for (u8 R = 0; R < N_BLOCKS; R++)
    {
        for (u8 C = 0; C < N_BLOCKS; C++)
        {
            tmpBuffer[R][C] = stateBuffer[R][C];
        }      
    }
    return;
}

static u8 circularShift(u8 byte, u8 numOfShifts)
{
    /** @brief              Perform a circular shift on given byte and number of shifts
     *  @param invByte      Inverted byte by GF256_inverseMultiplier function to be shifted
     *  @param numOfShifts  Number of bit shifts.
    */
    return ( byte << numOfShifts ) | ( byte >> (8 - numOfShifts ) );
}

static u32 cyclicalShift(u32 word, u8 numOfShifts)
{
    /** @brief              Perform a circular shift on given doubleword
     *  @param word         32-bit entity.
     *  @param numOfShifts  number of byte shifts.
    */

    return ( word >> ( 8 * numOfShifts ) ) | ( word << ( 32 - 8 * numOfShifts ) );
}

static u32 invCyclicalShift(u32 word, u8 numOfShifts)
{
    /** @brief              Perform a inverse circular shift on given doubleword
     *  @param word         32-bit entity.
     *  @param numOfShifts  number of byte shifts.
    */

    return ( word << ( 8 * numOfShifts ) ) | ( word >> ( 32 - 8 * numOfShifts ) );
}

static u32 rotWord(u32 doubleWord)
{
    /**  @brief              rotates 32-Bit unsigned word
     *   @param doubleWord   32-bit entity to be rotated
    */

    return ( doubleWord >> 8 ) | ( doubleWord << 24 );
} 
