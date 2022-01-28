/* ****************************************************** 
 * @file    params.h
 * @author  Antonio Vitor Grossi Bassi
 * @brief   Header file for enviroment adaptation
 * ******************************************************
 * @note contact info -> antoniovitor.gb@gmail.com
 *                    -> antonio.bassi@sprace.org.br
 * ******************************************************
*/


#ifndef PARAMS_H_
#define PARAMS_H_

#if defined(_WIN32) || defined(_WIN64) 
  #include "win_params.h"
#elif __linux__
  #include "lx_params.h"
#endif

#endif /* PARAMS_H_ */ 
