/**************************************************************************************
 * @file    win_params.h
 * @author  Antonio Vitor Grossi Bassi
 * @brief   Header file containing necessary libraries and macros for windows based OS
 ************************************************************************************** 
 * @note contact info -> antoniovitor.gb@gmail.com
 *                    -> antonio.bassi@sprace.org.br
 **************************************************************************************
*/

#ifndef WIN_PARAMS_H_
#define WIN_PARAMS_H_

#define IS_AES128 1
#define IS_AES192 0
#define IS_AES256 0

#if IS_AES128

  #define N_KEY       4                               // Number of Words present inside given Key
  #define N_BLOCKS    4                               // Number of 32 bit data blocks
  #define N_ROUNDS    10                              // Number of cryptography rounds
  #define MAX_SIZE    4 * N_KEY * N_BLOCKS            // Max size of key and ciphertext arrays.
  #define MODE_PROMPT "You Are Using AES-128 mode\n"  // User notification

#elif IS_AES192

  #define N_KEY       6                               // Number of Words present inside given Key
  #define N_BLOCKS    4                               // Number of 32 bit data blocks
  #define N_ROUNDS    12                              // Number of cryptography rounds
  #define MAX_SIZE    4 * N_KEY * N_BLOCKS            // Max size of key and ciphertext arrays.
  #define MODE_PROMPT "You are using AES-192 mode\n"  // User notification

#elif IS_AES256

  #define N_KEY       8                               // Number of Words present inside given Key
  #define N_BLOCKS    4                               // Number of 32 bit data blocks
  #define N_ROUNDS    14                              // Number of cryptography rounds
  #define MAX_SIZE    4 * N_KEY * N_BLOCKS            // Max size of key and ciphertext arrays.
  #define MODE_PROMPT "You are using AES-256 mode\n"  // User notification

#else

  #define N_KEY       4                                                    // Number of Words present inside given Key
  #define N_BLOCKS    4                                                    // Number of 32 bit data blocks
  #define N_ROUNDS    10                                                   // Number of cryptography rounds
  #define MAX_SIZE    4 * N_KEY * N_BLOCKS                                 // Max size of key and ciphertext arrays.
  #define MODE_PROMPT "No mode has been configured, using AES-128 mode\n"  // User notification

#endif

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

/* Function Prototypes */

/* Source Codes */
#include "files.c"
#include "routines.c"

#endif /* WIN_PARAMS_H_ */
