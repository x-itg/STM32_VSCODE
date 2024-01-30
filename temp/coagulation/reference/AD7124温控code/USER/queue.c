#include<stdio.h>  
#include"queue.h"  


u8 CreateQueue(queue_p qe,u8 *buf,u16 len)  
{  
    qe->pBase = buf;  
    if(NULL==qe->pBase)  
    {  
		return 1;   
    }  
    qe->front=0;        
    qe->rear=0;  
    qe->maxsize=len;  
	
	return 0;
}  


void ClearQueue(queue_p qe)
{
	u16 i=0;
	for(;i<qe->maxsize; i++)
	{
		qe->pBase[i] =0;
	}
	qe->front=0;        
    qe->rear=0;  
}

u8 FullQueue(queue_p qe)  
{  
    if(qe->front==(qe->rear+1)%qe->maxsize)   
	{
        return 0;  
	}
    else  
	{
        return 1;  
	}
}  


u8 EmptyQueue(queue_p qe)  
{  
    if(qe->front==qe->rear) 
	{
        return 0; 
	}		
    else  
	{
        return 1;  
	}
}  


void Enqueue(queue_p qe, u8 val)  
{  
    if(FullQueue(qe) == 0) 
	{		
        ClearQueue(qe);  
	}
  
	qe->pBase[qe->rear]=val;  
	qe->rear=(qe->rear+1)%qe->maxsize;  
}  
  

u8 Dequeue(queue_p qe, u8 *val)  
{  
    if(EmptyQueue(qe) == 0)  
    {  
        return 1;  
    }  
    else  
    {  
        *val=qe->pBase[qe->front];  
        qe->front=(qe->front+1)%qe->maxsize;  
        return 0;  
    }  
}  
