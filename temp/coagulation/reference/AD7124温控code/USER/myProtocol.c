#include "protocolParse.h"
#include "string.h"
/******************************************************************/
/*
*自定义协议 assic 传输方式 起始位、命令 停止位
*                           @       XXX    #
*                           命令参数使用：隔离
*                           回复命令之间使用;隔离
*ex     @CMD0:0#
*recv   @1.3;2.5;555.2;#
*/
u16 parse(queue_p qe,char *cmdbuf, char* paramBuf)
{
	u8    val =0;
	u8    ret = 0;
	u8    i   = 0;
	
	if(qe == 0 || cmdbuf == 0 || paramBuf==0)
	{
		return 1;
	}
	
	do
	{
		ret = Dequeue(qe,&val);
		if(ret != 0 )
		{
			return 1;
		}
	}while(val != '@');
	
	
	
	do
	{
		ret = Dequeue(qe,&val);
		if(ret != 0 )
		{
			return 1;
		}
		cmdbuf[i++] = val;
	}while(val != ':');
	cmdbuf[i++] =  0;
	
	
	do
	{
		ret = Dequeue(qe,&val);
		if(ret != 0 )
		{
			return 1;
		}
		paramBuf[i++] = val;
	}while(val != ':');
	paramBuf[i++] =  0;
	
	return 0;
}

u8 package(char *in,char *out,int inlen,int outlen)
{
	int i=0;
	
	if(in ==0 || out==0 || outlen < inlen+2)
	{
		return 1;
	}
	
	out[0]='@';
	for(i=1;i<inlen;i++)
	{
		out[i] = in[i];
	}
	out[inlen] = '#';
	out[inlen +1] = '0';
	
	return 0;
}
