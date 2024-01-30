#ifndef __PROTOCCOLPARSE_H_  
#define __PROTOCCOLPARSE_H_  

#include "sys.h"
#include "queue.h"

u16 parse(queue_p qe,char *cmdbuf, char* paramBuf);
u8 package(char *in,char *out,int len,int outlen);

#endif 
