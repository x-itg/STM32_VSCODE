#ifndef _FIFO_H_
#define _FIFO_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define NODESIZE 5
#define NODEMAX 5
#define NODESIZE 100
typedef struct tagFIFO_T
{
	unsigned int nNodeSize;			//每个节点的大小
	unsigned int nNodeMax;			//FIFO的长度，节点的个数
	void *pvDataBuff;				//FIFO 缓冲区内存
	void *pcNodeBuff;				//单节点缓存 默认用内存块最后一个节点做缓存
	void *pvRead;					//读指针       
	void *pvWrite;					//写指针
	unsigned int nNodeCount;	    //节点数量	
	unsigned int nReadTimes;        //读的次数
	unsigned int nWriteTimes;		//写的次数
}FIFO_T, *P_FIFO_T;



///////////////////////////////////////////////////////////////
//	函 数 名 : FIFO_Creat
//	函数功能 : FIFO的创建
//	处理过程 : 
//				
//	参数说明 : fifo_array : fifo数组内存池
//             pstFifo    : FIFO的名称.
//			   nNodeMax   : 最大节点数，实际要减1
//             nNodeSize  : 每个节点的大小
//	返 回 值 : 0 表示成功，非0表示失败。
///////////////////////////////////////////////////////////////
P_FIFO_T FIFO_Creat(void *fifo_array, P_FIFO_T pstFifo, int nNodeMax, int nNodeSize);


///////////////////////////////////////////////////////////////
//	函 数 名 : FIFO_Push
//	函数功能 : 往FIFO写入数据
//	处理过程 : 
//				
//	参数说明 :	hFifo : FIFO的名称. 
//				pvBuff: FIFO节点的指针，这里是用VOID型，是任意型，后面我定义了新的结构体，使用的人可以随意更改。
//	返 回 值 : 
///////////////////////////////////////////////////////////////
void FIFO_Push(P_FIFO_T hFifo, void *pvBuff);



///////////////////////////////////////////////////////////////
//	函 数 名 : FIFO_Pop
//	函数功能 : 从FIFO读出数据
//	处理过程 : 
//				
//	参数说明 : hFifo : FIFO的名称.
//	返 回 值 : 对应节点的地址。需要打印出来的话，请强制转换到目标类型。
///////////////////////////////////////////////////////////////
char * FIFO_Pop(P_FIFO_T hFifo);


///////////////////////////////////////////////////////////////
//	函 数 名 : myMemcpy
//	函数功能 : 系统memcpy的重新实现
///////////////////////////////////////////////////////////////
void *myMemcpy(void *dest, const void *src, int count);

#endif  //_FIFO_H_
