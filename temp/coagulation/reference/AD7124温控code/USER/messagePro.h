#ifndef __MESSAGEPRO_H_  
#define __MESSAGEPRO_H_  

#include "sys.h"
#include "queue.h"

typedef struct msgPro_t *msgPro_p;
typedef void (*pUser)(msgPro_p msfPro,char *cmd, char *param);

struct msgPro_t
{
	u8 recvPoll[1024];
	queue_t recvPollQueue;
	
	pUser userFun;
	u8 (*postMsg)(msgPro_p msgManager);
	u8 (*pWirte)(char * msg,u8 len);
	
};

u8 InitMsgPro(msgPro_p msgManager);
u8 AddUserPro(msgPro_p msgManager,pUser proFun);
#endif 
