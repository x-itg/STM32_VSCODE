#include "fifo.h"

P_FIFO_T FIFO_Creat(void *fifo_array, P_FIFO_T pstFifo,int nNodeMax,int nNodeSize)
{
	pstFifo->nNodeMax		= nNodeMax-1; //最后一个节点做缓存
	pstFifo->nNodeSize		= nNodeSize;
	pstFifo->nNodeCount		= 0;
	pstFifo->pvDataBuff		= (char *)fifo_array;
	pstFifo->pcNodeBuff		= (char *)pstFifo->pvDataBuff + pstFifo->nNodeSize * pstFifo->nNodeMax;
	pstFifo->pvRead			= pstFifo->pvDataBuff;
	pstFifo->pvWrite		= pstFifo->pvDataBuff;
	pstFifo->nReadTimes		= 0;
	pstFifo->nWriteTimes	= 0;
	return pstFifo;
}


void FIFO_Push(P_FIFO_T hFifo, void *pvBuff)
{
	P_FIFO_T pstFifo = hFifo;
	char *pcNewBuff = (char *)pvBuff;        
	char *pcBuffEnd = (char *)pstFifo->pvDataBuff + pstFifo->nNodeSize * pstFifo->nNodeMax;

	if (pstFifo->pvWrite == pcBuffEnd)
	{
		pstFifo->pvWrite = pstFifo->pvDataBuff;
		pstFifo->nWriteTimes++;

		if ((pstFifo->pvWrite == pstFifo->pvRead) &&
			(pstFifo->nWriteTimes > pstFifo->nReadTimes))
		{
			if (pstFifo->pvRead == pcBuffEnd)
			{
				pstFifo->pvRead =  pstFifo->pvDataBuff;
			}
			else
				pstFifo->pvRead = (char *)pstFifo->pvRead + pstFifo->nNodeSize;
		}
	}
	myMemcpy(pstFifo->pvWrite, pcNewBuff, pstFifo->nNodeSize);
	pstFifo->pvWrite = (char *)pstFifo->pvWrite + pstFifo->nNodeSize;
	pstFifo->nNodeCount++;
}


char * FIFO_Pop(P_FIFO_T hFifo)
{
	P_FIFO_T pstFifo = hFifo;
	char *pcBuffEnd;

	if (hFifo->pvRead == hFifo->pvWrite &&hFifo->nReadTimes == hFifo->nWriteTimes)
	{
		return NULL;
	}
	
	pcBuffEnd = (char *)pstFifo->pvDataBuff  + pstFifo->nNodeSize * pstFifo->nNodeMax;

	if ( pstFifo->pvRead == pcBuffEnd)
	{
		 pstFifo->pvRead = pstFifo->pvDataBuff;
		 pstFifo->nReadTimes++;
	}
	myMemcpy(pstFifo->pcNodeBuff, pstFifo->pvRead, pstFifo->nNodeSize);
	pstFifo->pvRead =  (char *)pstFifo->pvRead + pstFifo->nNodeSize;
	pstFifo->nNodeCount--;
	return (char*)pstFifo->pcNodeBuff;

}



void* myMemcpy(void *dst, const void *src, int size) 
{
	char *psrc, *pdst;
	if (dst == NULL || src == NULL)
		return NULL;
	if (dst <= src) {
		psrc = (char *)src;
		pdst = (char *)dst;
		while (size--)
			*pdst++ = *psrc++;
	}
	else {
		psrc = (char *)src + size - 1;
		pdst = (char *)dst + size - 1;
		while (size--) 
			*pdst-- = *psrc--;
		}
		return pdst;
}
