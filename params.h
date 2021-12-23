#ifndef PARAMS_H_
#define PARAMS_H_

#if defined(_WIN32) || defined(_WIN64) 
  #include "win_params.h"
#elif __linux__
  #include "lx_params.h"
#endif

#endif /* PARAMS_H_ */ 
