/**************************************************************************************
 * @file    params.h
 * @author  Antonio Vitor Grossi Bassi
 * @brief   Header file containing necessary libraries, macros, function prototypes, etc.
 ************************************************************************************** 
 * @note contact info: antoniovitor.gb@gmail.com
 * @note contact info: antonio.bassi@sprace.org.br
 **************************************************************************************
*/

#ifndef PARAMS_H_
#define PARAMS_H_

/* Standard C */
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

/* Macros */
#define FOWARD_MAP  0X63    // Forward S-Box constant
#define INVERSE_MAP 0X05    // Inverse S-Box constant    
#define RIJNDAEL_P  0x11B   // Rijndael's polynomial

#define N_KEY       4       // Number of Words present inside given Key
#define N_BLOCKS    4       // Number of 32 bit data blocks
#define N_ROUNDS    10      // Number of cryptography rounds

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define EXIT_USER    2
#define EXIT_UNKNWN  3

/* Global Variables */
uint32_t rCon[11] = 
{ 0x00000000,
  0x00000001,
  0x00000002,
  0x00000004,
  0x00000008,
  0x00000010,
  0x00000020,
  0x00000040,
  0x00000080,
  0x0000001b,
  0x00000036,
};

/* Cooler data types */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64; 
typedef signed int s32;

/* Function Prototypes */

/* Source Codes */
#include "operations.c"
#include "filesystem.c"
#include "routines.c"

#endif