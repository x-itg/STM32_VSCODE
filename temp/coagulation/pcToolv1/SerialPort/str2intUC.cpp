#include "mainwindow.h"
#include "ui_mainwindow.h"

int MainWindow::str2inthandle(char * pstr,unsigned char dilen)
{
  int res;
  res=0;
  unsigned char fuflag=0;
  unsigned char cnt=0;
  unsigned char dilentemp=dilen;
  if(*pstr=='-')
  {
    fuflag=1;
    pstr++;
  }
  while(*pstr>='0' && *pstr<='9')//如果是数字才进行转换，数字0~9的ASCII码：48~57
  {
    res = 10*res+  (*pstr++) -'0';//字符'0'的ASCII码为48,48-48=0刚好转化为数字0
  }

  if(*pstr=='.')//第一次遇到可能是小数点
  {
    pstr++;
    cnt=0;
    while(*pstr>='0' && *pstr<='9'&&dilen>0)//如果是数字才进行转换，数字0~9的ASCII码：48~57
    {
      cnt++;
      dilen--;
      res = 10*res+  (*pstr++) -'0';//字符'0'的ASCII码为48,48-48=0刚好转化为数字0
    }
    while(cnt<dilentemp)//97.50只输入97.5
    {
      cnt++;
      res = 10*res;
    }
  }else//没有小数点还要追加
  {
    while(dilen>0)
    {
      dilen--;
      res = 10*res;
    }
  }

  if(fuflag==1)
  {
    res=0-res;
  }
  return res;
}


