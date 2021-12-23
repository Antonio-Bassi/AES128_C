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
#include <stdint.h>
#include <string.h>
#include <math.h>

/* Macros */
#define FOWARD_MAP  0X63    // Forward S-Box constant
#define INVERSE_MAP 0X05    // Inverse S-Box constant    
#define RIJNDAEL_P  0x11B   // Rijndael's polynomial

#define N_KEY       4                       // Number of Words present inside given Key
#define N_BLOCKS    4                       // Number of 32 bit data blocks
#define N_ROUNDS    10                      // Number of cryptography rounds
#define MAX_SIZE    4 * N_KEY * N_BLOCKS    // Max size of key and ciphertext arrays.

/* Error codes */
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define EXIT_USER    2
#define EXIT_UNKNWN  3

/* Interface related macros */
#define SYS_CLI "cls"
#define WELCOME_PROMPT "Personal Cryptographer Win0.0\n"
#define MENU_PROMPT "Welcome, what do you wish to do?\n(1) Encrypt\n(2) Decrypt\n(3) Quit\nInput >> "

#define W_SUCCESS_PROMPT "Ciphertext successfully written. \nPress <Enter> key to continue."
#define W_FAILURE_PROMPT "Ciphertext was not written due to NULL file pointer. \nPress <Enter> key to continue."
#define W_DISCARD_PROMPT "Ciphertext was successfully discarded. \nPress <Enter> key to continue."
#define W_UNKNOWN_PROMPT "Ciphertext was not written due to unknown error. \nPress <Enter> key to continue."

#define R_FAILURE_PROMPT "No ciphersheet found or file Pointer is NULL. \nPress <Enter> key to continue."
#define R_USEREXIT_PROMPT "Ciphersheet was not read. \nPress <Enter> key to continue."
#define R_UNKNOWN_PROMPT "Ciphersheet could not be read due to unknown error. \nPress <Enter> key to continue."

#define KEY_PROMPT "\nPress <Enter> key to continue."

#define GOODBYE_PROMPT "Program terminated, goodbye. (~;A;)~ \nPress <Enter> key to continue."

/* Cooler data types */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64; 
typedef signed int s32;

/* Global Variables */
u32 rCon[11] = 
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

/* Function Prototypes */

/* Source Codes */
#include "operations.c"
#include "files.c"
#include "routines.c"

#endif