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
	unsigned int nNodeSize;			//ÿ���ڵ�Ĵ�С
	unsigned int nNodeMax;			//FIFO�ĳ��ȣ��ڵ�ĸ���
	void *pvDataBuff;				//FIFO �������ڴ�
	void *pcNodeBuff;				//���ڵ㻺�� Ĭ�����ڴ�����һ���ڵ�������
	void *pvRead;					//��ָ��       
	void *pvWrite;					//дָ��
	unsigned int nNodeCount;	    //�ڵ�����	
	unsigned int nReadTimes;        //���Ĵ���
	unsigned int nWriteTimes;		//д�Ĵ���
}FIFO_T, *P_FIFO_T;



///////////////////////////////////////////////////////////////
//	�� �� �� : FIFO_Creat
//	�������� : FIFO�Ĵ���
//	������� : 
//				
//	����˵�� : fifo_array : fifo�����ڴ��
//             pstFifo    : FIFO������.
//			   nNodeMax   : ���ڵ�����ʵ��Ҫ��1
//             nNodeSize  : ÿ���ڵ�Ĵ�С
//	�� �� ֵ : 0 ��ʾ�ɹ�����0��ʾʧ�ܡ�
///////////////////////////////////////////////////////////////
P_FIFO_T FIFO_Creat(void *fifo_array, P_FIFO_T pstFifo, int nNodeMax, int nNodeSize);


///////////////////////////////////////////////////////////////
//	�� �� �� : FIFO_Push
//	�������� : ��FIFOд������
//	������� : 
//				
//	����˵�� :	hFifo : FIFO������. 
//				pvBuff: FIFO�ڵ��ָ�룬��������VOID�ͣ��������ͣ������Ҷ������µĽṹ�壬ʹ�õ��˿���������ġ�
//	�� �� ֵ : 
///////////////////////////////////////////////////////////////
void FIFO_Push(P_FIFO_T hFifo, void *pvBuff);



///////////////////////////////////////////////////////////////
//	�� �� �� : FIFO_Pop
//	�������� : ��FIFO��������
//	������� : 
//				
//	����˵�� : hFifo : FIFO������.
//	�� �� ֵ : ��Ӧ�ڵ�ĵ�ַ����Ҫ��ӡ�����Ļ�����ǿ��ת����Ŀ�����͡�
///////////////////////////////////////////////////////////////
char * FIFO_Pop(P_FIFO_T hFifo);


///////////////////////////////////////////////////////////////
//	�� �� �� : myMemcpy
//	�������� : ϵͳmemcpy������ʵ��
///////////////////////////////////////////////////////////////
void *myMemcpy(void *dest, const void *src, int count);

#endif  //_FIFO_H_
