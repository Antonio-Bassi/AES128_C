/**************************************************************************************
 * @name    operators.c
 * @author  Antonio Vitor Grossi Bassi
 * @brief   Galois Field 256 arithmetic and data structure operators.
 **************************************************************************************
 * @note contact info: antoniovitor.gb@gmail.com
 * @note contact info: antonio.bassi@sprace.org.br
 ************************************************************************************** 
*/

static void safeScanf(uint8_t* dataIN)
{
    /** @brief          scans user input, stores in array and masks input with '*'
     *  @param dataIN   pointer to byte array  
    */
    uint8_t ch = 0;
    uint8_t I = 0;

    while( I < 4 * N_KEY * N_BLOCKS )
    {
        ch = getch();
        if( ch == 0x0D) 
        {
            printf("\n");
            return;
        }

        else
        {
            dataIN[I] = ch;   
            ch = '*';
            printf("%c", ch);
        }

        I++;
    }
    printf("\n");
    return;
}

static void load(uint8_t* dataIN, uint8_t* cipherIN, uint8_t memOffset)
{
    /** @brief           loads data from user input into 16 byte array to be processed according to given offset.
     *  @param dataIN    input buffer.
     *  @param cipherIN  cipher input (will be subsequently turned into a state).
     *  @param memOffset memory offset, integer number varying from 0 to 3.
    */
    uint8_t I = 0;
    while( I < 4 * N_BLOCKS )
    {
        cipherIN[I] = dataIN[I + 16 * memOffset ];
        I++;
    }
    return;
}

static void write(uint8_t* dataOUT, uint8_t (*stateBuffer)[N_KEY], uint8_t memOffset)
{
    /** @brief              writes cipher output to buffer.
     *  @param dataOUT      pointer to output array.
     *  @param stateBuffer  pointer to an array of N_KEY Bytes.
     *  @param memOffset    memory offset, integer number varying from 0 to 3.
    */
    for (uint8_t R = 0; R < N_KEY; R++)
    {
        for (uint8_t C = 0; C < N_BLOCKS; C++)
        {
            dataOUT[( R + 4*C ) + ( 16 * memOffset )] = stateBuffer[R][C];   
        }
    }
    return;
}

static void cleanKey(uint8_t* userKey)
{
    /** @brief          cleans user Key buffer.
     *  @param userKey  pointer to user key array.
    */
   for (uint8_t I = 0; I < 4 * N_BLOCKS; I++)
   {
       userKey[I] = 0;
   }
   return;
}

static void cleanBuffer(uint8_t* dataBuffer)
{
    /** @brief              Cleans data input buffers.
     *  @param dataBuffer   Pointer to dataIN or dataOUT buffers.
     * 
    */
   for(uint8_t I = 0; I < 4 * N_BLOCKS * N_KEY; I++)
   {
       dataBuffer[I] = 0;
   }
}

static void state(uint8_t* dataIN, uint8_t (*stateBuffer)[N_KEY] )
{
    /** @brief              re-organize input array into a state matrix.
     *  @param dataIN       pointer to byte array.
     *  @param stateBuffer  pointer to an array of N_KEY Bytes.
    */
    for (uint8_t R = 0; R < N_KEY; R++)
    {
        for (uint8_t C = 0; C < N_BLOCKS; C++)
        {
            stateBuffer[R][C] = dataIN[R + 4*C];   
        }
    }

    return;
}

static uint32_t wordRow(uint8_t (*stateBuffer)[N_KEY], uint8_t row)
{
    /** @brief              extracts bytes from stateBuffer rows and creates 32-bit "words" with them.
     *  @param stateBuffer  pointer to an array of N_KEY Bytes.
     *  @param row          desired row from state to be converted into a single doubleword entity.
    */

    uint32_t word = 0;
    uint8_t C = 0;

    while( C < N_KEY )
    {
        word |= ( (uint32_t) stateBuffer[row][C] ) << ( 8 * C );
        C++;
    }

    return word;
}

static uint32_t wordColumn(uint8_t (*stateBuffer)[N_KEY], uint8_t column)
{
    /** @brief              extracts bytes from stateBuffer columns and creates 32-bit words with them.
     *  @param stateBuffer  pointer to an array of N_KEY bytes.
     *  @param column       desired column from state to be converted into a single doubleword entity. 
    */

   uint32_t word = 0;
   uint8_t R = 0;

   while ( R < N_BLOCKS )
   {
       word |= ( (uint32_t) stateBuffer[R][column] ) << ( 8 * R );
       R++; 
   }

   return word;
   
}

static void matcpy(uint8_t (*stateBuffer)[N_KEY], uint8_t (*tmpBuffer)[N_KEY])
{
    /** @brief              copies elements from stateBuffer to tmpBuffer
     *  @param stateBuffer  state containing elements to be copied.
     *  @param tmpBuffer    buffer to store copied content. 
    */
    for (uint8_t R = 0; R < N_KEY; R++)
    {
        for (uint8_t C = 0; C < N_BLOCKS; C++)
        {
            tmpBuffer[R][C] = stateBuffer[R][C];
        }      
    }
    return;
}

static uint8_t circularShift(uint8_t byte, uint8_t numOfShifts)
{
    /** @brief              Perform a circular shift on given byte and number of shifts
     *  @param invByte      Inverted byte by GF256_inverseMultiplier function to be shifted
     *  @param numOfShifts  Number of bit shifts.
    */
    return ( byte << numOfShifts ) | ( byte >> (8 - numOfShifts ) );
}

static uint32_t cyclicalShift(uint32_t word, uint8_t numOfShifts)
{
    /** @brief              Perform a circular shift on given doubleword
     *  @param word         32-bit entity.
     *  @param numOfShifts  number of byte shifts.
    */

    return ( word >> ( 8 * numOfShifts ) ) | ( word << ( 32 - 8 * numOfShifts ) );
}

static uint32_t invCyclicalShift(uint32_t word, uint8_t numOfShifts)
{
    /** @brief              Perform a inverse circular shift on given doubleword
     *  @param word         32-bit entity.
     *  @param numOfShifts  number of byte shifts.
    */

    return ( word << ( 8 * numOfShifts ) ) | ( word >> ( 32 - 8 * numOfShifts ) );
}

static uint32_t rotWord(uint32_t doubleWord)
{
    /**  @brief              rotates 32-Bit unsigned word
     *   @param doubleWord   32-bit entity to be rotated
    */

    return ( doubleWord >> 8 ) | ( doubleWord << 24 );
}

static uint8_t GF256_multi(uint8_t num_A, uint8_t num_B)
{
    /** @brief          Multiplication operation inside the Galois Field of 256 binary elements
     *  @param  num_A   Integer for multiplication.
     *  @param  num_B   Integer for multiplication.
     * 
    */

    uint8_t product = 0;
    while(num_A && num_B)
    {
        if( num_B & 1 ) product ^= num_A;
        if( num_A & 0x80 ) num_A = ( num_A << 1 ) ^ RIJNDAEL_P;
        else num_A <<= 1;
        num_B >>= 1;      
    }
    return product;
}

static uint8_t GF256_inv(uint8_t num_A)
{
    /** @brief          Determines inverse multiplier in a Galois Field of 256 elements from given argument 
     *  @param  num_A   Integer to be inverted.
     * 
    */

    uint8_t num_X = 0;
    for(num_X = 1; num_X < RIJNDAEL_P; num_X++)
    {
        if( GF256_multi( num_A % RIJNDAEL_P, num_X % RIJNDAEL_P ) % RIJNDAEL_P == 1 ) return num_X;
        else if(num_A == 0) return num_A; 
        else; /*Do nothing*/
    }

    return EXIT_FAILURE;
}