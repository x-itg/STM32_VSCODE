#ifndef __QUEUE_H_  
#define __QUEUE_H_  

#include "sys.h"

typedef struct queue   
{  
    u8 *pBase;  
    u16 front;   
    u16 rear;    
    u16 maxsize; 
}queue_t,*queue_p;  
  
u8 CreateQueue(queue_p qe,u8 *buf,u16 len);    
void Enqueue(queue_p qe, u8 val);
u8 Dequeue(queue_p qe, u8 *val);  

#endif  


