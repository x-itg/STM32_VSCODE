#include "mainwindow.h"
#include "ui_mainwindow.h"
//接收校验
unsigned char  MainWindow::decodeRec(unsigned char *pChar,unsigned short *cmd,unsigned short len)
{
  unsigned short jlen;
  if(*pChar!=0x5E)
  {
    return 0;
  }
  pChar++;
  if(*pChar!=0xE5)
  {
    return 0;
  }
  pChar++;

  jlen=*(unsigned short *)pChar;

  if(jlen!=len)//长度校验
  {
    return 0;
  }

  pChar++;
  pChar++;

  *cmd=*(unsigned short *)pChar;
  pChar++;
  pChar++;


  return 1;

}
