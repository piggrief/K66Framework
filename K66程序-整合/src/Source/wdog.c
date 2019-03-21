/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技MK66FX1M0VLQ18核心板
【编    写】CHIUSIR
【备    注】
【软件版本】V1.0
【最后更新】2016年08月20日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【交流邮箱】chiusir@163.com
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
#include "wdog.h"	//包含wdog.h头文件
       
//-------------------------------------------------------------------------*
//函数名: wdog_enable                                                      *
//功  能: 看门狗使能                                                       * 
//参  数: 无							           *	
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void wdog_enable(void)
{
    wdog_unlock();                                  //解锁看门狗，这样才能配置看门狗

    WDOG_STCTRLH |= WDOG_STCTRLH_WDOGEN_MASK;       //WDOGEN置位，使能 看门狗
}
//-------------------------------------------------------------------------*
//函数名: wdog_disable                                                     *
//功  能: 看门狗关闭                                                       * 
//参  数: 无			  					   *	
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void wdog_disable(void)
{
    //给看门狗模块解锁，以便写寄存器
    wdog_unlock();
    //关闭看门狗
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}
//-------------------------------------------------------------------------*
//函数名: wdog_unlock                                                      *
//功  能: 看门狗喂狗                                                       * 
//参  数: 无							           *	
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void wdog_feed(void)
{
    //此函数不能单步执行
    DisableInterrupts;

    //看门狗刷新寄存器
    WDOG_REFRESH = 0xA602;
    WDOG_REFRESH = 0xB480;
    
    EnableInterrupts;
    
}
//-------------------------------------------------------------------------*
//函数名: wdog_unlock                                                      *
//功  能: 看门狗解锁                                                       * 
//参  数: 无							           *	
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void wdog_unlock(void)
{    
    //关总中断
    DisableInterrupts;		
    //写解锁寄存器
    WDOG_UNLOCK = 0xC520;//此函数不能单步执行
    //完成解锁
    WDOG_UNLOCK = 0xD928;	
    //开总中断
    EnableInterrupts;		
}

//-------------------------------------------------------------------------*
//函数名: wdog_unlock                                                      *
//功  能: 看门狗设置                                                       * 
//参  数: 无							           *	
//返  回: 无                                                               *
//说  明: 无                                                               *
//-------------------------------------------------------------------------*
void wdog_init_ms(uint32 ms)
{  

    wdog_unlock();                                  //解锁看门狗，这样才能配置看门狗

    WDOG_PRESC = WDOG_PRESC_PRESCVAL(0);            //设置分频系数 = PRESCVAL +1(PRESCVAL取值范围为0~7)

    WDOG_TOVALH = ms >> 16;                         //设置喂狗时间
    WDOG_TOVALL = (uint16)ms;

    WDOG_STCTRLH = ( 0
                    | WDOG_STCTRLH_WDOGEN_MASK     //WDOGEN置位，使能 看门狗
                    //| WDOG_STCTRLH_CLKSRC_MASK   //看门狗时钟选择（0为 LDO ，1为bus 时钟）
                    | WDOG_STCTRLH_ALLOWUPDATE_MASK
                    | WDOG_STCTRLH_STOPEN_MASK
                    | WDOG_STCTRLH_WAITEN_MASK
                    | WDOG_STCTRLH_STNDBYEN_MASK                    
                    );
}