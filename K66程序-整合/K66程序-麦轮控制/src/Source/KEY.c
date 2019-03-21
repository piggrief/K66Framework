/******************** LQ_K60_函数库 v1.0 ********************
 * 文件名           ：KEY.c
 * 功能             ：设置按键口工作模式
 * 备注             ：官方例程上修改
 * 日期             ：2016-01-23
 * 实验平台         ：龙丘 k66F18核心板VG2
 * 开发环境         ：IAR 7.3及以上
 * 作者             ：CHIUSIR
 * 淘宝店           ：https://longqiu.taobao.com
 * 龙丘智能车讨论群 ：202949437
*************************************************************/

#include "include.h"


void KEY_Init(void)
{
   GPIO_Init(PTB,20,GPI,1);
   GPIO_Init(PTB,21,GPI,1);
   GPIO_Init(PTB,22,GPI,1);   
}


u8 KEY_Read(KEYn_e keyno)
{
    switch(keyno) 
    {
      case KEY0:
        return GPIO_Get(PTB20);
      break;
      
      case KEY1:
        return GPIO_Get(PTB21);
      break;
      
      case KEY2:
        return GPIO_Get(PTB22);
      break;
      default:
        return 0XFF;
           
    }
}


//

