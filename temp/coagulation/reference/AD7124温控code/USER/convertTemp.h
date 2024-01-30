#ifndef _CONVERTTEMP_H
#define _CONVERTTEMP_H

#include "sys.h"

typedef enum
{
	eKtype =0,
}TETYPE_e;

float convertTemp(TETYPE_e teType,float voltage);
	
#endif
