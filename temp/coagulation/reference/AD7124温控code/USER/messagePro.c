#include "messagePro.h"
#include "protocolParse.h"
#include "usart.h"
#include "string.h"

static u8 postMsg(msgPro_p msgManager)
{
	char cmdBuf[64];
	char paramBuf[64];
	if(parse(&(msgManager->recvPollQueue),cmdBuf,paramBuf) == 0)
	{
		if(msgManager->userFun != 0)
		{
			msgManager->userFun(msgManager, cmdBuf,paramBuf);
		}
	}
	return 0;
}

static u8 pWirte(char * msg,u8 len)
{
	u8 i=0;
	char msgBuf[256];
	package(msg,msgBuf,len,256);
	for(i=0;i<strlen(msgBuf);i++)
	{
		UsartWrite(msgBuf[i]);
	}
	return 0;
}


u8 InitMsgPro(msgPro_p msgManager)
{
	u16 i=0;
	if(msgManager == 0)
	{
		return 0;
	}
	
	msgManager->postMsg = postMsg;
	msgManager->pWirte  = pWirte;
	msgManager->userFun = 0;
	
	for(i=0;i<sizeof(msgManager->recvPoll)/sizeof(msgManager->recvPoll[0]);i++)
	{
		msgManager->recvPoll[0] = 0;
	}
	
	CreateQueue(&msgManager->recvPollQueue,msgManager->recvPoll,1024);
	
	return 1;
}

u8 AddUserPro(msgPro_p msgManager,pUser proFun)
{
	if(msgManager == 0)
	{
		return 0;
	}
	
	msgManager->userFun = proFun;
	
	return 1;
}
